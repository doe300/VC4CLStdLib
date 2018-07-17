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

#define COMPARISON_1(func, content) \
	INLINE FUNC_1(int##16, func, float##16, val) CONST \
	{ \
		return (content) ? vc4cl_bitcast_int(ALL_BITS_SET) : 0; \
	} \
	INLINE FUNC_1(int##8, func, float##8, val) CONST \
	{ \
		return (content) ? vc4cl_bitcast_int(ALL_BITS_SET) : 0; \
	} \
	INLINE FUNC_1(int##4, func, float##4, val) CONST \
	{ \
		return (content) ? vc4cl_bitcast_int(ALL_BITS_SET) : 0; \
	} \
	INLINE FUNC_1(int##3, func, float##3, val) CONST \
	{ \
		return (content) ? vc4cl_bitcast_int(ALL_BITS_SET) : 0; \
	} \
	INLINE FUNC_1(int##2, func, float##2, val) CONST \
	{ \
		return (content) ? vc4cl_bitcast_int(ALL_BITS_SET) : 0; \
	} \
	INLINE FUNC_1(int, func, float, val) CONST \
	{ \
		return content; \
	}

#define COMPARISON_2(func, content) \
	INLINE FUNC_2(int##16, func, float##16, x, float##16, y) CONST \
	{ \
		return (content) ? vc4cl_bitcast_int(ALL_BITS_SET) : 0; \
	} \
	INLINE FUNC_2(int##8, func, float##8, x, float##8, y) CONST \
	{ \
		return (content) ? vc4cl_bitcast_int(ALL_BITS_SET) : 0; \
	} \
	INLINE FUNC_2(int##4, func, float##4, x, float##4, y) CONST \
	{ \
		return (content) ? vc4cl_bitcast_int(ALL_BITS_SET) : 0; \
	} \
	INLINE FUNC_2(int##3, func, float##3, x, float##3, y) CONST \
	{ \
		return (content) ? vc4cl_bitcast_int(ALL_BITS_SET) : 0; \
	} \
	INLINE FUNC_2(int##2, func, float##2, x, float##2, y) CONST \
	{ \
		return (content) ? vc4cl_bitcast_int(ALL_BITS_SET) : 0; \
	} \
	INLINE FUNC_2(int, func, float, x, float, y) CONST \
	{ \
		return content; \
	}

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

COMPARISON_2(isequal, x == y)
COMPARISON_2(isnotequal, x != y)
COMPARISON_2(isgreater, x > y)
COMPARISON_2(isgreaterequal, x >= y)
COMPARISON_2(isless, x < y)
COMPARISON_2(islessequal, x <= y)
COMPARISON_2(islessgreater, (x < y) || (x > y))

COMPARISON_1(isfinite, (vc4cl_bitcast_uint(val) & NAN) != INF)
COMPARISON_1(isinf, (vc4cl_bitcast_uint(val) & NAN) == INF)
COMPARISON_1(isnan, (vc4cl_bitcast_uint(val) & NAN) == NAN)
COMPARISON_1(isnormal, !isinf(val) && !isnan(val) && (((vc4cl_bitcast_uint(val) & 0x7F800000) != 0) /* neither zero nor denormal */ || ((vc4cl_bitcast_uint(val) & 0x7FFFFFFF) == 0) /* +/- zero */))

COMPARISON_2(isordered, isequal(x, x) && isequal(y, y))
COMPARISON_2(isunordered, isnan(x) || isnan(y))

COMPARISON_1(signbit, vc4cl_bitcast_int(vc4cl_bitcast_uint(val) & 0x80000000U))

FOR_ALL_ELEMENTS(any, uchar, |, vc4cl_msb_set)
FOR_ALL_ELEMENTS(any, char, |, vc4cl_msb_set)
FOR_ALL_ELEMENTS(any, ushort, |, vc4cl_msb_set)
FOR_ALL_ELEMENTS(any, short, |, vc4cl_msb_set)
FOR_ALL_ELEMENTS(any, uint, |, vc4cl_msb_set)
FOR_ALL_ELEMENTS(any, int, |, vc4cl_msb_set)

//TODO all(int4) is wrong, the rotation parameter are zeroinitializers?? The bit-cast is somehow optimized away
FOR_ALL_ELEMENTS(all, uchar, &, vc4cl_msb_set)
FOR_ALL_ELEMENTS(all, char, &, vc4cl_msb_set)
FOR_ALL_ELEMENTS(all, ushort, &, vc4cl_msb_set)
FOR_ALL_ELEMENTS(all, short, &, vc4cl_msb_set)
FOR_ALL_ELEMENTS(all, uint, &, vc4cl_msb_set)
FOR_ALL_ELEMENTS(all, int, &, vc4cl_msb_set)


//"Each bit of the result is the corresponding bit of a if the corresponding bit of c is 0.
// Otherwise it is the corresponding bit of b."
//based on pocl (pocl/lib/kernel/bitselect.cl)
SIMPLE_3(uchar, bitselect, uchar, a, uchar, b, uchar, c, (~c & a) | (c & b))
SIMPLE_3(char, bitselect, char, a, char, b, char, c, (~c & a) | (c & b))
SIMPLE_3(ushort, bitselect, ushort, a, ushort, b, ushort, c, (~c & a) | (c & b))
SIMPLE_3(short, bitselect, short, a, short, b, short, c, (~c & a) | (c & b))
SIMPLE_3(uint, bitselect, uint, a, uint, b, uint, c, (~c & a) | (c & b))
SIMPLE_3(int, bitselect, int, a, int, b, int, c, (~c & a) | (c & b))
SIMPLE_3(float, bitselect, float, a, float, b, float, c, vc4cl_bitcast_float((~vc4cl_bitcast_uint(c) & vc4cl_bitcast_uint(a)) | (vc4cl_bitcast_uint(c) & vc4cl_bitcast_uint(b))))

//"For each component of a vector type, result[i] = if MSB of c[i] is set ? b[i] : a[i]"
//"For a scalar type, result = c ? b : a."
//TODO "For a scalar type, result = c ? b : a." So no check for MSB!
COMPLEX_3(uchar, select, uchar, a, uchar, b, uchar, c,
{
	int_t mask = vc4cl_asr(vc4cl_extend(c) << 24, 32);
	return vc4cl_bitcast_uchar(mask & vc4cl_bitcast_int(vc4cl_extend(b)) | (~mask & vc4cl_bitcast_int(vc4cl_extend(a))));
})
COMPLEX_3(uchar, select, uchar, a, uchar, b, char, c,
{
	int_t mask = vc4cl_asr(vc4cl_extend(c) << 24, 32);
	return vc4cl_bitcast_uchar(mask & vc4cl_bitcast_int(vc4cl_extend(b)) | (~mask & vc4cl_bitcast_int(vc4cl_extend(a))));
})
SIMPLE_3(char, select, char, a, char, b, uchar, c, vc4cl_msb_set(c) ? b : a)
SIMPLE_3(char, select, char, a, char, b, char, c, vc4cl_msb_set(c) ? b : a)
COMPLEX_3(ushort, select, ushort, a, ushort, b, ushort, c,
{
	int_t mask = vc4cl_asr(vc4cl_extend(c) << 16, 32);
	return vc4cl_bitcast_ushort(mask & vc4cl_bitcast_int(vc4cl_extend(b)) | (~mask & vc4cl_bitcast_int(vc4cl_extend(a))));
})
COMPLEX_3(ushort, select, ushort, a, ushort, b, short, c,
{
	int_t mask = vc4cl_asr(vc4cl_extend(c) << 16, 32);
	return vc4cl_bitcast_ushort(mask & vc4cl_bitcast_int(vc4cl_extend(b)) | (~mask & vc4cl_bitcast_int(vc4cl_extend(a))));
})
SIMPLE_3(short, select, short, a, short, b, ushort, c, vc4cl_msb_set(c) ? b : a)
SIMPLE_3(short, select, short, a, short, b, short, c, vc4cl_msb_set(c) ? b : a)
SIMPLE_3(uint, select, uint, a, uint, b, uint, c, vc4cl_msb_set(c) ? b : a)
SIMPLE_3(uint, select, uint, a, uint, b, int, c, vc4cl_msb_set(c) ? b : a)
SIMPLE_3(int, select, int, a, int, b, uint, c, vc4cl_msb_set(c) ? b : a)
SIMPLE_3(int, select, int, a, int, b, int, c, vc4cl_msb_set(c) ? b : a)
SIMPLE_3(float, select, float, a, float, b, uint, c, vc4cl_msb_set(c) ? b : a)
SIMPLE_3(float, select, float, a, float, b, int, c, vc4cl_msb_set(c) ? b : a)

#undef COMPARISON_1
#undef COMPARISON_2
#undef FOR_ALL_ELEMENTS

#endif /* VC4CL_RELATIONAL_H */

