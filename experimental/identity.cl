#define arg_t float16
#define result_t float16
#define int_t int16

__kernel void identity_kernel(__global arg_t *out, const __global arg_t *in)
{
	uint gid = get_global_id(0);
	out[gid] = in[gid];
}
