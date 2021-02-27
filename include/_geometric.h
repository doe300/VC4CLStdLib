/*
 * Author: doe300
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef VC4CL_GEOMETRY_H
#define VC4CL_GEOMETRY_H

#include "_config.h"
#include "_overloads.h"

/*          a0   b0   a2 * b3 - a3 * b2
 * a x b =  a1 x b1 = a3 * b1 - a1 * b3
 *          a2   b2   a1 * b2 - a2 * b1
 */
INLINE float3 cross(float3 p0, float3 p1) OVERLOADABLE CONST
{
	return (float3) (p0.y * p1.z - p0.z * p1.y, p0.z * p1.x - p0.x * p1.z, p0.x * p1.y - p0.y * p1.x);
}

INLINE float4 cross(float4 p0, float4 p1) OVERLOADABLE CONST
{
	return (float4) (p0.y * p1.z - p0.z * p1.y, p0.z * p1.x - p0.x * p1.z, p0.x * p1.y - p0.y * p1.x, 0.0f);
}

/*         a0   b0
 * a * b = a1 * b1 = a1 * b1 + a2 * b2 + a3 * b3
 *         a2   b2
 */
INLINE float dot(float p0, float p1) OVERLOADABLE CONST
{
	return p0 * p1;
}

INLINE float dot(float2 p0, float2 p1) OVERLOADABLE CONST
{
	const float2 tmp = p0 * p1;
	return tmp.x + tmp.y;
}

INLINE float dot(float3 p0, float3 p1) OVERLOADABLE CONST
{
	const float3 tmp = p0 * p1;
	return tmp.x + tmp.y + tmp.z;
}

INLINE float dot(float4 p0, float4 p1) OVERLOADABLE CONST
{
	const float4 tmp = p0 * p1;
	return tmp.x + tmp.y + tmp.z + tmp.w;
}

float dot(float8 p0, float8 p1) OVERLOADABLE CONST;
float dot(float16 p0, float16 p1) OVERLOADABLE CONST;

COMPLEX_1_RETURN_SCALAR(float, length, float, p, {
	float tmp = dot(p, p);

	// To mitigate overflow errors for edge-cases, reduce large/increase small numbers, this is taken from LLVM libclc
	// E.g. since dot(x, x) calculates element-wise x^2, every exponent >= 64 goes to Infinity and every exponent <= -64 to zero!
	float inputFactor = 1.0f;
	float outputFactor = 1.0f;
	outputFactor = tmp == INFINITY ? 0x1.0p+65f : outputFactor;
	inputFactor = tmp == INFINITY ? 0x1.0p-65f : inputFactor;
	outputFactor = vc4cl_is_zero(tmp) ? 0x1.0p-86f : outputFactor;
	inputFactor = vc4cl_is_zero(tmp) ? 0x1.0p+86f : inputFactor;

	return sqrt(dot(p * inputFactor, p * inputFactor)) * outputFactor;
})

//"Returns the distance between p0 and p1.
// This is calculated as length(p0 - p1).
SIMPLE_2_RETURN_SCALAR(float, distance, float, p0, float, p1, length(p0 - p1))

/**
 * Expected behavior:
 *
 * normalize(v) = v for all elements in v = 0
 * normalize(v) = vector of NaNs for all elements in v = NaN
 * TODO special case for Inf elements
 */
SIMPLE_1(float, normalize, float, p, p / length(p))


SIMPLE_1_RETURN_SCALAR(float, fast_length, float, p, half_sqrt(dot(p, p)))

SIMPLE_2_RETURN_SCALAR(float, fast_distance, float, p0, float, p1, fast_length(p0 - p1))

SIMPLE_1(float, fast_normalize, float, p, p * half_rsqrt(dot(p, p)))

#endif /* VC4CL_GEOMETRY_H */

