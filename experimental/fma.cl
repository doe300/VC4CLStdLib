#define arg_t float16
#define result_t float16
#define int_t int16
#define uint_t uint16

#define CONCAT(a, b) a##b
#define CAT(a, b) CONCAT(a, b)

float16 vc4cl_lossy(ulong16) __attribute__((overloadable));
ulong16 vc4cl_add(ulong16, ulong16) __attribute__((overloadable));
ulong16 vc4cl_sub(ulong16, ulong16) __attribute__((overloadable));
ulong16 vc4cl_mul(float16, float16) __attribute__((overloadable));
ulong16 vc4cl_mul(ulong16, ulong16) __attribute__((overloadable));
ulong16 vc4cl_extend(float16 val) __attribute__((overloadable));

result_t fma_simple(arg_t in0, arg_t in1, arg_t in2)
{
	return in0 * in1 * in2;
}

__kernel void fma_simple_kernel(
	__global arg_t *out, const __global arg_t *in0, const __global arg_t *in1, const __global arg_t *in2)
{
	uint gid = get_global_id(0);
	out[gid] = fma_simple(in0[gid], in1[gid], in2[gid]);
}

result_t fma_extended_precision(arg_t in0, arg_t in1, arg_t in2)
{
	ulong16 mul = vc4cl_mul(in0, in1);
	return vc4cl_lossy(vc4cl_add(mul, vc4cl_extend(in2)));
}

__kernel void fma_extended_precision_kernel(
	__global arg_t *out, const __global arg_t *in0, const __global arg_t *in1, const __global arg_t *in2)
{
	uint gid = get_global_id(0);
	out[gid] = fma_extended_precision(in0[gid], in1[gid], in2[gid]);
}

__kernel void fma_builtin_kernel(
	__global arg_t *out, const __global arg_t *in0, const __global arg_t *in1, const __global arg_t *in2)
{
	uint gid = get_global_id(0);
	out[gid] = fma(in0[gid], in1[gid], in2[gid]);
}
