
#define arg_t float16
#define result_t float16
#define int_t int16

#define CONCAT(a, b) a##b
#define CAT(a, b) CONCAT(a, b)

// vc4cl_split(double) of M_LN2
#define M_LN2_FF 0xB102E3083F317218

float16 vc4cl_lossy(ulong16) __attribute__((overloadable));
ulong16 vc4cl_add(ulong16, ulong16) __attribute__((overloadable));
ulong16 vc4cl_sub(ulong16, ulong16) __attribute__((overloadable));
ulong16 vc4cl_mul(float16, float16) __attribute__((overloadable));
ulong16 vc4cl_mul(ulong16, ulong16) __attribute__((overloadable));
ulong16 vc4cl_extend(float16 val) __attribute__((overloadable));

result_t pow2(int_t val)
{
	// y = 2^x = 1.0 [implied] * 2^(x + offset)
	int_t tmp = val << 23;
	// alternative: tmp = (val + 127) << 23;
	tmp += (int_t) 0x3F800000;
	return CAT(as_, result_t)(tmp & (int_t) 0x7F800000);
}

int_t powerOfTwo(arg_t val)
{
	// Original code, produces Inf for e^(~10^38)
	// return CAT(convert_, int_t)(ceil((val / M_LN2_F) - 0.5f));
	// Using floor() instead of ceil(),
	// - fixes Inf for large exponents
	// - slightly reduces accuracy of Chebyshev implementations (by ~4 ULP),
	// - greatly reduces accuracy of Taylor (<10 ULP -> >1200 ULP) -> requires more iterations
	return CAT(convert_, int_t)(floor((val / M_LN2_F) - 0.5f));
}

/*
 * Taylor series with Horner's method and range reduction,
 *
 * https://www.pseudorandom.com/implementing-exp#section-6
 */
result_t exp_taylor(arg_t val)
{
	arg_t positive = fabs(val);

	// range reduction: e^x = 2^k * e^r [with x = r + k * log2(x)]
	int_t k = powerOfTwo(positive);
	arg_t r = positive - CAT(convert_, arg_t)(k) * M_LN2_F;

	arg_t tk = 1.0f;
	arg_t tn = 1.0f;
#pragma loop unroll
	for(int i = 1; i < 10; i++) // TODO can adjust number of iterations
	{
		tk *= r / i;
		tn += tk;
	};

	tn = tn * pow2(k);
	return val < 0 ? 1 / tn : tn;
}

__kernel void exp_taylor_kernel(__global arg_t *out, const __global arg_t *in)
{
	uint gid = get_global_id(0);
	out[gid] = exp_taylor(in[gid]);
}

result_t exp_taylor_extended_precision_exact(arg_t val)
{
	arg_t positive = fabs(val);

	// range reduction: e^x = 2^k * e^r [with x = r + k * log2(x)]
	int_t k = powerOfTwo(positive);
	ulong16 r = vc4cl_sub(vc4cl_extend(positive), vc4cl_mul(vc4cl_extend(CAT(convert_, arg_t)(k)), M_LN2_FF));

	ulong16 tk = 0x000000003F800000; // 1.0
	ulong16 tn = 0x000000003F800000; // 1.0

	tk = vc4cl_mul(tk, r);
	tn = vc4cl_add(tn, tk);

	tk = vc4cl_mul(tk, vc4cl_mul(r, 0x000000003F000000)); // 1 / 2
	tn = vc4cl_add(tn, tk);

	tk = vc4cl_mul(tk, vc4cl_mul(r, 0xB22AAAAB3EAAAAAB)); // 1 / 3
	tn = vc4cl_add(tn, tk);

	tk = vc4cl_mul(tk, vc4cl_mul(r, 0x000000003E800000)); // 1 / 4
	tn = vc4cl_add(tn, tk);

	tk = vc4cl_mul(tk, vc4cl_mul(r, 0xB14CCCCD3E4CCCCD)); // 1 / 5
	tn = vc4cl_add(tn, tk);

	tk = vc4cl_mul(tk, vc4cl_mul(r, 0xB1AAAAAB3E2AAAAB)); // 1 / 6
	tn = vc4cl_add(tn, tk);

	tk = vc4cl_mul(tk, vc4cl_mul(r, 0xB1DB6DB73E124925)); // 1 / 7
	tn = vc4cl_add(tn, tk);

	tk = vc4cl_mul(tk, vc4cl_mul(r, 0x000000003E000000)); // 1 / 8
	tn = vc4cl_add(tn, tk);

	tk = vc4cl_mul(tk, vc4cl_mul(r, 0xB0638E393DE38E39)); // 1 / 9
	tn = vc4cl_add(tn, tk);
	// removing any iteration makes the result inaccurate (removing last iteration gives 19 ULP)

	result_t result = vc4cl_lossy(tn) * pow2(k);
	return val < 0 ? 1.0f / result : result;
}

// __kernel void exp_taylor_extended_precision_exact_kernel(__global arg_t *out, const __global arg_t *in)
// {
// 	uint gid = get_global_id(0);
// 	out[gid] = exp_taylor_extended_precision_exact(in[gid]);
// }

// TODO Lagrange and Barycentric interpolations from https://www.pseudorandom.com/implementing-exp

/*
 * Chebyshev interpolation with range reduction,
 *
 * https://www.pseudorandom.com/implementing-exp#section-18
 */
result_t exp_chebyshev(arg_t val)
{
	// XXX could remove unneeded coefficients once we fix precision
	const float coefficients[] = {
		1.266065877752008335598244625214717537923,
		1.130318207984970054415392055219726613610,
		0.2714953395340765623657051399899818507081,
		0.04433684984866380495257149525979922986386,
		0.00547424044209373265027616843118645948703,
		0.000542926311913943750362147810307554678760,
		0.00004497732295429514665469032811091269841937,
		3.198436462401990505863872976602295688795e-6,
		1.992124806672795725961064384805589035648e-7,
		1.103677172551734432616996091335324170860e-8,
		5.50589607967374725047142040200552692791e-10,
		2.497956616984982522712010934218766985311e-11,
		1.039152230678570050499634672423840849837e-12,
		3.991263356414401512887720401532162026594e-14,
	};
	arg_t positive = fabs(val);

	// range reduction: e^x = 2^k * e^r [with x = r + k * log2(x)]
	int_t k = powerOfTwo(positive);
	arg_t r = positive - CAT(convert_, arg_t)(k) * M_LN2_F;

	arg_t ti = 1.0f;
	arg_t tj = r;
	arg_t p = coefficients[0] + (coefficients[1] * r);
#pragma loop unroll
	for(int i = 2; i < 8; i++) // TODO can adjust number of iterations
	{
		arg_t tk = (2 * r * tj) - ti;
		p += coefficients[i] * tk;
		ti = tj;
		tj = tk;
	}

	p = p * pow2(k);
	return val < 0 ? 1 / p : p;
}

__kernel void exp_chebyshev_kernel(__global arg_t *out, const __global arg_t *in)
{
	uint gid = get_global_id(0);
	out[gid] = exp_chebyshev(in[gid]);
}

result_t exp_chebyshev_extended_precision_exact(arg_t val)
{
	arg_t positive = fabs(val);

	// range reduction: e^x = 2^k * e^r [with x = r + k * log2(x)]
	int_t k = powerOfTwo(positive);
	ulong16 r = vc4cl_sub(vc4cl_extend(positive), vc4cl_mul(vc4cl_extend(CAT(convert_, arg_t)(k)), M_LN2_FF));

	ulong16 ti = 0x000000003F800000; // 1.0
	ulong16 tj = r;
	// 1.266065877752008335598244625214717537923 and 1.130318207984970054415392055219726613610
	ulong16 p = vc4cl_add(0x333386C33FA20E72, vc4cl_mul(0x33395E683F90AE44, r));
	r = vc4cl_mul(r, 0x0000000040000000); // 2.0

	ulong16 tk = vc4cl_sub(vc4cl_mul(r, tj), ti);
	p = vc4cl_add(p, vc4cl_mul(0xB13AF4A23E8B0170, tk)); // 0.2714953395340765623657051399899818507081
	ti = tj;
	tj = tk;

	tk = vc4cl_sub(vc4cl_mul(r, tj), ti);
	p = vc4cl_add(p, vc4cl_mul(0xB0FC8DF03D359A8F, tk)); // 0.04433684984866380495257149525979922986386
	ti = tj;
	tj = tk;

	tk = vc4cl_sub(vc4cl_mul(r, tj), ti);
	p = vc4cl_add(p, vc4cl_mul(0xAEA95A453BB36142, tk)); // 0.00547424044209373265027616843118645948703
	ti = tj;
	tj = tk;

	tk = vc4cl_sub(vc4cl_mul(r, tj), ti);
	p = vc4cl_add(p, vc4cl_mul(0x2B7994663A0E532B, tk)); // 0.000542926311913943750362147810307554678760
	ti = tj;
	tj = tk;

	tk = vc4cl_sub(vc4cl_mul(r, tj), ti);
	p = vc4cl_add(p, vc4cl_mul(0x2BC988B0383CA608, tk)); // 0.00004497732295429514665469032811091269841937
	ti = tj;
	tj = tk;

	tk = vc4cl_sub(vc4cl_mul(r, tj), ti);
	p = vc4cl_add(p, vc4cl_mul(0x29A61EF43656A4B8, tk)); // 3.198436462401990505863872976602295688795e-6
	ti = tj;
	tj = tk;

	tk = vc4cl_sub(vc4cl_mul(r, tj), ti);
	p = vc4cl_add(p, vc4cl_mul(0x26B66C3C3455E71C, tk)); // 1.992124806672795725961064384805589035648e-7
	ti = tj;
	tj = tk;
	// removing any iteration makes the result inaccurate (removing last iteration gives 5 ULP)

	result_t result = vc4cl_lossy(p) * pow2(k);
	return val < 0 ? 1.0f / result : result;
}

// __kernel void exp_chebyshev_extended_precision_exact_kernel(__global arg_t *out, const __global arg_t *in)
// {
// 	uint gid = get_global_id(0);
// 	out[gid] = exp_chebyshev_extended_precision_exact(in[gid]);
// }

/*
 * Chebyshev interpolation with monomial basis and range reduction,
 *
 * https://www.pseudorandom.com/implementing-exp#section-18
 */
result_t exp_chebyshev_monomial(arg_t val)
{
	// XXX could remove unneeded coefficients once we fix precision
	// TODO invert order of coefficients and traversal ?!
	const float coefficients[] = {
		1.000000000000000,
		1.000000000000000,
		0.500000000000002,
		0.166666666666680,
		0.041666666666727,
		0.008333333333342,
		0.001388888888388,
		1.984126978734782e-4,
		2.480158866546844e-5,
		2.755734045527853e-6,
		2.755715675968011e-7,
		2.504861486483735e-8,
		2.088459690899721e-9,
		1.632461784798319e-10,
	};
	arg_t positive = fabs(val);

	// range reduction: e^x = 2^k * e^r [with x = r + k * log2(x)]
	int_t k = powerOfTwo(positive);
	arg_t r = positive - CAT(convert_, arg_t)(k) * M_LN2_F;

	arg_t pn = 1.143364767943110e-11;
#pragma loop unroll
	for(int i = 0; i < 14; i++)
	{
		pn = pn * r + coefficients[13 - i];
	}

	pn = pn * pow2(k);
	return val < 0 ? 1 / pn : pn;
}

__kernel void exp_chebyshev_monomial_kernel(__global arg_t *out, const __global arg_t *in)
{
	uint gid = get_global_id(0);
	out[gid] = exp_chebyshev_monomial(in[gid]);
}

result_t exp_chebyshev_monomial_exact(arg_t val)
{
	arg_t positive = fabs(val);

	// range reduction: e^x = 2^k * e^r [with x = r + k * log2(x)]
	int_t k = powerOfTwo(positive);
	arg_t kFloat = CAT(convert_, arg_t)(k);
	arg_t r = vc4cl_lossy(vc4cl_sub(vc4cl_extend(positive), vc4cl_mul(vc4cl_extend(kFloat), M_LN2_FF)));

	arg_t pn = 1.143364767943110e-11;

	pn = pn * r + 1.632461784798319e-10f;
	pn = pn * r + 2.088459690899721e-9f;
	pn = pn * r + 2.504861486483735e-8f;
	pn = pn * r + 2.755715675968011e-7f;
	pn = pn * r + 2.755734045527853e-6f;
	pn = pn * r + 2.480158866546844e-5f;
	pn = pn * r + 1.984126978734782e-4f;
	pn = pn * r + 0.001388888888388f;
	pn = pn * r + 0.008333333333342f;
	pn = pn * r + 0.041666666666727f;
	pn = pn * r + 0.166666666666680f;
	pn = pn * r + 0.500000000000002f;
	pn = pn * r + 1.000000000000000f;
	pn = pn * r + 1.000000000000000f;

	pn = pn * pow2(k);
	return val < 0 ? 1 / pn : pn;
}

__kernel void exp_chebyshev_monomial_exact_kernel(__global arg_t *out, const __global arg_t *in)
{
	uint gid = get_global_id(0);
	out[gid] = exp_chebyshev_monomial_exact(in[gid]);
}

result_t exp_chebyshev_monomial_extended_precision_exact(arg_t val)
{
	arg_t positive = fabs(val);

	// range reduction: e^x = 2^k * e^r [with x = r + k * log2(x)]
	int_t k = powerOfTwo(positive);
	ulong16 r = vc4cl_sub(vc4cl_extend(positive), vc4cl_mul(vc4cl_extend(CAT(convert_, arg_t)(k)), M_LN2_FF));

	ulong16 pn = 0x209249252D492492;					  // 1.143364767943110e-11
	pn = vc4cl_add(vc4cl_mul(pn, r), 0xA21249252F337DB7); // 1.632461784798319e-10
	pn = vc4cl_add(vc4cl_mul(pn, r), 0x24924925310F8492); // 2.088459690899721e-9
	pn = vc4cl_add(vc4cl_mul(pn, r), 0xA65B6DB732D72A7D); // 2.504861486483735e-8
	pn = vc4cl_add(vc4cl_mul(pn, r), 0xA85B6DB73493F245); // 2.755715675968011e-7
	pn = vc4cl_add(vc4cl_mul(pn, r), 0xA9FDB6DB3638EF27); // 2.755734045527853e-6
	pn = vc4cl_add(vc4cl_mul(pn, r), 0xAB60000037D00D02); // 2.480158866546844e-5
	pn = vc4cl_add(vc4cl_mul(pn, r), 0xAC65BDB739500D01); // 1.984126978734782e-4
	pn = vc4cl_add(vc4cl_mul(pn, r), 0xAE161D323AB60B61); // 0.001388888888388
	pn = vc4cl_add(vc4cl_mul(pn, r), 0xAFEEEDB73C088889); // 0.008333333333342
	pn = vc4cl_add(vc4cl_mul(pn, r), 0xB0AAA88B3D2AAAAB); // 0.041666666666727
	pn = vc4cl_add(vc4cl_mul(pn, r), 0xB1AAAA8D3E2AAAAB); // 0.166666666666680
	pn = vc4cl_add(vc4cl_mul(pn, r), 0x271000003F000000); // 0.500000000000002
	pn = vc4cl_add(vc4cl_mul(pn, r), 0x000000003F800000); // 1.000000000000000
	pn = vc4cl_add(vc4cl_mul(pn, r), 0x000000003F800000); // 1.000000000000000

	result_t result = vc4cl_lossy(pn) * pow2(k);
	return val < 0 ? 1.0f / result : result;
}

// __kernel void exp_chebyshev_monomial_extended_precision_exact_kernel(__global arg_t *out, const __global arg_t *in)
// {
// 	uint gid = get_global_id(0);
// 	out[gid] = exp_chebyshev_monomial_extended_precision_exact(in[gid]);
// }

// TODO Remes from www.netlib.org/fdlibm/e_exp.c

// TODO Matters computational (sections 32.2.2.2 and 32.2.3)
// Pade Approximation (16 steps): (1680 + 840x + 180 x^2 + 20 x^3 + x^4) / (1680 - 840 x + 180 x^2 - 20 x^3 + x^4)

// TODO https://math.stackexchange.com/questions/1988901/approximating-the-exponential-function-with-taylor-series?rq=1
// TODO http://www.netlib.org/fdlibm/

__kernel void exp_builtin_kernel(__global arg_t *out, const __global arg_t *in)
{
	uint gid = get_global_id(0);
	out[gid] = exp(in[gid]);
}
