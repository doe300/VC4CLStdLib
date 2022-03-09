#define arg_t float16
#define result_t float16
#define int_t int16
#define uint_t uint16

#define CONCAT(a, b) a##b
#define CAT(a, b) CONCAT(a, b)

result_t approx_rootn(arg_t x, int_t n)
{
	// Divides the exponent by n and emplaces it back into the number
	// Adapted from: https://web.archive.org/web/20131227144655/http://metamerist.com/cbrt/cbrt.htm
	int_t i = CAT(as_, int_t)(x);
	int_t exp = (i - (int_t) (127 << 23)) / n + (int_t) (127 << 23);
	return CAT(as_, result_t)((i & (int_t) 0x807FFFFF) | (exp));
}

result_t approx_cbrt(arg_t f)
{
	// Adapted from: https://web.archive.org/web/20120111112244/http://metamerist.com/cbrt/CubeRoot.cpp
	uint_t p = CAT(as_, uint_t)(f);
	p = p / 3 + 709921077;
	return CAT(as_, result_t)(p);
}

result_t cbrt_halley_step(arg_t x, arg_t base)
{
	// Adapted from: https://web.archive.org/web/20120111112244/http://metamerist.com/cbrt/CubeRoot.cpp
	result_t x3 = x * x * x;
	return x * (x3 + base + base) / (x3 + x3 + base);
}

result_t cbrt_halley(arg_t val)
{
	// Adapted from: https://web.archive.org/web/20120111112244/http://metamerist.com/cbrt/CubeRoot.cpp
	arg_t abs = fabs(val);
	arg_t approx = approx_rootn(abs, 3);

	result_t result = approx;
#pragma loop unroll
	for(int i = 0; i < 4; ++i) // TODO can be adapted for accuracy
	{
		result = cbrt_halley_step(result, val);
	}
	return copysign(result, val);
}

__kernel void cbrt_halley_kernel(__global arg_t *out, const __global arg_t *in)
{
	uint gid = get_global_id(0);
	out[gid] = cbrt_halley(in[gid]);
}

result_t cbrt_newton_step(arg_t x, arg_t base)
{
	// Adapted from: https://web.archive.org/web/20120111112244/http://metamerist.com/cbrt/CubeRoot.cpp
	return x - (1.0f / 3.0f) * (x - base / (x * x));
}

result_t cbrt_newton(arg_t val)
{
	// Adapted from: https://web.archive.org/web/20120111112244/http://metamerist.com/cbrt/CubeRoot.cpp
	arg_t abs = fabs(val);
	arg_t approx = approx_cbrt(abs);

	result_t result = approx;
#pragma loop unroll
	for(int i = 0; i < 4; ++i) // TODO can be adapted for accuracy
	{
		result = cbrt_newton_step(result, val);
	}
	return copysign(result, val);
}

__kernel void cbrt_newton_kernel(__global arg_t *out, const __global arg_t *in)
{
	uint gid = get_global_id(0);
	out[gid] = cbrt_newton(in[gid]);
}

__kernel void cbrt_builtin_kernel(__global arg_t *out, const __global arg_t *in)
{
	uint gid = get_global_id(0);
	out[gid] = cbrt(in[gid]);
}

__kernel void cbrt_pow_kernel(__global arg_t *out, const __global arg_t *in)
{
	uint gid = get_global_id(0);
	out[gid] = pow(in[gid], 1.0f / 3.0f);
}
