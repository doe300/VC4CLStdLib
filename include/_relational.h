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

    COMPARISON_2(isequal, x == y)
    COMPARISON_2(isnotequal, x != y)
    COMPARISON_2(isgreater, x > y)
    COMPARISON_2(isgreaterequal, x >= y)
    COMPARISON_2(isless, x < y)
    COMPARISON_2(islessequal, x <= y)
    COMPARISON_2(islessgreater, x < y || y > x)

    COMPARISON_1(isfinite, (vc4cl_bitcast_uint(val) & NAN) != INF)
    COMPARISON_1(isinf, (vc4cl_bitcast_uint(val) & NAN) == INF)
    COMPARISON_1(isnan, (vc4cl_bitcast_uint(val) & NAN) == NAN)
    COMPARISON_1(isnormal, !isinf(val) && !isnan(val) && (((vc4cl_bitcast_uint(val) & 0x7F800000) != 0) /* neither zero nor denormal */ || ((vc4cl_bitcast_uint(val) & 0x7FFFFFFF) == 0) /* +/- zero */))

    COMPARISON_2(isordered, isequal(x, x) && isequal(y, y))
    COMPARISON_2(isunordered, isnan(x) || isnan(y))

    COMPARISON_1(signbit, vc4cl_bitcast_int(vc4cl_bitcast_uint(val) & 0x80000000U))

	COMPLEX_1_RETURN_SCALAR(int, any, uchar, val,
	{
		if(vc4cl_any_non_zero(vc4cl_msb_set(val)))
			return 1;
		else
			return 0;
	})

	COMPLEX_1_RETURN_SCALAR(int, any, char, val,
	{
		if(vc4cl_any_non_zero(vc4cl_msb_set(val)))
			return 1;
		else
			return 0;
	})

	COMPLEX_1_RETURN_SCALAR(int, any, ushort, val,
	{
		if(vc4cl_any_non_zero(vc4cl_msb_set(val)))
			return 1;
		else
			return 0;
	})

	COMPLEX_1_RETURN_SCALAR(int, any, short, val,
	{
		if(vc4cl_any_non_zero(vc4cl_msb_set(val)))
			return 1;
		else
			return 0;
	})

	COMPLEX_1_RETURN_SCALAR(int, any, uint, val,
	{
		if(vc4cl_any_non_zero(vc4cl_msb_set(val)))
			return 1;
		else
			return 0;
	})

	COMPLEX_1_RETURN_SCALAR(int, any, int, val,
	{
		if(vc4cl_any_non_zero(vc4cl_msb_set(val)))
			return 1;
		else
			return 0;
	})

	COMPLEX_1_RETURN_SCALAR(int, all, uchar, val,
	{
		if(vc4cl_all_non_zero(vc4cl_msb_set(val)))
			return 1;
		else
			return 0;
	})

	COMPLEX_1_RETURN_SCALAR(int, all, char, val,
	{
		if(vc4cl_all_non_zero(vc4cl_msb_set(val)))
			return 1;
		else
			return 0;
	})

	COMPLEX_1_RETURN_SCALAR(int, all, ushort, val,
	{
		if(vc4cl_all_non_zero(vc4cl_msb_set(val)))
			return 1;
		else
			return 0;
	})

	COMPLEX_1_RETURN_SCALAR(int, all, short, val,
	{
		if(vc4cl_all_non_zero(vc4cl_msb_set(val)))
			return 1;
		else
			return 0;
	})

	COMPLEX_1_RETURN_SCALAR(int, all, uint, val,
	{
		if(vc4cl_all_non_zero(vc4cl_msb_set(val)))
			return 1;
		else
			return 0;
	})

	COMPLEX_1_RETURN_SCALAR(int, all, int, val,
	{
		if(vc4cl_all_non_zero(vc4cl_msb_set(val)))
			return 1;
		else
			return 0;
	})

	/*
    INLINE int any(char val) OVERLOADABLE CONST
    {
        //tests for MSB set
        return vc4cl_msb_set(val);
    }

    INLINE int any(char2 val) OVERLOADABLE CONST
    {
        return any(val.x) || any(val.y);
    }

    INLINE int any(char3 val) OVERLOADABLE CONST
    {
        return any(val.x) || any(val.y) || any(val.z);
    }

    INLINE int any(char4 val) OVERLOADABLE CONST
    {
        return any(val.x) || any(val.y) || any(val.z) || any(val.w);
    }

    INLINE int any(char8 val) OVERLOADABLE CONST
    {
        return any(val.s0) || any(val.s1) || any(val.s2) || any(val.s3) || any(val.s4) || any(val.s5) || any(val.s6) || any(val.s7);
    }

    INLINE int any(char16 val) OVERLOADABLE CONST
    {
        return any(val.s0) || any(val.s1) || any(val.s2) || any(val.s3) || any(val.s4) || any(val.s5) || any(val.s6) || any(val.s7) || any(val.s8) || any(val.s9) || any(val.sa) || any(val.sb) || any(val.sc) || any(val.sd) || any(val.se) || any(val.sf);
    }

    INLINE int any(short val) OVERLOADABLE CONST
    {
        //tests for MSB set
        return vc4cl_msb_set(val);
    }

    INLINE int any(short2 val) OVERLOADABLE CONST
    {
        return any(val.x) || any(val.y);
    }

    INLINE int any(short3 val) OVERLOADABLE CONST
    {
        return any(val.x) || any(val.y) || any(val.z);
    }

    INLINE int any(short4 val) OVERLOADABLE CONST
    {
        return any(val.x) || any(val.y) || any(val.z) || any(val.w);
    }

    INLINE int any(short8 val) OVERLOADABLE CONST
    {
        return any(val.s0) || any(val.s1) || any(val.s2) || any(val.s3) || any(val.s4) || any(val.s5) || any(val.s6) || any(val.s7);
    }

    INLINE int any(short16 val) OVERLOADABLE CONST
    {
        return any(val.s0) || any(val.s1) || any(val.s2) || any(val.s3) || any(val.s4) || any(val.s5) || any(val.s6) || any(val.s7) || any(val.s8) || any(val.s9) || any(val.sa) || any(val.sb) || any(val.sc) || any(val.sd) || any(val.se) || any(val.sf);
    }

    INLINE int any(int val) OVERLOADABLE CONST
    {
        //tests for MSB set
        return vc4cl_msb_set(val);
    }

    INLINE int any(int2 val) OVERLOADABLE CONST
    {
        return any(val.x) || any(val.y);
    }

    INLINE int any(int3 val) OVERLOADABLE CONST
    {
        return any(val.x) || any(val.y) || any(val.z);
    }

    INLINE int any(int4 val) OVERLOADABLE CONST
    {
        return any(val.x) || any(val.y) || any(val.z) || any(val.w);
    }

    INLINE int any(int8 val) OVERLOADABLE CONST
    {
        return any(val.s0) || any(val.s1) || any(val.s2) || any(val.s3) || any(val.s4) || any(val.s5) || any(val.s6) || any(val.s7);
    }

    INLINE int any(int16 val) OVERLOADABLE CONST
    {
        return any(val.s0) || any(val.s1) || any(val.s2) || any(val.s3) || any(val.s4) || any(val.s5) || any(val.s6) || any(val.s7) || any(val.s8) || any(val.s9) || any(val.sa) || any(val.sb) || any(val.sc) || any(val.sd) || any(val.se) || any(val.sf);
    }

    INLINE int all(char val) OVERLOADABLE CONST
    {
        //tests for MSB set
        return vc4cl_msb_set(val);
    }

    INLINE int all(char2 val) OVERLOADABLE CONST
    {
        return all(val.x) && all(val.y);
    }

    INLINE int all(char3 val) OVERLOADABLE CONST
    {
        return all(val.x) && all(val.y) && all(val.z);
    }

    INLINE int all(char4 val) OVERLOADABLE CONST
    {
        return all(val.x) && all(val.y) && all(val.z) && all(val.w);
    }

    INLINE int all(char8 val) OVERLOADABLE CONST
    {
        return all(val.s0) && all(val.s1) && all(val.s2) && all(val.s3) && all(val.s4) && all(val.s5) && all(val.s6) && all(val.s7);
    }

    INLINE int all(char16 val) OVERLOADABLE CONST
    {
        return all(val.s0) && all(val.s1) && all(val.s2) && all(val.s3) && all(val.s4) && all(val.s5) && all(val.s6) && all(val.s7) && all(val.s8) && all(val.s9) && all(val.sa) && all(val.sb) && all(val.sc) && all(val.sd) && all(val.se) && all(val.sf);
    }

    INLINE int all(short val) OVERLOADABLE CONST
    {
        //tests for MSB set
        return vc4cl_msb_set(val);
    }

    INLINE int all(short2 val) OVERLOADABLE CONST
    {
        return all(val.x) && all(val.y);
    }

    INLINE int all(short3 val) OVERLOADABLE CONST
    {
        return all(val.x) && all(val.y) && all(val.z);
    }

    INLINE int all(short4 val) OVERLOADABLE CONST
    {
        return all(val.x) && all(val.y) && all(val.z) && all(val.w);
    }

    INLINE int all(short8 val) OVERLOADABLE CONST
    {
        return all(val.s0) && all(val.s1) && all(val.s2) && all(val.s3) && all(val.s4) && all(val.s5) && all(val.s6) && all(val.s7);
    }

    INLINE int all(short16 val) OVERLOADABLE CONST
    {
        return all(val.s0) && all(val.s1) && all(val.s2) && all(val.s3) && all(val.s4) && all(val.s5) && all(val.s6) && all(val.s7) && all(val.s8) && all(val.s9) && all(val.sa) && all(val.sb) && all(val.sc) && all(val.sd) && all(val.se) && all(val.sf);
    }

    INLINE int all(int val) OVERLOADABLE CONST
    {
        //tests for MSB set
        return vc4cl_msb_set(val);
    }

    INLINE int all(int2 val) OVERLOADABLE CONST
    {
        return all(val.x) && all(val.y);
    }

    INLINE int all(int3 val) OVERLOADABLE CONST
    {
        return all(val.x) && all(val.y) && all(val.z);
    }

    INLINE int all(int4 val) OVERLOADABLE CONST
    {
        return all(val.x) && all(val.y) && all(val.z) && all(val.w);
    }

    INLINE int all(int8 val) OVERLOADABLE CONST
    {
        return all(val.s0) && all(val.s1) && all(val.s2) && all(val.s3) && all(val.s4) && all(val.s5) && all(val.s6) && all(val.s7);
    }

    INLINE int all(int16 val) OVERLOADABLE CONST
    {
        return all(val.s0) && all(val.s1) && all(val.s2) && all(val.s3) && all(val.s4) && all(val.s5) && all(val.s6) && all(val.s7) && all(val.s8) && all(val.s9) && all(val.sa) && all(val.sb) && all(val.sc) && all(val.sd) && all(val.se) && all(val.sf);
    }
    */

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
    SIMPLE_3(uchar, select, uchar, a, uchar, b, uchar, c, vc4cl_msb_set(c) ? b : a)
    SIMPLE_3(uchar, select, uchar, a, uchar, b, char, c, vc4cl_msb_set(c) ? b : a)
	SIMPLE_3(uchar, select, uchar, a, uchar, b, ushort, c, vc4cl_bitcast_uchar(vc4cl_extend(vc4cl_msb_set(c))) ? b : a)
	SIMPLE_3(uchar, select, uchar, a, uchar, b, short, c, vc4cl_bitcast_uchar(vc4cl_extend(vc4cl_msb_set(c))) ? b : a)
	SIMPLE_3(uchar, select, uchar, a, uchar, b, uint, c, vc4cl_bitcast_uchar(vc4cl_extend(vc4cl_msb_set(c))) ? b : a)
	SIMPLE_3(uchar, select, uchar, a, uchar, b, int, c, vc4cl_bitcast_uchar(vc4cl_extend(vc4cl_msb_set(c))) ? b : a)
    SIMPLE_3(char, select, char, a, char, b, uchar, c, vc4cl_msb_set(c) ? b : a)
    SIMPLE_3(char, select, char, a, char, b, char, c, vc4cl_msb_set(c) ? b : a)
	SIMPLE_3(char, select, char, a, char, b, ushort, c, vc4cl_bitcast_char(vc4cl_extend(vc4cl_msb_set(c))) ? b : a)
	SIMPLE_3(char, select, char, a, char, b, short, c, vc4cl_bitcast_char(vc4cl_extend(vc4cl_msb_set(c))) ? b : a)
	SIMPLE_3(char, select, char, a, char, b, uint, c, vc4cl_bitcast_char(vc4cl_extend(vc4cl_msb_set(c))) ? b : a)
	SIMPLE_3(char, select, char, a, char, b, int, c, vc4cl_bitcast_char(vc4cl_extend(vc4cl_msb_set(c))) ? b : a)
    SIMPLE_3(ushort, select, ushort, a, ushort, b, uchar, c, vc4cl_bitcast_ushort(vc4cl_extend(vc4cl_msb_set(c))) ? b : a)
    SIMPLE_3(ushort, select, ushort, a, ushort, b, char, c, vc4cl_bitcast_ushort(vc4cl_extend(vc4cl_msb_set(c))) ? b : a)
	SIMPLE_3(ushort, select, ushort, a, ushort, b, ushort, c, vc4cl_msb_set(c) ? b : a)
	SIMPLE_3(ushort, select, ushort, a, ushort, b, short, c, vc4cl_msb_set(c) ? b : a)
	SIMPLE_3(ushort, select, ushort, a, ushort, b, uint, c, vc4cl_bitcast_ushort(vc4cl_extend(vc4cl_msb_set(c))) ? b : a)
	SIMPLE_3(ushort, select, ushort, a, ushort, b, int, c, vc4cl_bitcast_ushort(vc4cl_extend(vc4cl_msb_set(c))) ? b : a)
    SIMPLE_3(short, select, short, a, short, b, uchar, c, vc4cl_bitcast_short(vc4cl_extend(vc4cl_msb_set(c))) ? b : a)
    SIMPLE_3(short, select, short, a, short, b, char, c, vc4cl_bitcast_short(vc4cl_extend(vc4cl_msb_set(c))) ? b : a)
	SIMPLE_3(short, select, short, a, short, b, ushort, c, vc4cl_msb_set(c) ? b : a)
	SIMPLE_3(short, select, short, a, short, b, short, c, vc4cl_msb_set(c) ? b : a)
	SIMPLE_3(short, select, short, a, short, b, uint, c, vc4cl_bitcast_short(vc4cl_extend(vc4cl_msb_set(c))) ? b : a)
	SIMPLE_3(short, select, short, a, short, b, int, c, vc4cl_bitcast_short(vc4cl_extend(vc4cl_msb_set(c))) ? b : a)
    SIMPLE_3(uint, select, uint, a, uint, b, uchar, c, vc4cl_bitcast_uint(vc4cl_extend(vc4cl_msb_set(c))) ? b : a)
    SIMPLE_3(uint, select, uint, a, uint, b, char, c, vc4cl_bitcast_uint(vc4cl_extend(vc4cl_msb_set(c))) ? b : a)
	SIMPLE_3(uint, select, uint, a, uint, b, ushort, c, vc4cl_bitcast_uint(vc4cl_extend(vc4cl_msb_set(c))) ? b : a)
	SIMPLE_3(uint, select, uint, a, uint, b, short, c, vc4cl_bitcast_uint(vc4cl_extend(vc4cl_msb_set(c))) ? b : a)
	SIMPLE_3(uint, select, uint, a, uint, b, uint, c, vc4cl_msb_set(c) ? b : a)
	SIMPLE_3(uint, select, uint, a, uint, b, int, c, vc4cl_msb_set(c) ? b : a)
    SIMPLE_3(int, select, int, a, int, b, uchar, c, vc4cl_bitcast_int(vc4cl_extend(vc4cl_msb_set(c))) ? b : a)
    SIMPLE_3(int, select, int, a, int, b, char, c, vc4cl_bitcast_int(vc4cl_extend(vc4cl_msb_set(c))) ? b : a)
	SIMPLE_3(int, select, int, a, int, b, ushort, c, vc4cl_bitcast_int(vc4cl_extend(vc4cl_msb_set(c))) ? b : a)
	SIMPLE_3(int, select, int, a, int, b, short, c, vc4cl_bitcast_int(vc4cl_extend(vc4cl_msb_set(c))) ? b : a)
	SIMPLE_3(int, select, int, a, int, b, uint, c, vc4cl_msb_set(c) ? b : a)
	SIMPLE_3(int, select, int, a, int, b, int, c, vc4cl_msb_set(c) ? b : a)
    SIMPLE_3(float, select, float, a, float, b, uchar, c, vc4cl_extend(vc4cl_msb_set(c)) ? b : a)
    SIMPLE_3(float, select, float, a, float, b, char, c, vc4cl_extend(vc4cl_msb_set(c)) ? b : a)
	SIMPLE_3(float, select, float, a, float, b, ushort, c, vc4cl_extend(vc4cl_msb_set(c)) ? b : a)
	SIMPLE_3(float, select, float, a, float, b, short, c, vc4cl_extend(vc4cl_msb_set(c)) ? b : a)
	SIMPLE_3(float, select, float, a, float, b, uint, c, vc4cl_msb_set(c) ? b : a)
	SIMPLE_3(float, select, float, a, float, b, int, c, vc4cl_msb_set(c) ? b : a)

#endif /* VC4CL_RELATIONAL_H */

