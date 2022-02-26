/*
 * Author: doe300
 *
 * See the file "LICENSE" for the full license governing this code.
 */
/*
 * Implements a float-float floating point type providing improved accuracy over float32.
 *
 * Algorithms and ideas taken from:
 * - Guillaume da Gracca, David Defour. Implementation of float-float operators on graphics hardware. Real Numbers and
 *   Computers 7, Jul 2006, Nancy, France. pp.23-32. hal-00021443
 *   https://hal.archives-ouvertes.fr/hal-00021443 (https://hal.archives-ouvertes.fr/hal-00021443/document)
 * - https://andrewthall.org/papers/df64_qf128.pdf
 */
#ifndef VC4CL_FLOAT_FLOAT_H
#define VC4CL_FLOAT_FLOAT_H

#include "_intrinsics.h"

/**
 * Type for extended precision floating point values.
 *
 * By combining two 32-bit floats, greatly increases accuracy. Value range is not increased!
 *
 * The "real" value calculates as UPPER + LOWER part.
 *
 * Using a native 64-bit type implicitly provides vector versions (and proper handling by compiler)
 */
typedef ulong FloatFloat;
typedef ulong2 FloatFloat2;
typedef ulong3 FloatFloat3;
typedef ulong4 FloatFloat4;
typedef ulong8 FloatFloat8;
typedef ulong16 FloatFloat16;

SIMPLE_1(float, vc4cl_upper, FloatFloat, val, vc4cl_bitcast_float(vc4cl_long_to_int(val)))
SIMPLE_1(float, vc4cl_lower, FloatFloat, val, vc4cl_bitcast_float(vc4cl_long_to_int(val >> 32)))
SIMPLE_1(float, vc4cl_lossy, FloatFloat, val, vc4cl_upper(val) + vc4cl_lower(val))

COMPLEX_2(FloatFloat, vc4cl_combine, float, upper, float, lower, {
	result_t upper_extended = vc4cl_int_to_ulong(vc4cl_bitcast_uint(upper));
	result_t lower_extended = vc4cl_int_to_ulong(vc4cl_bitcast_uint(lower));
	return upper_extended | (lower_extended << 32);
})

// faster version of vc4cl_combine(val, 0)
SIMPLE_1(FloatFloat, vc4cl_extend, float, val, vc4cl_int_to_ulong(vc4cl_bitcast_uint(val)))

// TODO avoid using this, since it runs against Inf, due to calculating val * 2^15
COMPLEX_1(FloatFloat, vc4cl_split, float, val, {
	// 2^s where p/2 <= s <= p - 1 with (p = bits in mantissa = 23)
	const float split = (float) (1u << 15); // TODO can be modified for precision
	arg_t c = (split + 1) * val;
	arg_t high = c - (c - val);
	arg_t low = val - high;
	return vc4cl_combine(high, low);
})

// COMPLEX_1(FloatFloat, vc4cl_split, double, val, {
// 	// 2^s where p/2 <= s <= p - 1 with (p = bits in mantissa = 23)
// 	const double split = (double) (1u << 29); // TODO can be modified for precision
// 	arg_t c = (split + 1) * val;
// 	arg_t high = c - (c - val);
// 	arg_t low = val - high;
// 	return vc4cl_combine(high, low);
// })

COMPLEX_2(FloatFloat, vc4cl_add, float, a, float, b, {
	float_t s = a + b;
	float_t v = s - a;
	float_t e = (a - (s - v)) + (b - v);
	return vc4cl_combine(s, e);
})

COMPLEX_2(FloatFloat, vc4cl_add, FloatFloat, a, FloatFloat, b, {
	float_t r = vc4cl_upper(a) + vc4cl_upper(b);
	float_t s0 = (((vc4cl_upper(a) - r) + vc4cl_upper(b)) + vc4cl_lower(b)) + vc4cl_lower(a);
	float_t s1 = (((vc4cl_upper(b) - r) + vc4cl_upper(a)) + vc4cl_lower(a)) + vc4cl_lower(b);
	float_t s = fabs(vc4cl_upper(a)) >= fabs(vc4cl_upper(b)) ? s0 : s1;
	return vc4cl_add(r, s);
})

SIMPLE_2(FloatFloat, vc4cl_sub, FloatFloat, a, FloatFloat, b, vc4cl_add(a, vc4cl_combine(-vc4cl_upper(b), -vc4cl_lower(b))))

COMPLEX_2(FloatFloat, vc4cl_mul, float, a, float, b, {
	float_t x = a * b;
	result_t a_split = vc4cl_split(a);
	result_t b_split = vc4cl_split(b);
	float_t error1 = x - (vc4cl_upper(a_split) * vc4cl_upper(b_split));
	float_t error2 = error1 - (vc4cl_lower(a_split) * vc4cl_upper(b_split));
	float_t error3 = error2 - (vc4cl_upper(a_split) * vc4cl_lower(b_split));
	float_t y = vc4cl_lower(a_split) * vc4cl_lower(b_split) - error3;
	return vc4cl_combine(x, y);
})

COMPLEX_2(FloatFloat, vc4cl_mul, FloatFloat, a, FloatFloat, b, {
	result_t t = vc4cl_mul(vc4cl_upper(a), vc4cl_upper(b));
	float_t t1 = vc4cl_upper(a) * vc4cl_lower(b) + vc4cl_lower(a) * vc4cl_upper(b) + vc4cl_lower(t);
	return vc4cl_add(vc4cl_upper(t), t1);
})

COMPLEX_2(FloatFloat, vc4cl_div, FloatFloat, a, FloatFloat, b, {
	float_t xn = 1.0f / vc4cl_upper(b);
	float_t yn = vc4cl_upper(a) * xn;
	result_t y = vc4cl_extend(yn);
	float_t diff = vc4cl_upper(vc4cl_sub(a, vc4cl_mul(b, y)));
	result_t prod = vc4cl_mul(xn, diff);
	return vc4cl_add(y, prod);
})

COMPLEX_1(FloatFloat, vc4cl_sqrt, FloatFloat, a, {
	float_t xn = rsqrt(vc4cl_upper(a));
	float_t yn = vc4cl_upper(a) * xn;
	result_t y = vc4cl_extend(yn);
	result_t ynsqr = vc4cl_mul(y, y); // yn^2
	float_t diff = vc4cl_upper(vc4cl_sub(a, ynsqr));
	result_t prod = vc4cl_mul(xn, diff) / 2;
	return vc4cl_add(y, prod);
})

#endif /* VC4CL_FLOAT_FLOAT_H */
