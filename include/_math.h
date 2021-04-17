/*
 * Author: doe300
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef VC4CL_MATH_H
#define VC4CL_MATH_H

#include "_common.h"
#include "_intrinsics.h"

// TODO test-cases for all the known Edge Case Behavior
// TODO can remove nan-check from rounding functions? The large value check should also contain NaNs,
// since VC4 considers NaNs to be the "largest" values
// TODO recheck NaN handling for functions based on multiplication/division once they handle NaNs correctly,
// maybe we can remove some of those extra codes

/*
 * Remove some macros for more efficient versions
 */
#undef rsqrt // via own function
#undef ceil
#undef fabs
#undef floor
#undef fma
#undef rint
#undef round
#undef trunc
#undef half_cos
#undef half_divide
#undef half_exp
#undef half_exp2
#undef half_exp10
#undef half_log
#undef half_log2
#undef half_log10
#undef half_powr
#undef half_recip
#undef half_rsqrt
#undef half_sin
#undef half_sqrt
#undef half_tan
#undef native_cos
#undef native_divide // SFU RECIP
#undef native_exp	// via SFU EXP2
#undef native_exp2   // SFU EXP2
#undef native_exp10  // via SFU EXP2
#undef native_log	// via SFU LOG2
#undef native_log2   // SFU LOG2
#undef native_log10  // via SFU LOG2
#undef native_powr   // via SFU_EXP2
#undef native_recip  // SFU RECIP
#undef native_rsqrt  // SFU RSQRT
#undef native_sin
#undef native_sqrt // via SFU RECIP and SFU RSQRT
#undef native_tan

INLINE int factorial(int n) CONST
{
	return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}

COMPLEX_3_SCALAR(float, fast_pown, float, val, uint, n, uchar, width, {
	// reference: http://dimacs.rutgers.edu/archive/Workshops/Security/program2/quisquater/node3.html (method I)
	result_t result = 1.0f;
	result_t tmp = val;
	for(uchar i = 0; i < width; ++i)
	{
		// if n[i] == 1 then z = z * y
		result *= ((n >> i) & 1) ? tmp : 1.0f;
		// y = y * y
		tmp = tmp * tmp;
	}
	return result;
})

/**
 * Expected behavior:
 *
 * acos(1) = +-0
 * acos(x) = NaN for |x| > 1
 */
COMPLEX_1(float, acos, float, val, {
	result_t result = M_PI_2_F - asin(val);
	result = fabs(val) > (result_t)1.0f ? nan(0) : result;
	return isnan(val) ? val : result;
})

/**
 * Expected behavior:
 *
 * acosh(1) = +0
 * acosh(x) = NaN for x < 1
 * acosh(+Inf) = +Inf
 */
// https://en.wikipedia.org/wiki/Inverse_hyperbolic_functions#Series_expansions
// or: https://en.wikipedia.org/wiki/Hyperbolic_function#Inverse_functions_as_logarithms
// XXX optimize?!
SIMPLE_1(float, acosh, float, val, log(val + sqrt(val * val + 1)))

/**
 * Expected behavior:
 *
 * acospi(1) = +-0
 * acospi(x) = NaN for |x| > 1
 */
COMPLEX_1(float, acospi, float, val, {
	result_t result = acos(val) * M_1_PI_F;
	result = fabs(val) > (result_t)1.0f ? nan(0) : result;
	return isnan(val) ? val : result;
})

/**
 * Expected behavior:
 *
 * asin(+-0) = +-0
 * asin(x) = NaN for |x| > 1
 */
COMPLEX_1(float, asin, float, val, {
	result_t result = atan(val *rsqrt(1 - val * val));
	result = fabs(val) > (result_t)1.0f ? nan(0) : result;
	return isnan(val) ? val : result;
})

/**
 * Expected behavior:
 *
 * asinh(+-0) = +-0
 * asinh(+-Inf) = +-Inf
 */
// XXX optimize?!
SIMPLE_1(float, asinh, float, val, log(val + sqrt(val * val - 1)))

/**
 * Expected behavior:
 *
 * asinpi(+-0) = +-0
 * asinpi(x) = NaN for |x| > 1
 */
COMPLEX_1(float, asinpi, float, val, {
	result_t result = asin(val) * M_1_PI_F;
	result = fabs(val) > (result_t)1.0f ? nan(0) : result;
	return isnan(val) ? val : result;
})

// https://stackoverflow.com/questions/23047978/how-is-arctan-implemented
// https://en.wikipedia.org/wiki/Taylor_series#Approximation_and_convergence (too inaccurate!)
// http://www2.mae.ufl.edu/~uhk/ARCTAN-APPROX-PAPER.pdf (atan(x) = 1/x * F(1/x))
// http://www.jjj.de/fxt/fxtbook.pdf
/**
 * Expected behavior:
 *
 * atan(+-0) = +-0
 * atan(+-Inf) = +-pi/2
 *
 * Argument reduction:
 * 1) atan(-x) = -atan(x)
 * 2) atan(1/x) = PI/2 - atan(x), x > 0
 * 3) atan(1/x) = -PI/2 - atan(x), x < 0
 * 4) atan(x) = 2 atan(x / (1 + sqrt(1 + x^2)))
 *
 * Matters Computational (section 32.1.2.3): Pade Approximation (6 steps)
 * Has an error of ~ 0 for |x| <= 1/8 * PI:
 * (1155∙x + 1190∙x^3 + 231∙x^5)/(1155 + 1575∙x^2+525∙x^4+25∙x^6)
 *
 * Alternatively could be calculated via:
 * atan(x) = atan2(x, 1)
 */
COMPLEX_1(float, atan, float, val, {
	// reduce arguments a few times (r = r/(1+sqrt(1+r^2)))
	uint n = 2;
	result_t r = val;
	for(uint i = 0; i < n; ++i)
		r = r / (1 + sqrt(1 + r * r));

	// compute the pade approximation
	result_t approx = (1155 * r + 1190 * r * r * r + 231 * r * r * r * r * r) /
		(1155 + 1575 * r * r + 525 * r * r * r * r + 25 * r * r * r * r * r * r);

	// calculate the result (2^n * approx)
	result_t result = ((result_t)(2 << (n - 1))) * approx;
	// atan(x) with x >= 2^26 ~ atan(Inf) => pi/2
	result = (fabs(val) >= (result_t)0x1p+26f) ? copysign(M_PI_2_F, val) : result;
	// atan(+-NaN) = +-NaN
	return isnan(val) ? val : result;
})

/**
 * https://en.wikipedia.org/wiki/Atan2#Definition_and_computation
 *
 * Expected behavior:
 *
 * atan2(+-0, -0) = +-pi
 * atan2(+-0, +0) = +-0
 * atan2(+-0, x) = +-pi for x < 0
 * atan2(+-0, x) = +-0 for x > 0
 * atan2(y, +-0) = -pi/2 for y < 0
 * atan2(y, +-0) = pi/2 for y > 0
 * atan2(+-y, -Inf) = +-pi for y > 0 and y != Inf
 * atan2(+-y, +Inf) = +-0 for y > 0 and y != Inf
 * atan2(+-Inf, x) = +-pi/2 for x != Inf
 * atan2(+-Inf, -Inf) = +-3/4pi
 * atan2(+-Inf, +Inf) = +-pi/4
 */
COMPLEX_2(float, atan2, float, y, float, x, {
	// note: these calculation work since the condition is either 0 or all bits set
	result_t tan = atan(y / x);
	result_t s = sign(y);
	int_t xG0 = x > 0;
	int_t xL0 = x < 0;
	int_t yEq0 = y == 0;
	// arctan(y / x) if x > 0
	int_t res0 = xG0 & vc4cl_bitcast_int(tan);
	// arctan(y / x) + sign(y) * pi if x < 0, y != 0
	int_t res1 = xL0 & ~yEq0 & vc4cl_bitcast_int(tan + s * M_PI_F);
	// pi if x < 0, y = 0
	int_t res2 = xL0 & yEq0 & vc4cl_bitcast_int(M_PI_F);
	// sign(y) * pi/2 if x = 0, y != 0
	int_t res3 = (x == 0) & ~yEq0 & vc4cl_bitcast_int(s * M_PI_2_F);
	// undefined if x = 0, y = 0
	return vc4cl_bitcast_float(res0 | res1 | res2 | res3);
})

/**
 * Expected behavior:
 *
 * atanh(+-0) = +-0
 * atanh(+-1) = +-Inf
 * atanh(x) = NaN for |x| > 1
 */
SIMPLE_1(float, atanh, float, val, ((result_t) 0.5f) * log((1 + val) / (1 - val)))

/**
 * Expected behavior:
 *
 * atanpi(+-0) = +-0
 * atanpi(+-Inf) = +-0.5
 */
SIMPLE_1(float, atanpi, float, val, isnan(val) ? val : atan(val) * M_1_PI_F)

/**
 * Expected behavior:
 *
 * atan2pi(+-0, -0) = +-1
 * atan2pi(+-0, +0) = +-0
 * atan2pi(+-0, x) = +-1 for x < 0
 * atan2pi(+-0, x) = +-0 for x > 0
 * atan2pi(y, +-0) = -0.5 for y < 0
 * atan2pi(y, +-0) = 0.5 for y > 0
 * atan2pi(+-y, -Inf) = +-1 for y > 0
 * atan2pi(+-y, +Inf) = +-0 for y > 0
 * atan2pi(+-Inf, x) = +-0.5
 * atan2pi(+-Inf, -Inf) = +-0.75
 * atan2pi(+-Inf, +Inf) = +-0.25
 */
SIMPLE_2(float, atan2pi, float, x, float, y, atan2(x, y) * M_1_PI_F)

/**
 * Expected behavior:
 *
 * cbrt(+-0) = +-0
 * cbrt(+-Inf) = +-Inf
 */
// TODO different algorithm?
// e.g. http://www.hackersdelight.org/hdcodetxt/acbrt.c.txt (acbrt1 with a few more Newton steps, but requires several
// floating-point divisions)
SIMPLE_1(float, cbrt, float, val, pow(val, 1.0f / 3.0f))

/**
 * Expected behavior:
 *
 * ceil(+-0) = +-0
 * ceil(+-Inf) = +-Inf
 * ceil(x) = -0 for -1 < x < 0
 */
COMPLEX_1(float, ceil, float, val, {
	//"Round to integer toward + infinity. "

	// https://stackoverflow.com/questions/12279914/implement-ceil-in-c
	// http://blog.frama-c.com/index.php?post/2013/05/02/nearbyintf1
	// all floating point values |val| > 2^23 are exact numbers
	int_t tooBig = isnan(val) || fabs(val) >= 0x1.0p23f;
	// |val| < 2^23 fits into integer
	result_t truncated = vc4cl_itof(vc4cl_ftoi(val));
	// if the truncation is smaller than val (positive numbers), add 1 to round up
	result_t ceiling = truncated + (truncated < val ? (result_t)1.0f: (result_t)0.0f);
	// fix for ceil([-0.9, -0.0]) = -0.0
	result_t result = copysign(ceiling, val);
	// fix for negative subnormal values
	result = result < val ? result + (result_t) 1.0f : result;
	// deciding here which value to return saves us up to two jumps
	return tooBig ? val : result;
})

//"Returns x with its sign changed to match the sign of y. "
// sign = y & 0x80000000
// tmp = x & 0x7FFFFFFF
// return tmp | sign
SIMPLE_2(float, copysign, float, x, float, y,
	vc4cl_bitcast_float((vc4cl_bitcast_uint(y) & 0x80000000) | (vc4cl_bitcast_uint(x) & 0x7FFFFFFF)))

COMPLEX_1(float, cos_pade, float, val, {
	/*
	 * Pade approximation
	 * (https://ir.canterbury.ac.nz/bitstream/handle/10092/8886/brookes_thesis.pdf?sequence=1, page 86)
	 *
	 * Has a relative error of far less than 2^23 (1 ULP) for [-PI/2, PI/2], then rises fast
	 *
	 * Alternative:
	 * (see alternative for sin_pade)
	 */
	arg_t a = 45469.0f * val * val * val * val * val * val * val * val;
	arg_t b = 7029024.0f * val * val * val * val * val * val;
	arg_t c = 348731040.0f * val * val * val * val;
	arg_t d = 5269904640.0f * val * val;
	arg_t e = 10983772800.0f;
	arg_t f = 9336.0f * val * val * val * val * val * val;
	arg_t g = 2064720.0f * val * val * val * val;
	arg_t h = 221981760.0f * val * val;
	arg_t up = a - b + c - d + e;
	arg_t down = f + g + h + e;
	return up / down;
})

/**
 * Expected behavior:
 *
 * cos(+-0) = 1
 * cos(+-Inf) = NaN for |x| > 1
 */
COMPLEX_1(float, cos, float, val, {
	/*
	 * OpenCL 1.2 EMBEDDED PROFILE allows an error of up to 4 ulp
	 *
	 * We use argument reduction to bring it into range [-pi/2, pi/2] in which range the Pade approximation is accurate.
	 */
	// TODO normalization into [-pi/2, pi/2] is too inaccurate for large values!

	// Since cosine has a period of 2pi, these rewrites do not change the result (rounding error excluded):
	// bring into range [-2pi, 2pi]
	arg_t modifiedVal = fmod(val, 2.0f * M_PI_F);
	// bring into range [-pi, pi]
	modifiedVal = modifiedVal < -M_PI_F ? modifiedVal + (2.0f * M_PI_F) : modifiedVal;
	modifiedVal = modifiedVal > M_PI_F ? modifiedVal - (2.0f * M_PI_F) : modifiedVal;

	// We move by half a period, so we must invert the sign of the result
	// bring into range [-pi/2, pi/2]
	int_t invertSign = modifiedVal < -M_PI_2_F || modifiedVal > M_PI_2_F;
	modifiedVal = modifiedVal < -M_PI_2_F ? modifiedVal + M_PI_F : modifiedVal;
	modifiedVal = modifiedVal > M_PI_2_F ? modifiedVal - M_PI_F : modifiedVal;

	arg_t result = cos_pade(invertSign ? -modifiedVal : modifiedVal);
	result = invertSign ? -result : result;
	result = isinf(val) ? copysign(nan(0), val) : result;
	return isnan(val) ? val : result;
})

/**
 * Expected behavior:
 *
 * cosh(+-0) = 1
 * cosh(+-Inf) = +-Inf
 */
COMPLEX_1(float, cosh, float, val, {
	/*
	 * https://en.wikipedia.org/wiki/Taylor_series#Approximation_and_convergence
	 *
	 * This version (the Taylor-series up to x^16/16!) has following accuracy:
	 *
	 * cosh(PI) -> error of 1.5*10^-7 < ulp(PI) = 2*10^-7
	 * cosh(0.5f) -> error of < 10^-9 < ulp(0.5f) = 5*10^-8
	 * cosh(0.0f) -> error of < 10^-46 < ulp(0.0f) = 1*10^-45
	 *
	 * OpenCL 1.2 EMBEDDED PROFILE allows an error of up to 4 ulp
	 */
	const result_t fac2 = 1.0f / factorial(2);
	const result_t fac4 = 1.0f / factorial(4);
	const result_t fac6 = 1.0f / factorial(6);
	const result_t fac8 = 1.0f / factorial(8);
	const result_t fac10 = 1.0f / factorial(10);
	const result_t fac12 = 1.0f / factorial(12);
	const result_t fac14 = 1.0f / factorial(14);
	const result_t fac16 = 1.0f / factorial(16);

	/*
	 * Argument reduction
	 *
	 * Matters Computational, section 32.2.3
	 */
	const uint n = 12;
	// r = x / 2^n
	const result_t inv2N = 1.0f / (1 << n);
	result_t r = val * inv2N;

	// C = cosh(r) - 1
	result_t x = r;

	result_t x2 = x * x;
	result_t tmp1 = x2 * fac2;
	// cosh(x) = 1 + x^2/2! + ...
	result_t tmp = 1 + tmp1;
	result_t x4 = x2 * x * x;
	tmp1 = x4 * fac4;
	//... + x^4/4! + ...
	tmp = x + tmp1;
	result_t x6 = x4 * x * x;
	tmp1 = x6 * fac6;
	//... + x^6/6! + ...
	tmp = tmp + tmp1;
	result_t x8 = x6 * x * x;
	tmp1 = x8 + fac8;
	//... + x^8/8! + ...
	tmp = tmp + tmp1;
	result_t x10 = x8 * x * x;
	tmp1 = x10 * fac10;
	//... + x^10/10! + ...
	tmp = tmp + tmp1;
	result_t x12 = x10 * x * x;
	tmp1 = x12 + fac12;
	//... + x^12/12! + ...
	tmp = tmp + tmp1;
	result_t x14 = x12 * x * x;
	tmp1 = x14 * fac14;
	//... + x^14/14! + ...
	tmp = tmp + tmp1;
	result_t x16 = x14 * x * x;
	tmp1 = x16 + fac16;
	//... + x^16/16!
	tmp = tmp + tmp1;

	result_t C = tmp - 1;
	for(uint i = 0; i < n; ++i)
		C = 2 * (C + 1) * (C + 1) - 2;
	return C + 1;
})

/**
 * Expected behavior:
 *
 * cospi(+-0) = 1
 * cospi(x) = +0 for x = n + 0.5 and n is integer
 * cospi(+-Inf) = NaN
 */
SIMPLE_1(float, cospi, float, val, cos(val *M_PI_F))

/**
 * Expected behavior:
 *
 * erfc(-Inf) = 2
 * erfc(+Inf) = 0
 */
SIMPLE_1(float, erfc, float, x, 1 - erf(x))

/**
 * Expected behavior:
 *
 * erf(+-0) = +-0
 * erf(+-Inf) = +-1
 */
COMPLEX_1(float, erf, float, x, {
	/*
	 * https://en.wikipedia.org/wiki/Error_function#Numerical_approximations
	 *
	 * has a maximal error of 1.2 * 10^-7
	 *
	 * alternative: http://mathworld.wolfram.com/Erf.html
	 */
	const result_t t = 1 / (1 + 0.5f * fabs(x));

	const result_t r = t *
		exp(-x * x - 1.26551223f + 1.00002368f * t + 0.37409196f * t * t + 0.09678418f * t * t * t -
			0.18628806f * t * t * t * t + 0.27886807f * t * t * t * t * t - 1.13520398f * t * t * t * t * t * t +
			1.48851587f * t * t * t * t * t * t * t - 0.82215223f * t * t * t * t * t * t * t * t +
			0.17087277f * t * t * t * t * t * t * t * t * t);

	return x < 0 ? r - 1 : 1 - r;
})

/**
 * Expected behavior:
 *
 * exp(+-0) = 1
 * exp(-Inf) = 0
 * exp(+Inf) = +Inf
 *
 * Matters computational (sections 32.2.2.2 and 32.2.3)
 *
 * Pade Approximation (16 steps): (1680 + 840x + 180 x^2 + 20 x^3 + x^4) / (1680 - 840 x + 180 x^2 - 20 x^3 + x^4)
 *
 * with previous argument reduction
 */
COMPLEX_1(float, exp, float, val, {
	// TODO bad accurracy for negative exponents < -22
	// -> need to find a way to skip argument reduction for negative exponents
	// TODO accuracy generally too bad

	uint n = 16;
	result_t r = val / (result_t)(1 << n);
	result_t E = ((1680.0f + 840.0f * r + 180.0f * r * r + 20.0f * r * r * r + r * r * r * r) /
					 (1680.0f - 840.0f * r + 180.0f * r * r - 20.0f * r * r * r + r * r * r * r)) -
		1;
	for(uint i = 0; i < n; ++i)
		E = 2 * E + E * E;
	return E + 1;
})

/*
 * Taylor series (https://en.wikipedia.org/wiki/Taylor_series#Exponential_function) with argument reduction
 *
 * Argument reduction is taken from: http://www.netlib.org/fdlibm/
 *
 * Taylor series up to n=12 has very similar accuracy then the version in netlib (see link) and better accuracy than the
 * Pade-approximation (see Matters computational (sections 32.2.2.2 and 32.2.3))
 */
// alternatives:
// https://math.stackexchange.com/questions/1988901/approximating-the-exponential-function-with-taylor-series?rq=1 also:
// http://www.netlib.org/fdlibm/ 	COMPLEX_1(float, exp, float, val,
//	{
//		/*
//		 * e^x = e^g * 2^n
//		 *     = e^g * e^(n * ln(2))
//		 *     = e^(g + n * ln(2))
//		 * -> x = g + n * ln(2)
//		 * |g| <= 0.5* ln(2), n integer:
//		 * n = int(x + 0.5*ln(2) / ln(2))
//		 * g = x - n * ln(2)
//		 */
//		int_t n = vc4cl_ftoi((val + 0.5f * M_LN2_F) * 1.0f/ M_LN2_F);
//		result_t g = val - vc4cl_itof(n);
//		/*
//		 * e^x = e^g * 2^n
//		 * -> e^g is calculated via Taylor approximation
//		 */
//		result_t gPowN = 1;
//		result_t expG = 0;
//		for(uint n = 0; n < 13; ++n)
//		{
//			expG += gPowN * 1.0f / factorial(n);
//			gPowN *= g;
//		}
//
//		return ldexp(expG, n);
//	})

/**
 * Expected behavior:
 *
 * exp2(+-0) = 1
 * exp2(-Inf) = 0
 * exp2(+Inf) = +Inf
 */
// 2^x = e^(x * ln(2))
SIMPLE_1(float, exp2, float, x, exp(x *M_LN2_F))

/**
 * Expected behavior:
 *
 * exp10(+-0) = 1
 * exp10(-Inf) = 0
 * exp10(+Inf) = +Inf
 *
 * Handbook of Mathematical Functions (pages 70, 71)
 *
 * Using polynomial approximation
 */
COMPLEX_1(float, exp10, float, val, {
	/* argument reduction via equivalence: e^ab = (e^a)^b */
	/* more specific: e^(M*2^E) = (e^M)^(2^E) */
	int_t exponent = ilogb(val);
	result_t tmp = ldexp((arg_t) 1.0f, exponent);
	// val = val / tmp;
	// TODO negative exponents!
	// TODO to apply above argument reduction, need efficient power with power of two
	// TODO below algorithm is exact, but only for [0, 1]

	/* polynomial approximation for range [0, 1] */
	result_t approx = 1 + 1.15129277603f * val + 0.66273088429f * val * val + 0.25439357484f * val * val * val +
		0.07295173666f * val * val * val * val + 0.01742111988f * val * val * val * val * val +
		0.00255491796f * val * val * val * val * val * val + 0.00093264267f * val * val * val * val * val * val * val;

	/*XXX re-apply exponent 2^E, approx^2 is part of the approximation */
	return approx * approx;
})

/**
 * Expected behavior:
 *
 * expm1(+-0) = +-0
 * expm1(-Inf) = -1
 * expm1(+Inf) = +Inf
 */
// e^x - 1.0
SIMPLE_1(float, expm1, float, x, exp(x) - 1.0f)

/**
 * Expected behavior:
 *
 * fabs(+-0) = +0
 * fabs(+-Inf) = +Inf
 */
SIMPLE_1(float, fabs, float, val, vc4cl_fmaxabs(val, 0.0f))

/**
 * Expected behavior:
 *
 * fdim(x, NaN) = NaN
 * fdim(NaN, y) = NaN
 */
//"x-y if x>y, +0 if x is less than or equal to y."
COMPLEX_2(float, fdim, float, x, float, y, {
	result_t result = x > y ? x - y : 0.0f;
	result = isnan(x) ? x : result;
	return isnan(y) ? y : result;
})

/**
 * Expected behavior:
 *
 * floor(+-0) = +-0
 * floor(+-Inf) = +-Inf
 */
COMPLEX_1(float, floor, float, val, {
	//" Round to integer toward negative infinity. "

	// https://stackoverflow.com/questions/12279914/implement-ceil-in-c
	// http://blog.frama-c.com/index.php?post/2013/05/02/nearbyintf1
	// all floating point values |val| > 2^23 are exact numbers
	int_t tooBig = isnan(val) || fabs(val) >= 0x1.0p23f;
	// |val| < 2^23 fits into integer
	result_t truncated = vc4cl_itof(vc4cl_ftoi(val));
	// if the truncation is greater than val (negative numbers), subtract 1 to round down
	result_t floor_val = truncated - (truncated > val ? (result_t)1.0f : (result_t)0.0f);
	// deciding here which value to return saves us up to two jumps
	return tooBig ? val : floor_val;
})

/**
 * Expected behavior:
 *
 * fma(x, y, z) = xy + z
 * fma(x, y, z) = NaN for x = 0 or y = 0
 * fma(x, y, z)= NaN for x = Inf, y = Inf, z = Inf
 */
// TODO not "infinitely precise product" and maybe not "correctly rounded"
SIMPLE_3(float, fma, float, a, float, b, float, c, (a * b) + c)

// The fmax/min ALU operations (vc4cl_fmax/vc4cl_fmin) handle NaNs as bigger then all, we need to handle them as:
// " If one argument is a NaN, fmin/fmax() returns the other argument. If both arguments are NaNs, fmin/fmax() returns a NaN."
SIMPLE_2(float, fmax, float, x, float, y, isnan(x) ? y : isnan(y) ? x : vc4cl_fmax(x, y))
SIMPLE_2_SCALAR(float, fmax, float, x, float, y, isnan(x) ? y : isnan(y) ? x : vc4cl_fmax(x, y))

SIMPLE_2(float, fmin, float, x, float, y, isnan(x) ? y : isnan(y) ? x : vc4cl_fmin(x, y))
SIMPLE_2_SCALAR(float, fmin, float, x, float, y, isnan(x) ? y : isnan(y) ? x : vc4cl_fmin(x, y))

/**
 * Expected behavior:
 *
 * fmod(+-0, y) = +-0 for y != 0
 * fmod(x, y) = NaN for x = +-Inf or y = 0
 * fmod(x, +-Inf) = x
 * fmod(+-0, NaN) = NaN
 */
//"Modulus. Returns x - y * trunc(x/y)"
COMPLEX_2(float, fmod, float, x, float, y, {
	result_t result = x - y * trunc(x / y);
	result = (isinf(x) || sign(y) == (result_t)0.0f) ? (result_t)nan(0) : result;
	result = (isinf(y) || isnan(x)) ? x : result;
	return isnan(y) ? y : result;
})

/**
 * Expected behavior:
 *
 * 0 <= fract(x, iptr) < 1
 * fract(+0, iptr) = +0, iptr = +0
 * fract(-0, iptr) = -0, iptr = -0
 * fract(+Inf, iptr) = +0, iptr = +Inf
 * fract(-Inf, iptr) = -0, iptr = -Inf
 * fract(NaN, iptr) = NaN, iptr = NaN
 */
COMPLEX_2(float, fract, float, val, __global float, *iptr, {
	//"Returns fmin( x – floor (x), 0x1.fffffep-1f ). floor(x) is returned in iptr."
	result_t tmp = floor(val);
	*iptr = tmp;
	tmp = fmin(val - tmp, 0x1.fffffep-1f);
	return isnan(val) ? val : tmp;
})

COMPLEX_2(float, fract, float, val, __local float, *iptr, {
	//"Returns fmin( x – floor (x), 0x1.fffffep-1f ). floor(x) is returned in iptr."
	result_t tmp = floor(val);
	*iptr = tmp;
	tmp = fmin(val - tmp, 0x1.fffffep-1f);
	return isnan(val) ? val : tmp;
})

COMPLEX_2(float, fract, float, val, __private float, *iptr, {
	//"Returns fmin( x – floor (x), 0x1.fffffep-1f ). floor(x) is returned in iptr."
	result_t tmp = floor(val);
	*iptr = tmp;
	tmp = fmin(val - tmp, 0x1.fffffep-1f);
	return isnan(val) ? val : tmp;
})

/**
 * Expected behavior:
 *
 * frexp(+-0, exp) = +-0, exp = 0
 * frexp(+-Inf, exp) = +-Inf, exp = 0
 * frexp(NaN, exp) = NaN, exp = 0
 */
COMPLEX_2(float, frexp, float, x, __global int, *exp, {
	// Adapted from https://git.musl-libc.org/cgit/musl/tree/src/math/frexpf.c
	int_t absBits = vc4cl_bitcast_int(x) & (int_t)0x7FFFFFFF;
	int_t exponent = ((absBits >> 23) & 0xFF) - 126;
	// we need to support subnormals here, since they are only used as input, not output
	int_t mantissa = absBits & (int_t) 0x007FFFFF;
	int_t subnormalExpOffset = (vc4cl_clz(mantissa) - (32 - 23));
	// the next lines are required to fix-up the mantissa for our added subnormal exponent
	int_t subnormalExpOffsetLog2 = 33 - vc4cl_clz(subnormalExpOffset);
	mantissa = mantissa << (exponent == -126 ? subnormalExpOffsetLog2 : 0);
	exponent = exponent == -126 ? -subnormalExpOffset  - 126 : exponent;
	/* mask off exponent and replace with exponent for range [0.5, 1) */
	int_t tmp = mantissa | (int_t)0x3F000000;
	int_t specialCase = vc4cl_is_zero(x) || vc4cl_is_inf_nan(x);
	*exp = specialCase ? (int_t)0 : exponent;
	return specialCase ? x : copysign(vc4cl_bitcast_float(tmp), x);
})

COMPLEX_2(float, frexp, float, x, __local int, *exp, {
	// Adapted from https://git.musl-libc.org/cgit/musl/tree/src/math/frexpf.c
	int_t absBits = vc4cl_bitcast_int(x) & (int_t)0x7FFFFFFF;
	int_t exponent = ((absBits >> 23) & 0xFF) - 126;
	// we need to support subnormals here, since they are only used as input, not output
	int_t mantissa = absBits & (int_t) 0x007FFFFF;
	int_t subnormalExpOffset = (vc4cl_clz(mantissa) - (32 - 23));
	// the next lines are required to fix-up the mantissa for our added subnormal exponent
	int_t subnormalExpOffsetLog2 = 33 - vc4cl_clz(subnormalExpOffset);
	mantissa = mantissa << (exponent == -126 ? subnormalExpOffsetLog2 : 0);
	exponent = exponent == -126 ? -subnormalExpOffset  - 126 : exponent;
	/* mask off exponent and replace with exponent for range [0.5, 1) */
	int_t tmp = mantissa | (int_t)0x3F000000;
	int_t specialCase = vc4cl_is_zero(x) || vc4cl_is_inf_nan(x);
	*exp = specialCase ? (int_t)0 : exponent;
	return specialCase ? x : copysign(vc4cl_bitcast_float(tmp), x);
})

COMPLEX_2(float, frexp, float, x, __private int, *exp, {
	// Adapted from https://git.musl-libc.org/cgit/musl/tree/src/math/frexpf.c
	int_t absBits = vc4cl_bitcast_int(x) & (int_t)0x7FFFFFFF;
	int_t exponent = ((absBits >> 23) & 0xFF) - 126;
	// we need to support subnormals here, since they are only used as input, not output
	int_t mantissa = absBits & (int_t) 0x007FFFFF;
	int_t subnormalExpOffset = (vc4cl_clz(mantissa) - (32 - 23));
	// the next lines are required to fix-up the mantissa for our added subnormal exponent
	int_t subnormalExpOffsetLog2 = 33 - vc4cl_clz(subnormalExpOffset);
	mantissa = mantissa << (exponent == -126 ? subnormalExpOffsetLog2 : 0);
	exponent = exponent == -126 ? -subnormalExpOffset  - 126 : exponent;
	/* mask off exponent and replace with exponent for range [0.5, 1) */
	int_t tmp = mantissa | (int_t)0x3F000000;
	int_t specialCase = vc4cl_is_zero(x) || vc4cl_is_inf_nan(x);
	*exp = specialCase ? (int_t)0 : exponent;
	return specialCase ? x : copysign(vc4cl_bitcast_float(tmp), x);
})

/**
 * Expected behavior:
 *
 * hypot(x, y) = hypot(y, x) = hypot(x, -y)
 * hypot(x, +-0) = fabs(x)
 * hypot(+-Inf, y) = +Inf
 */
SIMPLE_2(float, hypot, float, x, float, y, sqrt(x *x + y * y))

/**
 * Expected behavior (C99 standard):
 *
 * ilogb(0) = FP_ILOGB0
 * ilogb(+-Inf) = INT_MAX
 * ilogb(+-NaN) = FP_ILOGBNAN
 */
COMPLEX_1(int, ilogb, float, x, {
	//"Return the exponent as an integer value."
	// https://en.wikipedia.org/wiki/Single-precision_floating-point_format
	result_t bitcast = vc4cl_bitcast_int(x);
	// deduct exponent offset
	int_t result = ((bitcast >> 23) & 0xFF) - 127;
	// we need to support subnormals here, since they are only used as input, not output
	int_t mantissa = bitcast & (int_t) 0x007FFFFF;
	result = result == -127 ? -(vc4cl_clz(mantissa) - (32 - 23)) - 127 : result;
	result = vc4cl_is_zero(x) ? (result_t)FP_ILOGB0 : result;
	result = isinf(x) ? (result_t)INT_MAX : result;
	return isnan(x) ? (result_t)FP_ILOGBNAN : result;
})

/**
 * Expected behavior:
 *
 * ldexp(+-0, n) = +-0
 * ldexp(x, 0) = x
 * ldexp(+-Inf, n) = +-Inf
 */
//"Multiply x by 2 to the power k."
COMPLEX_2(float, ldexp, float, x, int, k, {
	// Taken from MESA (https://gitlab.freedesktop.org/mesa/mesa/-/blob/master/src/compiler/nir/nir_opt_algebraic.py)
	// Alternatively in https://git.musl-libc.org/cgit/musl/tree/src/math/scalbnf.c
	int_t exp = min(max(k, -254), 254);
	int_t pow2_1 = ((exp >> 1) + 127) << 23;
	int_t pow2_2 = ((exp - (exp >> 1)) + 127) << 23;
	result_t result = x * vc4cl_bitcast_float(pow2_1) * vc4cl_bitcast_float(pow2_2);
	return vc4cl_is_inf_nan(x) ? x : result;
})
SIMPLE_2_SCALAR(float, ldexp, float, x, int, k, x *(k >= 0 ? vc4cl_itof(1 << k) : 1.0f / vc4cl_itof(1 << -k)))

/**
 * Expected behavior:
 *
 * lgamma(1) = 0
 * lgamma(2) = 0
 * lgamma(x) = +Inf for x <= 0 and x integer
 * lgamma(-Inf) = +Inf
 * lgamma(+Inf) = +Inf
 */
COMPLEX_1(float, lgamma, float, val, {
	//"Returns the natural logarithm of the absolute value of the gamma function"
	// see Numerical Recipes, chapter 6.1
	// has error of < 2 * 10^-10

	// XXX only for val > 0
	// TODO far too inaccurate/wrong?
	result_t x;
	result_t y;
	result_t tmp;
	result_t ser;

	y = x = val;
	tmp = x + 5.5f;
	tmp -= (x + 0.5f) * log(tmp);
	ser = 1.000000000190015f;
	y += 1;
	ser += 76.18009172947146f / y;
	y += 1;
	ser += -86.50532032941677f / y;
	y += 1;
	ser += 24.01409824083091f / y;
	y += 1;
	ser += -1.231739572450155f / y;
	y += 1;
	ser += 0.1208650973866179e-2f / y;
	y += 1;
	ser += -0.5395239384953e-5f / y;
	return -tmp + log((2.5066282746310005f) * ser / x);
})

/**
 * Expected behavior:
 *
 * lgamma_r(x, signp) -> signp = 0 for x < 0
 */
COMPLEX_2(float, lgamma_r, float, x, __global int, *signp, {
	// TODO better way?
	result_t tmp = lgamma(x);
	*signp = tmp < 0.0f ? -1 : (tmp == 0.0f) ? 0 : 1;
	return lgamma(x < 0.0f ? (-1.0f * x) : x);
})
COMPLEX_2(float, lgamma_r, float, x, __local int, *signp, {
	// TODO better way?
	result_t tmp = lgamma(x);
	*signp = tmp < 0.0f ? -1 : (tmp == 0.0f) ? 0 : 1;
	return lgamma(x < 0.0f ? (-1.0f * x) : x);
})
COMPLEX_2(float, lgamma_r, float, x, __private int, *signp, {
	// TODO better way?
	result_t tmp = lgamma(x);
	*signp = tmp < 0.0f ? -1 : (tmp == 0.0f) ? 0 : 1;
	return lgamma(x < 0.0f ? (-1.0f * x) : x);
})

COMPLEX_1(float, log1p_pade, float, val, {
	/*
	 * Calculates ln(1 + x) via Pade approximation
	 * (https://ir.canterbury.ac.nz/bitstream/handle/10092/8886/brookes_thesis.pdf?sequence=1, page 72/96)
	 *
	 * Has a relative error of less than 2*2^23 (2 ULP) for range [-0.5, 2], then rises fast
	 */
	// g(x) = (49x^6+1218x^5+7980x^4+20720x^3+23100x^2+9240x)/(10x^6+420x^5+4200x^4+16800x^3+31500x^2+27720x+9240)
	arg_t a = 49.0f * val * val * val * val * val * val;
	arg_t b = 1218.0f * val * val * val * val * val;
	arg_t c = 7980.0f * val * val * val * val;
	arg_t d = 20720.0f * val * val * val;
	arg_t e = 23100.0f * val * val;
	arg_t f = 9240.0f * val;
	arg_t g = 10.0f * val * val * val * val * val * val;
	arg_t h = 420.0f * val * val * val * val * val;
	arg_t i = 4200.0f * val * val * val * val;
	arg_t k = 16800.0f * val * val * val;
	arg_t l = 31500.0f * val * val;
	arg_t m = 27720.0f * val;
	arg_t n = 9240.0f;
	arg_t up = a + b + c + d + e + f;
	arg_t down = g + h + i + k + l + m + n;
	return up / down;
})

/**
 * Expected behavior:
 *
 * log2(+-0) = -Inf
 * log2(1) = 0
 * log2(x) = Nan for x < 0
 * log2(+Inf) = +Inf
 */
COMPLEX_1(float, log2, float, val, {

	/* log2(x) = log2(M * e^E) = E + log2(M) */
	int_t bitcast = vc4cl_bitcast_int(val);
	/* deduct exponent offset, we use -126 instead of -127, since we go into the range [0.5, 1), not [1, 2) */
	int_t exponent = ((bitcast >> 23) & 0xFF) - 126;

	/* mask off exponent and replace with exponent for range [0.5, 1) */
	int_t tmp = (bitcast & (int_t)0x807FFFFF) | (int_t)0x3F000000;
	arg_t mantissa = vc4cl_bitcast_float(tmp);

	/*
	 * Alternatively could use SFU and Newton-Raphson steps:
	 * f(x) = log2(x) - y
	 * root: f(x) = 0 -> log2(x) = y
	 * f'(x) = 1/(x * ln(2))
	 * xn+1 = xn - (log2(xn) - y) / (1/(xn * ln(2)))
	 * xn+1 = xn - xn * ln(2) * (log2(x) - y)
	 * => but don't know y which is exact log2(x)!!!
	 *
	 * Alternatively could approximate root of: f(y) = 2^y - x, need exact exp2(x)
	 */
	// result_t logMantissa = vc4cl_sfu_log2(mantissa);

	// log2(M*2^E) = E + log2(M) = E + ln(M) / ln(2) = E + ln1p(M - 1) / ln(2)
	result_t logMantissa = log1p_pade(mantissa - 1.0f) / M_LN2_F;

	result_t result = vc4cl_itof(exponent) + logMantissa;
	result = vc4cl_is_inf_nan(val) ? val : result;
	result = signbit(val) ? (result_t)nan(0) : result;
	result = vc4cl_is_zero(val) ? (result_t)-INFINITY: result;
	return result;
})

/**
 * Expected behavior:
 *
 * log(+-0) = -Inf
 * log(1) = 0
 * log(x) = Nan for x < 0
 * log(+Inf) = +Inf
 */
COMPLEX_1(float, log, float, val, {
	/*
	 * Other sources/calculations:
	 * - Matters computational (sections 32.1.1 and 32.1.3)
	 * - https://math.stackexchange.com/questions/977586/is-there-an-approximation-to-the-natural-log-function-at-large-values
	 * - https://math.stackexchange.com/questions/1382070/iterative-calculation-of-log-x
	 * - https://evoq-eval.siam.org/Portals/0/Publications/SIURO/Vol7/New_Method_for_Approximating_Logarithms.pdf?ver=2018-04-06-151916-493
	 * - https://cs.stackexchange.com/questions/91185/compute-ex-given-starting-approximation
	 * - https://shoueiko.wordpress.com/2017/05/01/approximation-of-log-2/
	 * - https://math.stackexchange.com/questions/3381629/what-is-the-fastest-algorithm-for-finding-the-natural-logarithm-of-a-big-number
	 */

	/* log(M * 2^E) = log(M) + E log(2) */
	int_t bitcast = vc4cl_bitcast_int(val);
	/* deduct exponent offset, we use -126 instead of -127, since we go into the range [0.5, 1), not [1, 2) */
	int_t exponent = ((bitcast >> 23) & 0xFF) - 126;

	/* mask off exponent and replace with exponent for range [0.5, 1) */
	int_t tmp = (bitcast & (int_t)0x807FFFFF) | (int_t)0x3F000000;
	arg_t mantissa = vc4cl_bitcast_float(tmp);

	result_t logMantissa = log1p_pade(mantissa - 1.0f);

	result_t result = vc4cl_itof(exponent) * M_LN2_F + logMantissa;
	result = vc4cl_is_inf_nan(val) ? val : result;
	result = signbit(val) ? (result_t)nan(0) : result;
	result = vc4cl_is_zero(val) ? (result_t)-INFINITY: result;
	return result;
})

/**
 * Expected behavior:
 *
 * log10(+-0) = -Inf
 * log10(1) = 0
 * log10(x) = Nan for x < 0
 * log10(+Inf) = +Inf
 */
COMPLEX_1(float, log10, float, val, {
	/* log10(M * 2^E) = log10(M) + E log10(2) */
	int_t bitcast = vc4cl_bitcast_int(val);
	/* deduct exponent offset, we use -126 instead of -127, since we go into the range [0.5, 1), not [1, 2) */
	int_t exponent = ((bitcast >> 23) & 0xFF) - 126;

	/* mask off exponent and replace with exponent for range [0.5, 1) */
	int_t tmp = (bitcast & (int_t)0x807FFFFF) | (int_t)0x3F000000;
	arg_t mantissa = vc4cl_bitcast_float(tmp);

	/* log10(M) = log(M) / log(10) */
	result_t logMantissa = log1p_pade(mantissa - 1.0f) / M_LN10_F;

	const arg_t log2_10 = M_LN2_F / M_LN10_F;
	result_t result = vc4cl_itof(exponent) * log2_10 + logMantissa;
	result = vc4cl_is_inf_nan(val) ? val : result;
	result = signbit(val) ? (result_t)nan(0) : result;
	result = vc4cl_is_zero(val) ? (result_t)-INFINITY: result;
	return result;
})

/**
 * Expected behavior:
 *
 * log2(+-0) = +-0
 * log2(-1) = -Inf
 * log2(x) = Nan for x < -1
 * log2(+Inf) = +Inf
 */
 //TODO use log1p_pade(), needs argument reduction
SIMPLE_1(float, log1p, float, x, log(1.0f + x))

/**
 * Expected behavior:
 *
 * logb(+-0) = -Inf
 * logb(+-Inf) = +Inf
 */
COMPLEX_1(float, logb, float, x, {
	int_t bitcast = vc4cl_bitcast_int(x);
	// deduct exponent offset
	int_t exponent = ((bitcast >> 23) & 0xFF) - 127;
	// we need to support subnormals here, since they are only used as input, not output
	int_t mantissa = bitcast & (int_t) 0x007FFFFF;
	exponent = exponent == -127 ? -(vc4cl_clz(mantissa) - (32 - 23)) - 127 : exponent;
	result_t result = vc4cl_itof(exponent);
	result = vc4cl_is_zero(x) ? (result_t)-INFINITY : result;
	result = isinf(x) ? fabs(x) : result;
	return isnan(x) ? x : result;
})

SIMPLE_3(float, mad, float, a, float, b, float, c, (a * b) + c)

//"Returns x if |x|>|y|, y if |y|>|x|, otherwise fmax(x, y)"
COMPLEX_2(float, maxmag, float, x, float, y, {
	// explicitly calculate both variants to prevent clang from converting the ?:-operator to an if-else block
	result_t tmp = fmax(x, y);
	result_t other = fabs(y) > fabs(x) ? y : tmp;
	return fabs(x) > fabs(y) ? x : other;
})
//"Returns x if |x|<|y|, y if |y|<|x|, otherwise fmin(x, y)"
COMPLEX_2(float, minmag, float, x, float, y, {
	// explicitly calculate both variants to prevent clang from converting the ?:-operator to an if-else block
	result_t tmp = fmin(x, y);
	result_t other = fabs(y) < fabs(x) ? y : tmp;
	return fabs(x) < fabs(y) ? x : other;
})

/**
 * Expected behavior:
 *
 * modf(+-x, iptr) = value with same sign as x
 * modf(+-Inf, iptr) = +-0, iptr = +-Inf
 * modf(NaN, iptr) = NaN, iptr = NaN
 */
// taken OpenCL C 1.2 specification, 7.5.2. Changes to C99 TC2 Behavior
COMPLEX_2(float, modf, float, value, __global float, *iptr, {
	result_t tmp = trunc(value);
	*iptr = isnan(value) ? value : tmp;
	result_t result = copysign(isinf(value) ? (result_t)0.0f : value - tmp, value);
	return isnan(value) ? value : result;
})
COMPLEX_2(float, modf, float, value, __local float, *iptr, {
	result_t tmp = trunc(value);
	*iptr = isnan(value) ? value : tmp;
	result_t result = copysign(isinf(value) ? (result_t)0.0f : value - tmp, value);
	return isnan(value) ? value : result;
})
COMPLEX_2(float, modf, float, value, __private float, *iptr, {
	result_t tmp = trunc(value);
	*iptr = isnan(value) ? value : tmp;
	result_t result = copysign(isinf(value) ? (result_t)0.0f : value - tmp, value);
	return isnan(value) ? value : result;
})

SIMPLE_1(float, nan, uint, nancode, vc4cl_bitcast_float(NAN | nancode))
/**
 * Expected behavior:
 *
 * nextafter(+smallest normal, y < +smallest normal) = +0
 * nextafter(-smallest normal, y > -smallest normal) = -0
 * nextafter(-0, y > 0) = smallest positive normal value
 * nextafter(0, y < 0) = smallest negative normal value
 */
COMPLEX_2(float, nextafter, float, x, float, y, {
	int_t ix = vc4cl_bitcast_int(x);
	int_t iy = vc4cl_bitcast_int(y);
	/* x > y -> x -= ulp otherwise x += ulp */
	int_t xPos = ix > iy ? ix - 1 : ix + 1;
	/* x < y -> x -= ulp otherwise x += ulp */
	int_t xNeg = iy >= 0 || ix > iy ? ix - 1 : ix + 1;
	int_t xNotY = ix >= 0 ? /* x > 0 */ xPos : /* x < 0 */ xNeg;
	int_t res = x == y ? iy : xNotY;
	result_t result = vc4cl_bitcast_float(res);
	result = (x == (result_t)-0.0f && y > 0.0f) ? (result_t)FLT_MIN : result;
	result = (x == (result_t)0.0f && y < 0.0f) ? (result_t)-FLT_MIN : result;
	result = (x == (result_t)FLT_MIN && y < (result_t)FLT_MIN) ? (result) +0.0f : result;
	result = (x == (result_t)-FLT_MIN && y > (result_t)-FLT_MIN) ? (result) -0.0f : result;
	return isnan(x) ? x : (isnan(y) ? y : result);
})

/**
 * Expected behavior:
 *
 * pow(+-0, y) = +-Inf for y odd and y < 0
 * pow(+-0, y) = +Inf for y even and y < 0
 * pow(+-0, y) = +-0 for y odd and y > 0
 * pow(+-0, y) = 0 for y even and y > 0
 * pow(-1, +-Inf) = 1
 * pow(1, y) = 1
 * pow(x, +-0) = 1
 * pow(x, y) = NaN for x < 0 and y not an integer
 * pow(x, -Inf) = +Inf for |x| < 0
 * pow(x, -Inf) = 0 for |x| > 0
 * pow(x, +Inf) = 0 for |x| < 0
 * pow(x, +Inf) = 0 for |x| > 0
 * pow(-Inf, y) = -0 for y odd and y < 0
 * pow(-Inf, y) = 0 for y even and y < 0
 * pow(-Inf, y) = -Inf for y odd and y > 0
 * pow(-Inf, y) = +Inf for y even and y > 0
 * pow(+Inf, y) = 0 for y < 0
 * pow(+Inf, y) = +Inf for y > 0
 * pow(+-0, -Inf) =+Inf
 */
// for pow, see also https://stackoverflow.com/questions/4518011/algorithm-for-powfloat-float
COMPLEX_2(float, pow, float, x, float, y, {
	result_t tmp = powr(x, y);
	return y < 0.0f ? (result_t) 1.0f / tmp : tmp;
})

/**
 * Expected behavior:
 *
 * pown(x, 0) = 1
 * pown(+-0, n) = +-Inf for odd n and n < 0
 * pown(+-0, n) = +Inf for even n and n < 0
 * pown(+-0, n) = +-0 for odd n and n > 0
 * pown(+-0, n) = 0 for even n and n > 0
 */
 // TODO is there a way to not need to calculate both versions? Or at least calculate both at once?!
SIMPLE_2(float, pown, float, x, int, n,
	n < 0 ? (1.0f / fast_pown(x, vc4cl_bitcast_uint(-n), 32)) : fast_pown(x, vc4cl_bitcast_uint(n), 32))

/**
 * Expected behavior:
 *
 * powr(x, +-0) = 1 for x > 0
 * powr(+-0, y) = +Inf for y < 0
 * powr(+-0, -Inf) = +Inf
 * powr(+-0, y) = +0 for y > 0
 * powr(1, y) = 1
 * powr(x, y) = NaN for x < 0
 * powr(+-0, +-0) = NaN
 * powr(+Inf, +-0) = NaN
 * powr(1, +-Inf) = NaN
 * powr(x, NaN) = NaN for x >= 0
 * powr(NaN, y) = NaN
 */
//"Compute x to the power y, where x is >= 0."
// x^y = e^(y * ln(x))
SIMPLE_2(float, powr, float, x, float, y, exp(y *log(x)))

/**
 * Expected behavior:
 *
 * remainder(x, 0) = NaN or 0
 * remainder(Inf, y) = NaN
 * remainder(x, Inf) = x
 */
COMPLEX_2(float, remainder, float, x, float, y, {
	// TODO correct?
	result_t k = rint(x / y);
	result_t result = x - k * y;

	result = vc4cl_is_zero(y) ? (result_t)-nan(0) : result;
	result = isinf(x) ? (result_t)nan(0) : result;
	result = isnan(x) ? x : result;
	return isnan(y) ? y : result;
})

/**
 * Expected behavior:
 *
 * remquo(x, y, quo) = NaN, quo = 0 for x = +-Inf or y = 0 or x = NaN or y = NaN
 * exp2(-Inf) = 0
 * exp2(+Inf) = +Inf
 */
// taken from pocl (https://github.com/pocl/pocl/blob/master/lib/kernel/vecmathlib-pocl/remquo.cl)
COMPLEX_3(float, remquo, float, x, float, y, __global int, *quo, {
	result_t k = rint(x / y);
	*quo = (vc4cl_ftoi(k) & 0x7F) * (1 - 2 * signbit(k));
	return x - k * y;
})
COMPLEX_3(float, remquo, float, x, float, y, __local int, *quo, {
	result_t k = rint(x / y);
	*quo = (vc4cl_ftoi(k) & 0x7F) * (1 - 2 * signbit(k));
	return x - k * y;
})
COMPLEX_3(float, remquo, float, x, float, y, __private int, *quo, {
	result_t k = rint(x / y);
	*quo = (vc4cl_ftoi(k) & 0x7F) * (1 - 2 * signbit(k));
	return x - k * y;
})

/**
 * Expected behavior:
 *
 * rint(x) = -0 for -0.5 <= x < 0
 */
COMPLEX_1(float, rint, float, val, {
	//" Round to nearest even integer. "
	// round like round, but decides on nearest even for halfway cases
	// TODO always round to nearest even (e.g. 0.4 to 2.0) or just for half-way cases?

	// https://stackoverflow.com/questions/12279914/implement-ceil-in-c
	// http://blog.frama-c.com/index.php?post/2013/05/02/nearbyintf1

	int_t tooBig = isnan(val) || fabs(val) >= 0x1.0p23f;
	// |val| < 2^23 fits into integer
	int_t integer = vc4cl_ftoi(val);
	result_t truncated = vc4cl_itof(integer);
	// for negative numbers, truncated is larger than value, for positive it is smaller
	result_t diff = val - truncated;
	// -> if the |difference| is > 0.5, subtract/add 1
	result_t correction = 0.0f + (diff < -0.5f ? (result_t)-1.0f : (result_t)0.0f) + (diff > 0.5f ? (result_t)1.0f : (result_t)0.0f);
	// if the value is exactly the half, round to nearest even
	int_t isHalfWay = diff == 0.5f || diff == -0.5f;
	// if the truncated integer is even, use it. Otherwise (lsb of integer is 1) subtract/add 1
	result_t halfWayCorrection = 0 + vc4cl_itof(integer & 1) * sign(val);
	result_t result = truncated + correction + vc4cl_itof(isHalfWay) * halfWayCorrection;
	// deciding here which value to return saves us up to two jumps
	return tooBig ? val : result;
})

/**
 * Expected behavior:
 *
 * rootn(+-0, n) = +-Inf for odd n < 0
 * rootn(+-0, n) = +Inf for even n < 0
 * rootn(+-0, n) = +-0 for odd n > 0
 * rootn(+-0, n) = +0 for even n > 0
 * rootn(x, n) = NaN for x < 0 and n even
 * rootn(x, 0) = NaN
 */
// TODO different algorithm?
SIMPLE_2(float, rootn, float, x, int, y, pow(x, (arg0_t) 1.0f / vc4cl_itof(y)))
// see: https://en.wikipedia.org/wiki/Nth_root_algorithm

/**
 * Expected behavior:
 *
 * round(+-0) = +-0
 * round(+-Inf) = +-Inf
 * round(x) = -0 for -0.5 < x < 0
 */
COMPLEX_1(float, round, float, val, {
	//" Return the integral value nearest to x rounding halfway cases away from zero, regardless of the current rounding
	// direction. "

	// https://stackoverflow.com/questions/12279914/implement-ceil-in-c
	// http://blog.frama-c.com/index.php?post/2013/05/02/nearbyintf1
	int_t tooBig = isnan(val) || fabs(val) >= 0x1.0p23f;
	// |val| < 2^23 fits into integer
	result_t truncated = vc4cl_itof(vc4cl_ftoi(val));
	// for negative numbers, truncated is larger than value, for positive it is smaller
	result_t diff = val - truncated;
	// -> if the |difference| is >= 0.5, subtract/add 1
	result_t correction = 0.0f + (diff <= -0.5f ? (result_t)-1.0f : (result_t)0.0f) + (diff >= 0.5f ? (result_t)1.0f : (result_t)0.0f);
	result_t result = truncated + correction;
	// deciding here which value to return saves us up to two jumps
	return tooBig ? val : result;
})

COMPLEX_1(float, rsqrt, float, x, {
	// Using Goldschmidt's algorithm with 2 iterations
	// see http://www.informatik.uni-trier.de/Reports/TR-08-2004/rnc6_12_markstein.pdf (formula 9)
	arg_t y = vc4cl_sfu_rsqrt(x);
	arg_t g = x * y;
	arg_t h = y * (arg_t)0.5f;

	for(unsigned i = 0; i < 2; ++i) {
		arg_t r = (arg_t)0.5f - g * h;
		g = g + g * r;
		h = h + h * r;
	}

	// rsqrt(+-0) = +-Inf
	result_t result = vc4cl_is_zero(x) ? copysign((result_t)INFINITY, x) : (arg_t)2.0f * h;
	// rsqrt(NaN) = NaN
	result = isnan(x) ? x : result;
	// rsqrt(Inf) = 0
	result = isinf(x) ? (result_t)0.0f : result;
	// rsqrt(x) for x < 0 = -NaN
	result = x < (result_t)0.0f ? copysign((result_t)nan(0), -1.0f) : result;
	return result;
})

COMPLEX_1(float, sin_pade, float, val, {
	/*
	 * Pade approximation
	 * (https://en.wikipedia.org/wiki/Pad%C3%A9_approximant#Examples)
	 *
	 * Has a relative error of less than 2^23 (1 ULP) for range [-PI/2, PI/2], then rises fast
	 *
	 * Alternative:
	 * Mendenhall algorithm to calculate both sine and cosine:
	 * - Is also exact enough for range [-PI/2, PI/2]
	 * - https://arxiv.org/pdf/cs/0406049.pdf
	 * -
	 * https://github.com/broadcomCM/android_hardware_broadcom_brcm_usrlib/blob/ics/dag/vmcsx/middleware/khronos/glsl/glsl_mendenhall.c
	 */
	arg_t a = 12671.0f / 4363920.0f;
	arg_t b = 2363.0f / 18183.0f;
	arg_t c = 445.0f / 12122.0f;
	arg_t d = 601.0f / 872784.0f;
	arg_t e = 121.0f / 16662240.0f;

	arg_t up = (a * val * val * val * val * val) - (b * val * val * val) + val;
	arg_t down = 1.0f + (c * val * val) + (d * val * val * val * val) + (e * val * val * val * val * val * val);
	return up / down;
})

COMPLEX_1(float, sin_fast, float, x, {
	/*
	 * Alternative implementation, used by Mesa3D
	 * - less instructions to calculate
	 * - has greater error (even for range -pi/2, pi/2)
	 * https://gitlab.freedesktop.org/mesa/mesa/blob/master/src/compiler/nir/nir_opt_algebraic.py
	 * https://web.archive.org/web/20180105155939/http://forum.devmaster.net/t/fast-and-accurate-sine-cosine/9648
	 */
	const arg_t B = 4.0 / M_PI;
    const arg_t C = -4.0 / (M_PI * M_PI);

    arg_t y = B * x + C * x * fabs(x);

    //  const float Q = 0.775;
    const arg_t P = (arg_t)0.225;
	y = P * (y * fabs(y) - y) + y;   // Q * y + P * y * abs(y)
	return y;
})

//COMPLEX_1(float, sin_taylor, float, x, {
	/*
	 * Alternative implementation, used by Mesa3D VC4 driver
	 * - calculates the Taylor series
	 * - 
	 * https://gitlab.freedesktop.org/mesa/mesa/blob/master/src/gallium/drivers/vc4/vc4_program.c  (function ntq_fsin)
	 */
//})

/**
 * Expected behavior:
 *
 * sin(+-0) = +-0
 * sin(+-Inf) = NaN for |x| > 1
 */
COMPLEX_1(float, sin, float, val, {
	/*
	 * OpenCL 1.2 EMBEDDED PROFILE allows an error of up to 4 ulp
	 *
	 * We use argument reduction to bring it into range [-pi/2, pi/2] in which range the Pade approximation is accurate.
	 */
	// TODO normalization into [-pi/2, pi/2] is too inaccurate for large values! fmod too inaccurate (~8 ULP, should be 0)

	// Since sine has a period of 2pi, these rewrites do not change the result (rounding error excluded):
	// bring into range [-2pi, 2pi]
	arg_t modifiedVal = fmod(val, 2.0f * M_PI_F);
	// bring into range [-pi, pi]
	modifiedVal = modifiedVal < -M_PI_F ? modifiedVal + (2.0f * M_PI_F) : modifiedVal;
	modifiedVal = modifiedVal > M_PI_F ? modifiedVal - (2.0f * M_PI_F) : modifiedVal;

	// We move by half a period, so we must invert the sign of the result
	// bring into range [-pi/2, pi/2]
	int_t invertSign = modifiedVal < -M_PI_2_F || modifiedVal > M_PI_2_F;
	modifiedVal = modifiedVal < -M_PI_2_F ? modifiedVal + M_PI_F : modifiedVal;
	modifiedVal = modifiedVal > M_PI_2_F ? modifiedVal - M_PI_F : modifiedVal;

	arg_t result = sin_pade(modifiedVal);
	return invertSign ? -result : result;
})

SIMPLE_2(float, sincos, float, x, __global float, *cosval, (*cosval = cos(x), sin(x)))
SIMPLE_2(float, sincos, float, x, __local float, *cosval, (*cosval = cos(x), sin(x)))
SIMPLE_2(float, sincos, float, x, __private float, *cosval, (*cosval = cos(x), sin(x)))

/**
 * Expected behavior:
 *
 * sinh(+-0) = +-0
 * sinh(+-Inf) = +-Inf
 */
COMPLEX_1(float, sinh, float, val, {
	/*
	 * https://en.wikipedia.org/wiki/Taylor_series#Approximation_and_convergence
	 *
	 * This version (the Taylor-series up to x^17/17!) has following accuracy:
	 *
	 * sinh(PI) -> error of 2*10^-8 < ulp(PI) = 2*10^-7
	 * sinh(0.5f) -> error of < 10^-10 < ulp(0.5f) = 5*10^-8
	 * sinh(0.0f) -> error of < 10^-48 < ulp(0.0f) = 1*10^-45
	 *
	 * OpenCL 1.2 EMBEDDED PROFILE allows an error of up to 4 ulp
	 */
	// TODO inaccurate out of the range of -PI <= x <= PI
	/*
	 * Alternatives:
	 * sinh(x) = sqrt((cosh(2x)-1)/2)
	 * sinh(x) = exp(x) - cosh(x)
	 */
	const result_t fac3 = 1.0f / factorial(3);
	const result_t fac5 = 1.0f / factorial(5);
	const result_t fac7 = 1.0f / factorial(7);
	const result_t fac9 = 1.0f / factorial(9);
	const result_t fac11 = 1.0f / factorial(11);
	const result_t fac13 = 1.0f / factorial(13);
	const result_t fac15 = 1.0f / factorial(15);
	const result_t fac17 = 1.0f / factorial(17);

	result_t x = val;
	/// TODO optimize!!
	/*
	while(x < -M_PI_F)
	{
	x = x + 2 * M_PI_F;
	}
	while(x > M_PI_F)
	{
	x = x - 2 * M_PI_F;
	}
	 */
	result_t x3 = x * x * x;
	result_t tmp1 = x3 * fac3;
	// sinh(x) = x + x^3/3! + ...
	result_t tmp = x + tmp1;
	result_t x5 = x3 * x * x;
	tmp1 = x5 * fac5;
	//... + x^5/5! + ...
	tmp = x + tmp1;
	result_t x7 = x5 * x * x;
	tmp1 = x7 * fac7;
	//... + x^7/7! + ...
	tmp = tmp + tmp1;
	result_t x9 = x7 * x * x;
	tmp1 = x9 * fac9;
	//... + x^9/9! + ...
	tmp = tmp + tmp1;
	result_t x11 = x9 * x * x;
	tmp1 = x11 * fac11;
	//... + x^11/11! + ...
	tmp = tmp + tmp1;
	result_t x13 = x11 * x * x;
	tmp1 = x13 * fac13;
	//... + x^13/13! + ...
	tmp = tmp + tmp1;
	result_t x15 = x13 * x * x;
	tmp1 = x15 * fac15;
	//... + x^15/15! + ...
	tmp = tmp + tmp1;
	result_t x17 = x15 * x * x;
	tmp1 = x17 * fac17;
	//... + x^17/17!
	tmp = tmp + tmp1;
	return tmp;
})

/**
 * Expected behavior:
 *
 * sinpi(+-0) = +-0
 * sinpi(n) = 0 for n integer and n > 0
 * sinpi(n) = -0 for n integer and n < 0
 * sinpi(+-Inf) = NaN
 */
SIMPLE_1(float, sinpi, float, val, sin(val *M_PI_F))

COMPLEX_1(float, sqrt, float, x, {
	// Using Goldschmidt's algorithm with 2 iterations
	// see http://www.informatik.uni-trier.de/Reports/TR-08-2004/rnc6_12_markstein.pdf (formula 9)
	arg_t y = vc4cl_sfu_rsqrt(x);
	arg_t g = x * y;
	arg_t h = y * (arg_t)0.5f;

	for(unsigned i = 0; i < 2; ++i) {
		arg_t r = (arg_t)0.5f - g * h;
		g = g + g * r;
		h = h + h * r;
	}

	// sqrt(NaN) = NaN, sqrt(Inf) = Inf
	result_t result = vc4cl_is_inf_nan(x) ? x : g;
	// sqrt(+-0) = +-0
	result = vc4cl_is_zero(x) ? x : result;
	// sqrt(x) = NaN, for x < 0
	return x < 0.0f ? (result_t)nan(0) : result;
})

/**
 * Expected behavior:
 *
 * tan(+-0) = +-0
 * tan(+-Inf) = NaN
 */
// TODO different algorithm?!
// Taylor Series is too inaccurate (converges too slow)
SIMPLE_1(float, tan, float, val, sin(val) / cos(val))

/**
 * Expected behavior:
 *
 * tanh(+-0) = +-0
 * tanh(+-Inf) = +-Inf
 */
// TODO different algorithm?!
// Taylor Series is too inaccurate (converges too slow)
SIMPLE_1(float, tanh, float, val, sinh(val) / cosh(val))

/**
 * Expected behavior:
 *
 * tanpi(+-0) = +-0
 * tanpi(+-Inf) = NaN
 * tanpi(n) = copysign(0,n) for n even and n integer
 * tanpi(n) = copysign(0, -n) for n odd and n integer
 * tanpi(x) = +Inf for x = n + 0.5 and n even integer
 * tanpi(x) = -Inf for x = n + 0.5 and n odd integer
 */
SIMPLE_1(float, tanpi, float, val, tan(val *M_PI_F))

/**
 * Expected behavior:
 *
 * tgamma(+-0) = +-Inf
 * tgamma(x) = NaN for x < 0 and x integer
 * tgamma(-Inf) = NaN
 * tgamma(+Inf) = +Inf
 */
// TODO different algorithm?! E.g. http://www.math.hkbu.edu.hk/support/aands/page_257.htm with argument reduction
// (previous page)
SIMPLE_1(float, tgamma, float, val, exp(lgamma(val)))

/**
 * Expected behavior:
 *
 * trunc(+-0) = +-0
 * trunc(+-Inf) = +-Inf
 * trunc(x) = -0 for -1 < x < 0
 */
COMPLEX_1(float, trunc, float, val, {
	//"  Round to integer toward zero.  "
	// https://stackoverflow.com/questions/12279914/implement-ceil-in-c
	// http://blog.frama-c.com/index.php?post/2013/05/02/nearbyintf1
	int_t tooBig = isnan(val) || fabs(val) >= 0x1.0p23f;
	// |val| < 2^23 fits into integer
	result_t truncated = vc4cl_itof(vc4cl_ftoi(val));
	// deciding here which value to return saves us up to two jumps
	return tooBig ? val : truncated;
})

/*
 * native functions
 */
SIMPLE_1(float, native_cos, float, val, cos(val))

SIMPLE_2(float, native_divide, float, x, float, y, x *native_recip(y))

// e^x = 2^(x * log2(e))
SIMPLE_1(float, native_exp, float, x, native_exp2(x *M_LOG2E_F))

SIMPLE_1(float, native_exp2, float, val, vc4cl_sfu_exp2(val))

// 10^x = 2^(x * log2(10))
SIMPLE_1(float, native_exp10, float, x, native_exp2(x *M_LOG210))

// log(x) = log2(x) / log2(e)
SIMPLE_1(float, native_log, float, x, native_log2(x) * (1.0f / M_LOG2E_F))

SIMPLE_1(float, native_log2, float, val, vc4cl_sfu_log2(val))

// log10(x) = log2(x) / log2(10)
SIMPLE_1(float, native_log10, float, x, native_log2(x) * (1.0f / M_LOG210))

//"Compute x to the power y, where x is >= 0."
// x^y = 2^(y * log2(x))
SIMPLE_2(float, native_powr, float, x, float, y, native_exp2(y *native_log2(x)))

SIMPLE_1(float, native_recip, float, val, vc4cl_sfu_recip(val))
SIMPLE_1(float, native_rsqrt, float, val, vc4cl_sfu_rsqrt(val))

SIMPLE_1(float, native_sin, float, val, sin(val))

SIMPLE_1(float, native_sqrt, float, val, val * vc4cl_sfu_rsqrt(val))

SIMPLE_1(float, native_tan, float, val, tan(val))

/*
 * half functions, accuracy of 8192 ULP (11 MSB of the mantissa)
 */
SIMPLE_1(float, half_cos, float, val, cos(val))
SIMPLE_2(float, half_divide, float, x, float, y, x * vc4cl_sfu_recip(y))
SIMPLE_1(float, half_exp, float, val, native_exp(val))
COMPLEX_1(float, half_exp2, float, val, {
	result_t result = vc4cl_sfu_exp2(val);

	result = vc4cl_is_zero(val) ? (result_t)1.0f : result;
	result = val < (result_t)-126.0f ? (result_t)0.0f : result;
	result = val >= (result_t)128.0f ? (result_t)INFINITY : result;
	return isnan(val) ? val : result;
})
SIMPLE_1(float, half_exp10, float, val, native_exp10(val))
SIMPLE_1(float, half_log, float, val, native_log(val))
COMPLEX_1(float, half_log2, float, val, {
	/* log2(x) = log2(M * e^E) = E + log2(M) */
	int_t bitcast = vc4cl_bitcast_int(val);
	/* deduct exponent offset, we use -126 instead of -127, since we go into the range [0.5, 1), not [1, 2) */
	int_t exponent = ((bitcast >> 23) & 0xFF) - 126;

	/* mask off exponent and replace with exponent for range [0.5, 1) */
	int_t tmp = (bitcast & (int_t)0x807FFFFF) | (int_t)0x3F000000;
	arg_t mantissa = vc4cl_bitcast_float(tmp);
	result_t logMantissa = vc4cl_sfu_log2(mantissa);

	result_t result = vc4cl_itof(exponent) + logMantissa;
	result = vc4cl_is_inf_nan(val) ? val : result;
	result = signbit(val) ? (result_t)nan(0) : result;
	result = vc4cl_is_zero(val) ? (result_t)-INFINITY: result;
	return result;
})
SIMPLE_1(float, half_log10, float, val, native_log10(val))
SIMPLE_2(float, half_powr, float, x, float, y, powr(x, y))
SIMPLE_1(float, half_recip, float, val, (arg_t)1.0f / val)
COMPLEX_1(float, half_rsqrt, float, val, {
	arg_t x = vc4cl_sfu_rsqrt(val);

	// rsqrt(+-0) = +-Inf
	result_t result = vc4cl_is_zero(val) ? copysign((result_t)INFINITY, val) : x;
	// rsqrt(NaN) = NaN
	result = isnan(val) ? val : result;
	// rsqrt(Inf) = 0
	result = isinf(val) ? (result_t)0.0f : result;
	// rsqrt(x) for x < 0 = -NaN
	result = val < (result_t)0.0f ? copysign((result_t)nan(0), -1.0f) : result;
	return result;
})
SIMPLE_1(float, half_sin, float, val, sin(val))
COMPLEX_1(float, half_sqrt, float, val, {
	arg_t x = val * vc4cl_sfu_rsqrt(val);

	// sqrt(NaN) = NaN, sqrt(Inf) = Inf
	result_t result = vc4cl_is_inf_nan(val) ? val : x;
	// sqrt(+-0) = +-0
	result = vc4cl_is_zero(val) ? val : result;
	// sqrt(x) = NaN, for x < 0
	return val < 0.0f ? (result_t)nan(0) : result;
})
SIMPLE_1(float, half_tan, float, val, tan(val))

#endif /* VC4CL_MATH_H */
