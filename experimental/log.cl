#define arg_t float16
#define result_t float16
#define int_t int16

/*
 * Helper, arithmetic-geometric-mean,
 *
 * https://en.wikipedia.org/wiki/Arithmetic%E2%80%93geometric_mean
 */
result_t agm(arg_t x, arg_t y)
{
	arg_t arithm = x;
	arg_t geom = y;
	for(unsigned iteration = 0; iteration < 6; ++iteration) // TODO can adjust number of iterations
	{
		arg_t arithm_new = (arithm + geom) / (arg_t) 2.0;
		geom = sqrt(arithm * geom);
		arithm = arithm_new;
	}
	return arithm;
}

#define CONCAT(a, b) a##b
#define CAT(a, b) CONCAT(a, b)

#define REDUCE_ARGUMENT_TO_0_1                                                                                         \
	/* log(S * M * 2^E) = log(S * M) + E log(2) */                                                                     \
	int_t bitcast = CAT(as_, int_t)(val);                                                                              \
	/* deduct exponent offset, we use -126, to go into the range [0.5, 1) */                                           \
	int_t exponent = ((bitcast >> 23) & 0xFF) - 126;                                                                   \
	/* mask off exponent and replace with exponent for range [0.5, 1) */                                               \
	int_t signedMantissaBits = (bitcast & (int_t) 0x807FFFFF) | (int_t) 0x3F000000;                                    \
	arg_t mantissa = CAT(as_, result_t)(signedMantissaBits);                                                           \
	result_t reduced = CAT(convert_, result_t)(exponent) * M_LN2_F;

/*
 * Taylor-series,
 *
 * https://en.wikipedia.org/wiki/Mercator_series
 */
result_t log1p_taylor(arg_t val)
{
	// ln (1 + x) = x - x^2/2 + x^3/3 - x^4/4
	// converges for -1 < x <= 1 (requires argument reduction)

	REDUCE_ARGUMENT_TO_0_1

	// iteration 1
	result_t result = mantissa;
	arg_t power = mantissa;
#pragma loop unroll
	for(unsigned iteration = 2; iteration <= 26; ++iteration) // TODO can adjust number of iterations
	{
		power *= mantissa;
		arg_t sign = iteration & 1 ? (arg_t) 1.0 : (arg_t) -1.0;
		result = result + sign * (arg_t) (1.0 / iteration) * power;
	}
	return result + reduced;
}

__kernel void log1p_taylor_kernel(__global arg_t *out, const __global arg_t *in)
{
	uint gid = get_global_id(0);
	out[gid] = log1p_taylor(in[gid] - (arg_t) 1.0f);
}

result_t log1p_taylor_unrolled(arg_t val)
{
	// ln (1 + x) = x - x^2/2 + x^3/3 - x^4/4
	// converges for -1 < x <= 1 (requires argument reduction)

	REDUCE_ARGUMENT_TO_0_1

	// iteration 1
	result_t result = mantissa;
	arg_t power = mantissa;

	// iteration 2
	power *= mantissa;
	result = result - (arg_t) (1.0 / 2.0) * power;

	// iteration 3
	power *= mantissa;
	result = result + (arg_t) (1.0 / 3.0) * power;

	// iteration 4
	power *= mantissa;
	result = result - (arg_t) (1.0 / 4.0) * power;

	// iteration 5
	power *= mantissa;
	result = result + (arg_t) (1.0 / 5.0) * power;

	// iteration 6
	power *= mantissa;
	result = result - (arg_t) (1.0 / 6.0) * power;

	// iteration 7
	power *= mantissa;
	result = result + (arg_t) (1.0 / 7.0) * power;

	// iteration 8
	power *= mantissa;
	result = result - (arg_t) (1.0 / 8.0) * power;

	// iteration 9
	power *= mantissa;
	result = result + (arg_t) (1.0 / 9.0) * power;

	// iteration 10
	power *= mantissa;
	result = result - (arg_t) (1.0 / 10.0) * power;

	// iteration 11
	power *= mantissa;
	result = result + (arg_t) (1.0 / 11.0) * power;

	// iteration 12
	power *= mantissa;
	result = result - (arg_t) (1.0 / 12.0) * power;

	// iteration 13
	power *= mantissa;
	result = result + (arg_t) (1.0 / 13.0) * power;

	// iteration 14
	power *= mantissa;
	result = result - (arg_t) (1.0 / 14.0) * power;

	// iteration 15
	power *= mantissa;
	result = result + (arg_t) (1.0 / 15.0) * power;

	// iteration 16
	power *= mantissa;
	result = result - (arg_t) (1.0 / 16.0) * power;

	// iteration 17
	power *= mantissa;
	result = result + (arg_t) (1.0 / 17.0) * power;

	// iteration 18
	power *= mantissa;
	result = result - (arg_t) (1.0 / 18.0) * power;

	// iteration 19
	power *= mantissa;
	result = result + (arg_t) (1.0 / 19.0) * power;

	// iteration 20
	power *= mantissa;
	result = result - (arg_t) (1.0 / 20.0) * power;

	// iteration 21
	power *= mantissa;
	result = result + (arg_t) (1.0 / 21.0) * power;

	// iteration 22
	power *= mantissa;
	result = result - (arg_t) (1.0 / 22.0) * power;

	// iteration 23
	power *= mantissa;
	result = result + (arg_t) (1.0 / 23.0) * power;

	// iteration 24
	power *= mantissa;
	result = result - (arg_t) (1.0 / 24.0) * power;

	// iteration 25
	power *= mantissa;
	result = result + (arg_t) (1.0 / 25.0) * power;

	// iteration 26
	power *= mantissa;
	result = result - (arg_t) (1.0 / 26.0) * power;

	// TODO can adjust number of iterations

	return result + reduced;
}

__kernel void log1p_taylor_unrolled_kernel(__global arg_t *out, const __global arg_t *in)
{
	uint gid = get_global_id(0);
	out[gid] = log1p_taylor_unrolled(in[gid] - (arg_t) 1.0f);
}

/*
 * Taylor series with optimization, requires argument reduction,
 *
 * https://math.stackexchange.com/a/3383716
 */
result_t log_taylor(arg_t val)
{
	REDUCE_ARGUMENT_TO_0_1

	result_t result = 0;
	// iteration 1
	arg_t tmp = 2 * (mantissa - (arg_t) 1.0) / (mantissa + (arg_t) 1.0);
	arg_t factor = tmp * tmp;
#pragma loop unroll
	for(unsigned iteration = 1; iteration <= 26; iteration += 2) // TODO can adjust number of iterations
	{
		result += tmp / (arg_t) iteration;
		tmp *= factor;
	}
	return result + reduced;
}

__kernel void log_taylor_kernel(__global arg_t *out, const __global arg_t *in)
{
	uint gid = get_global_id(0);
	out[gid] = log_taylor(in[gid]);
}

/*
 * Using the arithmetic-geometric-mean,
 *
 * https://en.wikipedia.org/wiki/Natural_logarithm#High_precision
 */
result_t log_agm(arg_t val)
{
	const unsigned m = 8; // TODO can adjust for precision
	arg_t s = val * (arg_t) (1 << m);
	arg_t mean = agm(1.0, (arg_t) 4.0 / s);
	return (val * M_PI_F) / (2 * mean) - (arg_t) (m * M_LN2);
}

__kernel void log_agm_kernel(__global arg_t *out, const __global arg_t *in)
{
	uint gid = get_global_id(0);
	out[gid] = log_agm(in[gid]);
}

result_t log_agm_reduced(arg_t val)
{
	REDUCE_ARGUMENT_TO_0_1

	const unsigned m = 8; // TODO can adjust for precision
	arg_t s = mantissa * (arg_t) (1 << m);
	arg_t mean = agm(1.0, (arg_t) 4.0 / s);
	return (mantissa * M_PI_F) / (2 * mean) - (arg_t) (m * M_LN2) + reduced;
}

__kernel void log_agm_reduced_kernel(__global arg_t *out, const __global arg_t *in)
{
	uint gid = get_global_id(0);
	out[gid] = log_agm_reduced(in[gid]);
}

__kernel void log_builtin_kernel(__global arg_t *out, const __global arg_t *in)
{
	uint gid = get_global_id(0);
	out[gid] = log(in[gid]);
}
