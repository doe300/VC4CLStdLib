/*
 * Author: doe300
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef VC4CL_COMMON_H
#define VC4CL_COMMON_H

#include "_overloads.h"
#include "_intrinsics.h"


/*
 * Common functions
 *
 * Some functions have no maximum error in the OpenCL specification, see here: https://github.com/KhronosGroup/OpenCL-Docs/issues/33
 *
 * degrees -> 2 ULP
 * radians -> 2 ULP
 * mix -> "implementation defined"
 * smoothstep -> "implementation defined"
 * clamp, min, max, step, sign -> 0 ULP
 */

SIMPLE_3(float, clamp, float, x, float, minval, float, maxval, fmin(fmax(x, minval), maxval))
//TODO version with limits as scalar

// NOTE: using 0x1.ca5dc2p+5 (= 180/M_PI_F + 1 ULP) is slightly more accurate than using 0x1.ca5dcp+5 (180 / M_PI_F),
// but both are accurate enough for 2 ULP maximum error
SIMPLE_1(float, degrees, float, radians, 0x1.ca5dc2p+5 * radians)

SIMPLE_2(float, max, float, x, float, y, vc4cl_fmax(x, y))
SIMPLE_2_SCALAR(float, max, float, x, float, y, vc4cl_fmax(x, y))

SIMPLE_2(float, min, float, x, float, y, vc4cl_fmin(x, y))
SIMPLE_2_SCALAR(float, min, float, x, float, y, vc4cl_fmin(x, y))

//" Returns the linear blend of x and y implemented as:
// x + (y - x) * a
// a must be a value in the range 0.0 ... 1.0. If a is not in the range 0.0 ... 1.0, the return values are undefined. "

SIMPLE_3(float, mix, float, x, float, y, float, a, x + (y - x) * a)
SIMPLE_3_SCALAR(float, mix, float, x, float, y, float, a, x + (y - x) * a)

SIMPLE_1(float, radians, float, degrees, (M_PI_F / 180) * degrees)

SIMPLE_2(float, step, float, edge, float, val, val < edge ? 0.0f : 1.0f)
INLINE float2 step(float edge, float2 val) OVERLOADABLE
{
	return step((float2)edge, val);
}
INLINE float3 step(float edge, float3 val) OVERLOADABLE
{
	return step((float3)edge, val);
}
INLINE float4 step(float edge, float4 val) OVERLOADABLE
{
	return step((float4)edge, val);
}
INLINE float8 step(float edge, float8 val) OVERLOADABLE
{
	return step((float8)edge, val);
}
INLINE float16 step(float edge, float16 val) OVERLOADABLE
{
	return step((float16)edge, val);
}

COMPLEX_3(float, smoothstep, float, edge0, float, edge1, float, val,
{
	result_t tmp = clamp((result_t) (val - edge0) / (edge1 - edge0), (result_t)0.0f, (result_t)1.0f);
	return tmp * tmp * (3 - 2 * tmp);
})
INLINE float2 smoothstep(float edge0, float edge1, float2 val) OVERLOADABLE
{
	return smoothstep((float2)edge0, (float2)edge1, val);
}
INLINE float3 smoothstep(float edge0, float edge1, float3 val) OVERLOADABLE
{
	return smoothstep((float3)edge0, (float3)edge1, val);
}
INLINE float4 smoothstep(float edge0, float edge1, float4 val) OVERLOADABLE
{
	return smoothstep((float4)edge0, (float4)edge1, val);
}
INLINE float8 smoothstep(float edge0, float edge1, float8 val) OVERLOADABLE
{
	return smoothstep((float8)edge0, (float8)edge1, val);
}
INLINE float16 smoothstep(float edge0, float edge1, float16 val) OVERLOADABLE
{
	return smoothstep((float16)edge0, (float16)edge1, val);
}

SIMPLE_1(float, sign, float, val, val > 0.0f ? 1.0f : val < 0.0f ? -1.0f : 0.0f)

#endif /* VC4CL_COMMON_H */

