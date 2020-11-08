/*
 * Author: doe300
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef VC4CL_RELATIONAL_H
#define VC4CL_RELATIONAL_H

#include "_config.h"
#include "_overloads.h"
#include "_intrinsics.h"

#ifndef COMPARISON_1
#define COMPARISON_1(func, content) \
	INLINE FUNC_1(int##16, func, float##16, val) CONST \
	{ \
		return (content) ? -1 : 0; \
	} \
	INLINE FUNC_1(int##8, func, float##8, val) CONST \
	{ \
		return (content) ? -1 : 0; \
	} \
	INLINE FUNC_1(int##4, func, float##4, val) CONST \
	{ \
		return (content) ? -1 : 0; \
	} \
	INLINE FUNC_1(int##3, func, float##3, val) CONST \
	{ \
		return (content) ? -1 : 0; \
	} \
	INLINE FUNC_1(int##2, func, float##2, val) CONST \
	{ \
		return (content) ? -1 : 0; \
	} \
	INLINE FUNC_1(int, func, float, val) CONST \
	{ /* 1 instead of -1 here on purpose! */ \
		return (content) ? 1 : 0; \
	}
#endif

#ifndef COMPARISON_2
#define COMPARISON_2(func, content) \
	INLINE FUNC_2(int##16, func, float##16, x, float##16, y) CONST \
	{ \
		return (content) ? -1 : 0; \
	} \
	INLINE FUNC_2(int##8, func, float##8, x, float##8, y) CONST \
	{ \
		return (content) ? -1 : 0; \
	} \
	INLINE FUNC_2(int##4, func, float##4, x, float##4, y) CONST \
	{ \
		return (content) ? -1 : 0; \
	} \
	INLINE FUNC_2(int##3, func, float##3, x, float##3, y) CONST \
	{ \
		return (content) ? -1 : 0; \
	} \
	INLINE FUNC_2(int##2, func, float##2, x, float##2, y) CONST \
	{ \
		return (content) ? -1 : 0; \
	} \
	INLINE FUNC_2(int, func, float, x, float, y) CONST \
	{ /* 1 instead of -1 here on purpose! */ \
		return (content) ? 1 : 0; \
	}
#endif

#ifndef FOR_ALL_ELEMENTS
#define FOR_ALL_ELEMENTS(func, type, op, conv) \
	INLINE FUNC_1(int, func, type##16, x) CONST \
	{ \
		/* (s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, sa, sb, sc, sd, se, sf) */ \
		type##16 val0 = conv(x); \
		/* (s0 op s1, s1 op s2, s2 op s3, s3 op s4, s4 op s5, s5 op s6, s6 op s7, s7 op s8, s8 op s9, s9 op sa, sa op sb, sb op sc, sc op sd, sd op se, se op sf, sf op s0) */ \
		val0 = val0 op vc4cl_vector_rotate(val0, -1); \
		/* (s0 op s1 op s2 op s3, s1 op s2 op s3 op s4, s2 op s3 op s4 op s5, s3 op s4 op s5 op s6, s4 op s5 op s6 op s7, s5 op s6 op s7 op s8, s6 op s7 op s8 op s9, s7 op s8 op s9 op sa, s8 op s9 op sa op sb, s9 op sa op sb op sc, sa op sb op sc op sd, sb op sc op sd op se, sc op sd op se op sf, ...) */ \
		const type##16 val1 = val0 op vc4cl_vector_rotate(val0, -2); \
		/* (s0 op s1 op s2 op s3 op s4 op s5 op s6 op s7, ..., s8 op s9 op sa op ab op sc op sd op se op sf, ...) */ \
		const type##16 val2 = val1 op vc4cl_vector_rotate(val1, -4); \
		/* s0 op s1 op s2 op s3 op s4 op s5 op s6 op s7 op s8 op s9 op sa op ab op sc op sd op se op sf */ \
		return (val2 op val1 op vc4cl_vector_rotate(val2, -8)).x != 0; \
	} \
	INLINE FUNC_1(int, func, type##8, x) CONST \
	{ \
		/* (s0, s1, s2, s3, s4, s5, s6, s7) */ \
		type##8 val0 = conv(x); \
		/* (s0 op s1, s1 op s2, s2 op s3, s3 op s4, s4 op s5, s5 op s6, s6 op s7, s7 op ?) */ \
		val0 = val0 op vc4cl_vector_rotate(val0, -1); \
		/* (s0 op s1 op s2 op s3, s1 op s2 op s3 op s4, s2 op s3 op s4 op s5, s3 op s4 op s5 op s6, s4 op s5 op s6 op s7, s5 op s6 op s7 op ?, s6 op s7 op ? op ?, s7 op ? op ? op ?) */ \
		const type##8 val1 = val0 op vc4cl_vector_rotate(val0, -2); \
		/* s0 op s1 op s2 op s3 op s4 op s5 op s6 op s7 */ \
		return (val1 op vc4cl_vector_rotate(val1, -4)).x != 0; \
	} \
	INLINE FUNC_1(int, func, type##4, x) CONST \
	{ \
		/* (x, y, z, w) */ \
		type##4 val0 = conv(x); \
		/* (x op y, y op z, z op w, w op ?) */ \
		val0 = val0 op vc4cl_vector_rotate(val0, -1); \
		/* (z op w, w op ?, ? op ?, ? op ?) */ \
		const type##4 val1 = vc4cl_vector_rotate(val0, -2); \
		/* (x op y op z op w, ...) */ \
		return (val0 op val1).x != 0; \
	} \
	INLINE FUNC_1(int, func, type##3, x) CONST \
	{ \
		type##3 val = conv(x); \
		return (val.x op val.y op val.z) != 0; \
	} \
	INLINE FUNC_1(int, func, type##2, x) CONST \
	{ \
		type##2 val = conv(x); \
		return (val.x op val.y) != 0; \
	} \
	INLINE FUNC_1(int, func, type, x) CONST \
	{ \
		type val = conv(x); \
		return val != 0; \
	}
#endif

#ifndef SELECT_SCALAR
#define SELECT_SCALAR(type, maskType, content) \
	INLINE FUNC_3(type, select, type, a, type, b, maskType, c) CONST \
	{ \
		return content; \
	}
#endif

#ifndef SELECT_VECTOR
#define SELECT_VECTOR(type, maskType, content) \
	INLINE FUNC_3(type##2, select, type##2, a, type##2, b, maskType##2, c) CONST \
	{ \
		typedef int##2 int_t; \
		content \
	} \
	INLINE FUNC_3(type##3, select, type##3, a, type##3, b, maskType##3, c) CONST \
	{ \
		typedef int##3 int_t; \
		content \
	} \
	INLINE FUNC_3(type##4, select, type##4, a, type##4, b, maskType##4, c) CONST \
	{ \
		typedef int##4 int_t; \
		content \
	} \
	INLINE FUNC_3(type##8, select, type##8, a, type##8, b, maskType##8, c) CONST \
	{ \
		typedef int##8 int_t; \
		content \
	} \
	INLINE FUNC_3(type##16, select, type##16, a, type##16, b, maskType##16, c) CONST \
	{ \
		typedef int##16 int_t; \
		content \
	}
#endif

/*
 * The checks for NaNs as defined in the specification are done in the intrinsic of the comparison operators:
 *
 * "The relational functions isequal, isgreater, isgreaterequal, isless, islessequal, and islessgreater
 * always return 0 if either argument is not a number (NaN). isnotequal returns 1 if one or both
 * arguments are not a number (NaN) and the argument type is a scalar [...]"
 * - OpenCL 1.2, section 6.12.6 Relational Functions
 */
COMPARISON_2(isequal, x == y)
COMPARISON_2(isnotequal, x != y)
COMPARISON_2(isgreater, x > y)
COMPARISON_2(isgreaterequal, x >= y)
COMPARISON_2(isless, x < y)
COMPARISON_2(islessequal, x <= y)
COMPARISON_2(islessgreater, (x < y) || (x > y))

// From <cmath>: "A finite value is any floating-point value that is neither infinite nor NaN (Not-A-Number)."
COMPARISON_1(isfinite, !vc4cl_is_inf_nan(val))
COMPARISON_1(isinf, (vc4cl_bitcast_uint(val) & NAN) == INF)
COMPARISON_1(isnan, vc4cl_is_nan(val))
// From <cmath>: "Returns whether x is a normal value: i.e., whether it is neither infinity, NaN, zero or subnormal."
COMPARISON_1(isnormal, !isinf(val) && !isnan(val) && ((vc4cl_bitcast_uint(val) & 0x7F800000) != 0) /* neither zero nor denormal */)
COMPARISON_2(isordered, isequal(x, x) && isequal(y, y))
COMPARISON_2(isunordered, isnan(x) || isnan(y))

// for vector,directly use asr, for scalar shr. This is way more efficient than everything else (1 instruction)
INLINE FUNC_1(int16, signbit, float16, val) CONST
{
       return vc4cl_asr(vc4cl_bitcast_uint(val), 31);
}
INLINE FUNC_1(int8, signbit, float8, val) CONST
{
       return vc4cl_asr(vc4cl_bitcast_uint(val), 31);
}
INLINE FUNC_1(int4, signbit, float4, val) CONST
{
       return vc4cl_asr(vc4cl_bitcast_uint(val), 31);
}
INLINE FUNC_1(int3, signbit, float3, val) CONST
{
       return vc4cl_asr(vc4cl_bitcast_uint(val), 31);
}
INLINE FUNC_1(int2, signbit, float2, val) CONST
{
       return vc4cl_asr(vc4cl_bitcast_uint(val), 31);
}
INLINE FUNC_1(int, signbit, float, val) CONST
{
       return vc4cl_bitcast_uint(val) >> 31;
}

FOR_ALL_ELEMENTS(any, char, |, vc4cl_msb_set)
FOR_ALL_ELEMENTS(any, short, |, vc4cl_msb_set)
FOR_ALL_ELEMENTS(any, int, |, vc4cl_msb_set)
FOR_ALL_ELEMENTS(any, long, |, vc4cl_msb_set)

FOR_ALL_ELEMENTS(all, char, &, vc4cl_msb_set)
FOR_ALL_ELEMENTS(all, short, &, vc4cl_msb_set)
FOR_ALL_ELEMENTS(all, int, &, vc4cl_msb_set)
FOR_ALL_ELEMENTS(all, long, &, vc4cl_msb_set)


//"Each bit of the result is the corresponding bit of a if the corresponding bit of c is 0.
// Otherwise it is the corresponding bit of b."
//based on pocl (pocl/lib/kernel/bitselect.cl)
SIMPLE_3(uchar, bitselect, uchar, a, uchar, b, uchar, c, (~c & a) | (c & b))
SIMPLE_3(char, bitselect, char, a, char, b, char, c, (~c & a) | (c & b))
SIMPLE_3(ushort, bitselect, ushort, a, ushort, b, ushort, c, (~c & a) | (c & b))
SIMPLE_3(short, bitselect, short, a, short, b, short, c, (~c & a) | (c & b))
SIMPLE_3(uint, bitselect, uint, a, uint, b, uint, c, (~c & a) | (c & b))
SIMPLE_3(int, bitselect, int, a, int, b, int, c, (~c & a) | (c & b))
SIMPLE_3(ulong, bitselect, ulong, a, ulong, b, ulong, c, (~c & a) | (c & b))
SIMPLE_3(long, bitselect, long, a, long, b, long, c, (~c & a) | (c & b))
SIMPLE_3(float, bitselect, float, a, float, b, float, c, vc4cl_bitcast_float((~vc4cl_bitcast_uint(c) & vc4cl_bitcast_uint(a)) | (vc4cl_bitcast_uint(c) & vc4cl_bitcast_uint(b))))

//"For a scalar type, result = c ? b : a."
SELECT_SCALAR(uchar, uchar, vc4cl_extend(c) ? b : a)
SELECT_SCALAR(uchar, char, vc4cl_extend(c) ? b : a)
SELECT_SCALAR(char, uchar, vc4cl_extend(c) ? b : a)
SELECT_SCALAR(char, char, vc4cl_extend(c) ? b : a)
SELECT_SCALAR(ushort, ushort, vc4cl_extend(c) ? b : a)
SELECT_SCALAR(ushort, short, vc4cl_extend(c) ? b : a)
SELECT_SCALAR(short, ushort, vc4cl_extend(c) ? b : a)
SELECT_SCALAR(short, short, vc4cl_extend(c) ? b : a)
SELECT_SCALAR(uint, uint, vc4cl_extend(c) ? b : a)
SELECT_SCALAR(uint, int, vc4cl_extend(c) ? b : a)
SELECT_SCALAR(int, uint, vc4cl_extend(c) ? b : a)
SELECT_SCALAR(int, int, vc4cl_extend(c) ? b : a)
SELECT_SCALAR(ulong, ulong, vc4cl_extend(c) ? b : a)
SELECT_SCALAR(ulong, long, vc4cl_extend(c) ? b : a)
SELECT_SCALAR(long, ulong, vc4cl_extend(c) ? b : a)
SELECT_SCALAR(long, long, vc4cl_extend(c) ? b : a)
SELECT_SCALAR(float, uint, vc4cl_extend(c) ? b : a)
SELECT_SCALAR(float, int, vc4cl_extend(c) ? b : a)

//"For each component of a vector type, result[i] = if MSB of c[i] is set ? b[i] : a[i]"
SELECT_VECTOR(uchar, uchar,
{
	int_t mask = vc4cl_asr(vc4cl_extend(c) << 24, 31);
	return vc4cl_bitcast_uchar(mask & vc4cl_bitcast_int(vc4cl_extend(b)) | (~mask & vc4cl_bitcast_int(vc4cl_extend(a))));
})
SELECT_VECTOR(uchar, char,
{
	int_t mask = vc4cl_asr(vc4cl_extend(c) << 24, 31);
	return vc4cl_bitcast_uchar(mask & vc4cl_bitcast_int(vc4cl_extend(b)) | (~mask & vc4cl_bitcast_int(vc4cl_extend(a))));
})
SELECT_VECTOR(char, char,
{
	return vc4cl_msb_set(c) ? b : a;
})
SELECT_VECTOR(char, uchar,
{
	return vc4cl_msb_set(c) ? b : a;
})
SELECT_VECTOR(ushort, ushort,
{
	int_t mask = vc4cl_asr(vc4cl_extend(c) << 16, 31);
	return vc4cl_bitcast_ushort(mask & vc4cl_bitcast_int(vc4cl_extend(b)) | (~mask & vc4cl_bitcast_int(vc4cl_extend(a))));
})
SELECT_VECTOR(ushort, short,
{
	int_t mask = vc4cl_asr(vc4cl_extend(c) << 16, 31);
	return vc4cl_bitcast_ushort(mask & vc4cl_bitcast_int(vc4cl_extend(b)) | (~mask & vc4cl_bitcast_int(vc4cl_extend(a))));
})
SELECT_VECTOR(short, short,
{
	return vc4cl_msb_set(c) ? b : a;
})
SELECT_VECTOR(short, ushort,
{
	return vc4cl_msb_set(c) ? b : a;
})
SELECT_VECTOR(uint, uint,
{
	return vc4cl_msb_set(c) ? b : a;
})
SELECT_VECTOR(uint, int,
{
	return vc4cl_msb_set(c) ? b : a;
})
SELECT_VECTOR(int, int,
{
	return vc4cl_msb_set(c) ? b : a;
})
SELECT_VECTOR(int, uint,
{
	return vc4cl_msb_set(c) ? b : a;
})
SELECT_VECTOR(ulong, ulong,
{
	return vc4cl_msb_set(c) ? b : a;
})
SELECT_VECTOR(ulong, long,
{
	return vc4cl_msb_set(c) ? b : a;
})
SELECT_VECTOR(long, long,
{
	return vc4cl_msb_set(c) ? b : a;
})
SELECT_VECTOR(long, ulong,
{
	return vc4cl_msb_set(c) ? b : a;
})
SELECT_VECTOR(float, uint,
{
	return vc4cl_msb_set(c) ? b : a;
})
SELECT_VECTOR(float, int,
{
	return vc4cl_msb_set(c) ? b : a;
})

#undef COMPARISON_1
#undef COMPARISON_2
#undef FOR_ALL_ELEMENTS
#undef SELECT_SCALAR
#undef SELECT_VECTOR

#endif /* VC4CL_RELATIONAL_H */

