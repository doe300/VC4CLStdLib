
#define CL_TARGET_OPENCL_VERSION 120
#define CL_HPP_CL_1_2_DEFAULT_BUILD 1
#define CL_HPP_ENABLE_EXCEPTIONS 1
#define CL_HPP_TARGET_OPENCL_VERSION 120
#define CL_HPP_MINIMUM_OPENCL_VERSION 120
#include <CL/cl.hpp>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unistd.h> // geteuid()
#include <vector>

static constexpr uint32_t DEFAULT_NUM_LINEAR = 12 * 16 * 8;
static constexpr uint32_t DEFAULT_NUM_RANDOM = 12 * 16 * 8;

// VC4CL performance counters
#define CL_PROFILING_PERFORMANCE_COUNTER_EXECUTION_CYCLES_VC4CL (CL_PROFILING_COMMAND_END + 10)
#define CL_PROFILING_PERFORMANCE_COUNTER_IDLE_CYCLES_VC4CL (CL_PROFILING_COMMAND_END + 11)
#define CL_PROFILING_PERFORMANCE_COUNTER_INSTRUCTION_COUNT_VC4CL (CL_PROFILING_COMMAND_END + 12)
#define CL_PROFILING_PERFORMANCE_COUNTER_INSTRUCTION_CACHE_MISSES_VC4CL (CL_PROFILING_COMMAND_END + 13)
#define CL_PROFILING_PERFORMANCE_COUNTER_L2_CACHE_MISSES_VC4CL (CL_PROFILING_COMMAND_END + 14)

struct Range
{
	float min;
	float max;
};

struct ReferenceFunction
{
	ReferenceFunction(float (*func)(float)) : numParameters(1), ptr(reinterpret_cast<void *>(func)) {}
	ReferenceFunction(float (*func)(float, float)) : numParameters(2), ptr(reinterpret_cast<void *>(func)) {}
	ReferenceFunction(float (*func)(float, float, float)) : numParameters(3), ptr(reinterpret_cast<void *>(func)) {}

	float operator()(float val) const
	{
		if(numParameters != 1)
			throw std::runtime_error{"Reference function called with the wrong number of arguments"};
		return reinterpret_cast<float (*)(float)>(ptr)(val);
	}

	float operator()(float val0, float val1) const
	{
		if(numParameters != 2)
			throw std::runtime_error{"Reference function called with the wrong number of arguments"};
		return reinterpret_cast<float (*)(float, float)>(ptr)(val0, val1);
	}

	float operator()(float val0, float val1, float val2) const
	{
		if(numParameters != 3)
			throw std::runtime_error{"Reference function called with the wrong number of arguments"};
		return reinterpret_cast<float (*)(float, float, float)>(ptr)(val0, val1, val2);
	}

	std::vector<float> operator()(const std::vector<std::vector<float>> &inputs) const
	{
		std::vector<float> out(inputs.front().size());
		for(std::size_t i = 0; i < out.size(); ++i)
		{
			if(numParameters == 1)
				out[i] = (*this)(inputs[0][i]);
			if(numParameters == 2)
				out[i] = (*this)(inputs[0][i], inputs[1][i]);
			if(numParameters == 3)
				out[i] = (*this)(inputs[0][i], inputs[1][i], inputs[2][i]);
		}
		return out;
	}

	uint8_t numParameters;
	void *ptr;
};

struct Test
{
	std::string name;
	ReferenceFunction reference;
	uint32_t allowedErrorInUlp;
	std::string sourceFile;
	std::vector<Range> ranges;
};

static float identity(float val)
{
	return val;
}

// XXX OpenCL-CTS calculates reference in double, thus is more accurate. So tests being accurate here might not be in
// the CTS!
static const std::vector<Test> floatTests = {
	Test{"log", logf, 4, "log.cl",
		{
			{0.5, 1.0}, // reduced range some implementations use
			{std::numeric_limits<float>::min(), std::numeric_limits<float>::max()} // full range
		}},
	Test{"exp", expf, 4, "exp.cl",
		{
			{0.0, 0.5f * logf(2.0f)}, // reduced range some implementations use
			{-87.0f /* everything below e^-87 is subnormal */, 89.0f /* everything above e^89 is Inf */} // full range
		}},
	Test{"identity", identity, 0, "identity.cl",
		{
			{-10.0f, 10.0f}, {std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()} // full range
		}},
	Test{"cbrt", cbrtf, 4, "cbrt.cl",
		{
			{-1.0, 1.0}, // limited range for precision testing
			{std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()} // full range
		}},
	Test{"fma", fmaf, 0, "fma.cl",
		{
			{-100.0f, 100.0f}, // reduced range to not run into NaN/Inf
			{std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max()} // full range
		}}};

static std::vector<float> generateInputData(const Range &range, uint32_t numLinear, uint32_t numRandom)
{
	std::vector<float> data{};
	data.reserve(numLinear + numRandom);
	auto step = (range.max - range.min) / static_cast<float>(numLinear); // TODO overflows on full ranges
	for(float val = range.min; val < range.max; val += step)
		data.emplace_back(val);

	std::random_device rd{};
	std::default_random_engine gen(rd());
	std::uniform_real_distribution<> dist{range.min, range.max};

	while(data.size() < (numLinear + numRandom))
		data.emplace_back(static_cast<float>(dist(gen)));

	return data;
}

static std::vector<std::vector<float>> generateInputData(
	const Range &range, uint32_t numLinear, uint32_t numRandom, uint8_t numInputs)
{
	std::vector<std::vector<float>> data{};
	for(uint8_t i = 0; i < numInputs; ++i)
		data.emplace_back(generateInputData(range, numLinear, numRandom));
	return data;
}

static std::vector<cl::Kernel> createKernels(const cl::Context &context, const Test &test)
{
	std::stringstream ss;
	{
		std::ifstream fis{test.sourceFile};
		ss << fis.rdbuf();
	}
	cl::Program program(context, ss.str(), true);

	std::vector<cl::Kernel> kernels;
	program.createKernels(&kernels);
	return kernels;
}

struct ErrorResult
{
	std::vector<float> inputValues;
	float expected;
	float actual;
	uint32_t errorInUlp;

	// ordered by "most wrong" first
	bool operator<(const ErrorResult &other) const noexcept
	{
		if(errorInUlp > other.errorInUlp)
			return true;
		if(errorInUlp < other.errorInUlp)
			return false;
		return inputValues < other.inputValues;
	}

	friend std::ostream &operator<<(std::ostream &os, const ErrorResult &error)
	{
		os << "Error of " << error.errorInUlp << " ULP for ";
		if(error.inputValues.size() == 1)
			os << std::scientific << error.inputValues.front();
		else if(error.inputValues.size() == 2)
			os << std::scientific << '{' << error.inputValues.front() << ", " << error.inputValues.back() << '}';
		else if(error.inputValues.size() == 3)
			os << std::scientific << '{' << error.inputValues[0] << ", " << error.inputValues[1] << ", "
			   << error.inputValues[2] << '}';
		else
		{
			os << '{';
			for(auto input : error.inputValues)
				os << std::scientific << input << ", ";
			os << '}';
		}
		os << ", expected " << error.expected << ", got " << error.actual << std::defaultfloat;
		return os;
	}
};

template <typename Out, typename In>
static Out bit_cast(In val)
{
	union
	{
		In in;
		Out out;
	} u;
	u.in = val;
	return u.out;
}

static uint32_t calculateError(float reference, float result, uint32_t allowedErrorInUlp)
{
	if(std::isinf(reference) && std::isinf(result) && std::signbit(reference) == std::signbit(result))
		return 0;
	if(std::isnan(reference) && std::isnan(result))
		return 0;
	// auto ulp = std::abs(reference * std::numeric_limits<float>::epsilon());
	// float difference = std::abs(result - reference);
	// if(difference > static_cast<float>(allowedErrorInUlp))
	// 	return static_cast<uint32_t>(std::ceil(difference / ulp));
	// return 0;
	return static_cast<uint32_t>(std::abs(bit_cast<int32_t>(reference) - bit_cast<int32_t>(result)));
}

static std::pair<std::vector<ErrorResult>, uint32_t> checkResults(const std::vector<std::vector<float>> &inputs,
	const std::vector<float> &reference, const std::vector<float> &result, uint32_t allowedErrorInUlp)
{
	std::vector<ErrorResult> errors;
	uint32_t maxError = 0;

	for(std::size_t i = 0; i < std::min(reference.size(), result.size()); ++i)
	{
		auto error = calculateError(reference.at(i), result.at(i), allowedErrorInUlp);
		maxError = std::max(maxError, error);
		if(error > allowedErrorInUlp)
		{
			std::vector<float> errorInputs;
			for(const auto &input : inputs)
				errorInputs.push_back(input.at(i));
			errors.emplace_back(ErrorResult{std::move(errorInputs), reference.at(i), result.at(i), error});
		}
	}

	std::sort(errors.begin(), errors.end());
	return std::make_pair(std::move(errors), maxError);
}

static void runTest(
	const cl::Context &context, const cl::CommandQueue &queue, const Test &test, uint32_t numLinear, uint32_t numRandom)
{
	std::cout << "Running test " << test.sourceFile << " ..." << std::endl;
	std::cout << "\tRunning " << test.ranges.size() << " ranges with " << (numLinear + numRandom) << " values"
			  << std::endl;
	auto kernels = createKernels(context, test);
	std::cout << "\tTesting " << kernels.size() << " implementations " << std::endl;

	for(const auto &range : test.ranges)
	{
		auto inputs = generateInputData(range, numLinear, numRandom, test.reference.numParameters);
		auto inputSize = inputs.front().size();
		cl::NDRange globalSize(inputSize / 16);
		std::vector<float> reference = test.reference(inputs);

		std::vector<cl::Buffer> inputBuffers;
		for(auto &input : inputs)
			inputBuffers.emplace_back(queue, input.begin(), input.end(), true);
		cl::Buffer outputBuffer(context, CL_MEM_READ_WRITE | CL_MEM_HOST_READ_ONLY, inputSize * sizeof(float));

		for(auto &kernel : kernels)
		{
			kernel.setArg(0, outputBuffer);
			for(std::size_t i = 0; i < inputBuffers.size(); ++i)
				kernel.setArg(1 + i, inputBuffers[i]);

			std::cout << "\tRunning kernel '" << kernel.getInfo<CL_KERNEL_FUNCTION_NAME>() << "' with "
					  << (inputSize / 16) << " work-items ... " << std::endl;
			auto start = std::chrono::steady_clock::now();
			cl::Event kernelEvent{};
			queue.enqueueNDRangeKernel(kernel, cl::NullRange, globalSize, cl::NullRange, nullptr, &kernelEvent);
			kernelEvent.wait();
			auto end = std::chrono::steady_clock::now();
			std::cout << "\t- Finished in "
					  << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " us"
					  << std::endl;
			std::chrono::nanoseconds deviceDuration{kernelEvent.getProfilingInfo<CL_PROFILING_COMMAND_END>() -
				kernelEvent.getProfilingInfo<CL_PROFILING_COMMAND_START>()};
			std::cout << "\t- Executed for "
					  << std::chrono::duration_cast<std::chrono::microseconds>(deviceDuration).count() << " us"
					  << std::endl;
			if(geteuid() == 0) // TODO only on hardware
			{
				cl_ulong numInstructions = 0;
				kernelEvent.getProfilingInfo(
					CL_PROFILING_PERFORMANCE_COUNTER_INSTRUCTION_COUNT_VC4CL, &numInstructions);
				cl_ulong numCycles = 0;
				kernelEvent.getProfilingInfo(CL_PROFILING_PERFORMANCE_COUNTER_EXECUTION_CYCLES_VC4CL, &numCycles);
				std::cout << "\t- Executed " << numInstructions << " instructions in " << numCycles << " cycles"
						  << std::endl;
			}

			std::vector<float> result(inputSize);
			queue.enqueueReadBuffer(outputBuffer, CL_TRUE, 0, inputSize * sizeof(float), result.data());
			auto errors = checkResults(inputs, reference, result, test.allowedErrorInUlp);
			std::cout << "\t- Has " << errors.first.size() << " wrong results and a maximum error of " << errors.second
					  << " ULP (of allowed " << test.allowedErrorInUlp << " ULP)" << std::endl;
			for(std::size_t i = 0; i < std::min(errors.first.size(), std::size_t{8}); ++i)
				std::cout << "\t\t" << errors.first[i] << std::endl;
			if(errors.first.size() > 8)
				std::cout << "\t\t[...]" << std::endl;
		}
	}
}

static void printHelp()
{
	std::cout << "Usage: <program> [<options>] <test> [<test>...]" << std::endl;
	std::cout << "Options: " << std::endl;
	std::cout << "\t--help         Shows this help message" << std::endl;
	std::cout << "\t--linear=<num> Specifies the number of linear test values, defaults to " << DEFAULT_NUM_LINEAR
			  << std::endl;
	std::cout << "\t--random=<num> Specifies the number of random test values, defaults to " << DEFAULT_NUM_RANDOM
			  << std::endl;
	std::cout << "Available tests: ";
	for(const auto &test : floatTests)
		std::cout << test.name << ", ";
	std::cout << std::endl;
}

int main(int argc, char **argv)
{
	uint32_t numLinear = DEFAULT_NUM_LINEAR;
	uint32_t numRandom = DEFAULT_NUM_RANDOM;

	if(argc < 2)
	{
		printHelp();
		return EXIT_SUCCESS;
	}

	auto platform = cl::Platform::get();
	cl::Device device{};
	{
		std::vector<cl::Device> devices;
		platform.getDevices(CL_DEVICE_TYPE_GPU, &devices);
		if(devices.empty())
		{
			std::cout << "No device found!" << std::endl;
			return EXIT_FAILURE;
		}
		device = devices.front();
	}
	cl::Context context(device);
	cl::CommandQueue queue(context, CL_QUEUE_PROFILING_ENABLE);

	std::vector<std::reference_wrapper<const Test>> selectedTests;
	for(int i = 1; i < argc; ++i)
	{
		if(argv[i][0] == '-')
		{
			if(std::string{"--help"} == argv[i])
			{
				printHelp();
				return EXIT_SUCCESS;
			}
			else if(strstr(argv[i], "--linear=") == argv[i])
				numLinear = static_cast<uint32_t>(std::atoi(argv[i] + strlen("--linear=")));
			else if(strstr(argv[i], "--random=") == argv[i])
				numRandom = static_cast<uint32_t>(std::atoi(argv[i] + strlen("--random=")));
			else
			{
				std::cout << "Unknown option: " << argv[i] << std::endl;
				printHelp();
				return EXIT_FAILURE;
			}
		}
		auto testIt =
			std::find_if(floatTests.begin(), floatTests.end(), [&](const Test &test) { return test.name == argv[i]; });
		if(testIt != floatTests.end())
			selectedTests.emplace_back(std::cref(*testIt));
		else
		{
			std::cout << "No such test '" << argv[i] << "', available tests: ";
			for(const auto &test : floatTests)
				std::cout << test.name << ", ";
			std::cout << std::endl;
			return EXIT_FAILURE;
		}
	}

	for(const auto &test : selectedTests)
		runTest(context, queue, test.get(), numLinear, numRandom);

	return EXIT_SUCCESS;
}
