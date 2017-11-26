/*
 * Author: doe300
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef VC4CL_INTEGER_H
#define VC4CL_INTEGER_H

#include "_config.h"
#include "_intrinsics.h"
#include "_operators.h"

#define SIMPLE_INTEGER_2(func, argName0, argName1, content) \
		SIMPLE_2(uchar, func, uchar, argName0, uchar, argName1, content) \
		SIMPLE_2(char, func, char, argName0, char, argName1, content) \
		SIMPLE_2(ushort, func, ushort, argName0, ushort, argName1, content) \
		SIMPLE_2(short, func, short, argName0, short, argName1, content) \
		SIMPLE_2(uint, func, uint, argName0, uint, argName1, content) \
		SIMPLE_2(int, func, int, argName0, int, argName1, content) \

#define SIMPLE_INTEGER_3(func, argName0, argName1, argName2, content) \
		SIMPLE_3(uchar, func, uchar, argName0, uchar, argName1, uchar, argName2, content) \
		SIMPLE_3(char, func, char, argName0, char, argName1, char, argName2, content) \
		SIMPLE_3(ushort, func, ushort, argName0, ushort, argName1, ushort, argName2, content) \
		SIMPLE_3(short, func, short, argName0, short, argName1, short, argName2, content) \
		SIMPLE_3(uint, func, uint, argName0, uint, argName1, uint, argName2, content) \
		SIMPLE_3(int, func, int, argName0, int, argName1, int, argName2, content) \


SIMPLE_1(uchar, abs, char, val, vc4cl_bitcast_uchar(max(vc4cl_extend(val), -vc4cl_extend(val))))
SIMPLE_1(uchar, abs, uchar, val, val)
SIMPLE_1(ushort, abs, short, val, vc4cl_bitcast_ushort(max(vc4cl_extend(val), -vc4cl_extend(val))))
SIMPLE_1(ushort, abs, ushort, val, val)
SIMPLE_1(uint, abs, int, val, vc4cl_bitcast_uint(max(val, -val)))
SIMPLE_1(uint, abs, uint, val, val)

//based on pocl (pocl/lib/kernel/abs_diff.cl)
SIMPLE_2(uchar, abs_diff, uchar, x, uchar, y, (result_t)abs(x > y ? x - y : y - x))
SIMPLE_2(uchar, abs_diff, char, x, char, y, vc4cl_bitcast_char(vc4cl_msb_set(x & y)) ? /* same sign -> no under/overflow */ (result_t)abs(x - y) : /* different signs */ abs(x) + abs(y))
SIMPLE_2(ushort, abs_diff, ushort, x, ushort, y, (result_t)abs(x > y ? x - y : y - x))
SIMPLE_2(ushort, abs_diff, short, x, short, y, vc4cl_bitcast_short(vc4cl_msb_set(x & y)) ? /* same sign -> no under/overflow */ (result_t)abs(x - y) : /* different signs */ abs(x) + abs(y))
SIMPLE_2(uint, abs_diff, uint, x, uint, y, abs(x > y ? x - y : y - x))
SIMPLE_2(uint, abs_diff, int, x, int, y, vc4cl_msb_set(x & y) ? /* same sign -> no under/overflow */ abs(x - y) : /* different signs */ abs(x) + abs(y))

SIMPLE_2(uchar, add_sat, uchar, x, uchar, y, vc4cl_saturate_lsb(vc4cl_extend(x) + vc4cl_extend(y)))
SIMPLE_2(char, add_sat, char, x, char, y, vc4cl_bitcast_char(clamp(vc4cl_extend(x) + vc4cl_extend(y), SCHAR_MIN, SCHAR_MAX)))
SIMPLE_2(ushort, add_sat, ushort, x, ushort, y, vc4cl_bitcast_ushort(clamp(vc4cl_extend(x) + vc4cl_extend(y), (uint) 0, (uint) USHRT_MAX)))
//SIMPLE_2(short, add_sat, short, x, short, y, vc4cl_bitcast_short(clamp(vc4cl_extend(x) + vc4cl_extend(y), SHRT_MIN, SHRT_MAX)))
SIMPLE_2(short, add_sat, short, x, short, y, vc4cl_saturate_short(vc4cl_extend(x) + vc4cl_extend(y)))
//based on pocl (pocl/lib/kernel/add_sat.cl)
SIMPLE_2(uint, add_sat, uint, x, uint, y, x > ((result_t)UINT_MAX) - y ? UINT_MAX : x + y)
SIMPLE_2(int, add_sat, int, x, int, y, vc4cl_saturated_add(x, y))

//"Returns (x + y) >> 1.  The intermediate sum does not modulo overflow."
SIMPLE_2(uchar, hadd, uchar, x, uchar, y, vc4cl_pack_lsb((vc4cl_extend(x) + vc4cl_extend(y)) >> 1))
SIMPLE_2(char, hadd, char, x, char, y, vc4cl_bitcast_char(vc4cl_asr(vc4cl_extend(x) + vc4cl_extend(y), 1)))
SIMPLE_2(ushort, hadd, ushort, x, ushort, y, vc4cl_bitcast_ushort((vc4cl_extend(x) + vc4cl_extend(y)) >> 1))
SIMPLE_2(short, hadd, short, x, short, y, vc4cl_bitcast_short(vc4cl_asr(vc4cl_extend(x) + vc4cl_extend(y), 1)))
//based on pocl (pocl/lib/kernel/hadd.cl)
SIMPLE_2(uint, hadd, uint, x, uint, y, (x >> (arg0_t)1) + (y >> (arg0_t)1) + (x & y & (arg0_t)1))
SIMPLE_2(int, hadd, int, x, int, y, (x >> (arg0_t)1) + (y >> (arg0_t)1) + (x & y & (arg0_t)1))

//"Returns (x + y + 1) >> 1.  The intermediate sum does not modulo overflow."
SIMPLE_2(uchar, rhadd, uchar, x, uchar, y, vc4cl_pack_lsb((vc4cl_extend(x) + vc4cl_extend(y) + (uint)1) >> 1))
SIMPLE_2(char, rhadd, char, x, char, y, vc4cl_bitcast_char(vc4cl_asr(vc4cl_extend(x) + vc4cl_extend(y) + (int)1, 1)))
SIMPLE_2(ushort, rhadd, ushort, x, ushort, y, vc4cl_bitcast_ushort((vc4cl_extend(x) + vc4cl_extend(y) + (uint)1) >> 1))
SIMPLE_2(short, rhadd, short, x, short, y, vc4cl_bitcast_short(vc4cl_asr(vc4cl_extend(x) + vc4cl_extend(y) + (int)1, 1)))
//based on pocl (pocl/lib/kernel/rhadd.cl)
SIMPLE_2(uint, rhadd, uint, x, uint, y, (x >> (arg0_t)1) + (y >> (arg0_t)1) + ((x | y) & (arg0_t)1))
SIMPLE_2(int, rhadd, int, x, int, y, (x >> (arg0_t)1) + (y >> (arg0_t)1) + ((x | y) & (arg0_t)1))

SIMPLE_INTEGER_3(clamp, val, minval, maxval, min(max(val, minval), maxval))
SIMPLE_3_TWO_SCALAR(uchar, clamp, uchar, val, uchar, minval, uchar, maxval, min(max(val, minval), maxval))
SIMPLE_3_TWO_SCALAR(char, clamp, char, val, char, minval, char, maxval, min(max(val, minval), maxval))
SIMPLE_3_TWO_SCALAR(ushort, clamp, ushort, val, ushort, minval, ushort, maxval, min(max(val, minval), maxval))
SIMPLE_3_TWO_SCALAR(short, clamp, short, val, short, minval, short, maxval, min(max(val, minval), maxval))
SIMPLE_3_TWO_SCALAR(uint, clamp, uint, val, uint, minval, uint, maxval, min(max(val, minval), maxval))
SIMPLE_3_TWO_SCALAR(int, clamp, int, val, int, minval, int, maxval, min(max(val, minval), maxval))

SIMPLE_1(uchar, clz, uchar, x, vc4cl_bitcast_uchar(vc4cl_clz((vc4cl_and(x, (arg_t)0xFF) << 24) | 0xFFFFFF)))
SIMPLE_1(char, clz, char, x, vc4cl_bitcast_char(vc4cl_clz((vc4cl_and(x, (arg_t)0xFF) << 24) | 0xFFFFFF)))
SIMPLE_1(ushort, clz, ushort, x, vc4cl_bitcast_ushort(vc4cl_clz((vc4cl_and(x, (arg_t)0xFFFF) << 16) | 0xFFFF)))
SIMPLE_1(short, clz, short, x, vc4cl_bitcast_short(vc4cl_clz((vc4cl_and(x, (arg_t)0xFFFF) << 16) | 0xFFFF)))
SIMPLE_1(uint, clz, uint, x, vc4cl_bitcast_uint(vc4cl_clz(x)))
SIMPLE_1(int, clz, int, x, vc4cl_bitcast_int(vc4cl_clz(x)))

SIMPLE_INTEGER_3(mad_hi, x, y, z, mul_hi(x, y) + z)

SIMPLE_3(uchar, mad_sat, uchar, x, uchar, y, uchar, z, vc4cl_bitcast_uchar(clamp(vc4cl_extend(x) * vc4cl_extend(y) + vc4cl_extend(z), (uint) 0, (uint) UCHAR_MAX)))
SIMPLE_3(char, mad_sat, char, x, char, y, char, z, vc4cl_bitcast_char(clamp(vc4cl_extend(x) * vc4cl_extend(y) + vc4cl_extend(z), (int) CHAR_MIN, (int) CHAR_MAX)))
SIMPLE_3(ushort, mad_sat, ushort, x, ushort, y, ushort, z, vc4cl_bitcast_ushort(clamp(vc4cl_extend(x) * vc4cl_extend(y) + vc4cl_extend(z), (uint) 0, (uint) USHRT_MAX)))
SIMPLE_3(short, mad_sat, short, x, short, y, short, z, vc4cl_bitcast_short(clamp(vc4cl_extend(x) * vc4cl_extend(y) + vc4cl_extend(z), (int) SHRT_MIN, (int) SHRT_MAX)))
COMPLEX_3(uint, mad_sat, uint, x, uint, y, uint, z,
{
	result_t a = x * y;
	return UINT_MAX - a < z ? (result_t)UINT_MAX : a + z;
})
COMPLEX_3(int, mad_sat, int, x, int, y, int, z,
{
	result_t a = x * y;
	return INT_MAX - a < z ? (result_t)INT_MAX :
		INT_MIN + a > z ? (result_t)INT_MIN :
			a + z;
})

SIMPLE_2(uchar, max, uchar, x, uchar, y, vc4cl_bitcast_uchar(vc4cl_max(vc4cl_zero_extend(x), vc4cl_zero_extend(y), VC4CL_UNSIGNED)))
SIMPLE_2_SCALAR(uchar, max, uchar, x, uchar, y, vc4cl_bitcast_uchar(vc4cl_max(vc4cl_zero_extend(x), vc4cl_zero_extend(y), VC4CL_UNSIGNED)))
SIMPLE_2(char, max, char, x, char, y, vc4cl_bitcast_char(vc4cl_max(vc4cl_sign_extend(x), vc4cl_sign_extend(y), VC4CL_SIGNED)))
SIMPLE_2_SCALAR(char, max, char, x, char, y, vc4cl_bitcast_char(vc4cl_max(vc4cl_sign_extend(x), vc4cl_sign_extend(y), VC4CL_SIGNED)))
SIMPLE_2(ushort, max, ushort, x, ushort, y, vc4cl_bitcast_ushort(vc4cl_max(vc4cl_zero_extend(x), vc4cl_zero_extend(y), VC4CL_UNSIGNED)))
SIMPLE_2_SCALAR(ushort, max, ushort, x, ushort, y, vc4cl_bitcast_ushort(vc4cl_max(vc4cl_zero_extend(x), vc4cl_zero_extend(y), VC4CL_UNSIGNED)))
SIMPLE_2(short, max, short, x, short, y, vc4cl_bitcast_short(vc4cl_max(vc4cl_sign_extend(x), vc4cl_sign_extend(y), VC4CL_SIGNED)))
SIMPLE_2_SCALAR(short, max, short, x, short, y, vc4cl_bitcast_short(vc4cl_max(vc4cl_sign_extend(x), vc4cl_sign_extend(y), VC4CL_SIGNED)))
SIMPLE_2(uint, max, uint, x, uint, y, vc4cl_max(x, y, VC4CL_UNSIGNED))
SIMPLE_2_SCALAR(uint, max, uint, x, uint, y, vc4cl_max(x, y, VC4CL_UNSIGNED))
SIMPLE_2(int, max, int, x, int, y, vc4cl_max(x, y, VC4CL_SIGNED))
SIMPLE_2_SCALAR(int, max, int, x, int, y, vc4cl_max(x, y, VC4CL_SIGNED))

SIMPLE_2(uchar, min, uchar, x, uchar, y, vc4cl_bitcast_uchar(vc4cl_min(vc4cl_zero_extend(x), vc4cl_zero_extend(y), VC4CL_UNSIGNED)))
SIMPLE_2_SCALAR(uchar, min, uchar, x, uchar, y, vc4cl_bitcast_uchar(vc4cl_min(vc4cl_zero_extend(x), vc4cl_zero_extend(y), VC4CL_UNSIGNED)))
SIMPLE_2(char, min, char, x, char, y, vc4cl_bitcast_char(vc4cl_min(vc4cl_sign_extend(x), vc4cl_sign_extend(y), VC4CL_SIGNED)))
SIMPLE_2_SCALAR(char, min, char, x, char, y, vc4cl_bitcast_char(vc4cl_min(vc4cl_sign_extend(x), vc4cl_sign_extend(y), VC4CL_SIGNED)))
SIMPLE_2(ushort, min, ushort, x, ushort, y, vc4cl_bitcast_ushort(vc4cl_min(vc4cl_zero_extend(x), vc4cl_zero_extend(y), VC4CL_UNSIGNED)))
SIMPLE_2_SCALAR(ushort, min, ushort, x, ushort, y, vc4cl_bitcast_ushort(vc4cl_min(vc4cl_zero_extend(x), vc4cl_zero_extend(y), VC4CL_UNSIGNED)))
SIMPLE_2(short, min, short, x, short, y, vc4cl_bitcast_short(vc4cl_min(vc4cl_sign_extend(x), vc4cl_sign_extend(y), VC4CL_SIGNED)))
SIMPLE_2_SCALAR(short, min, short, x, short, y, vc4cl_bitcast_short(vc4cl_min(vc4cl_sign_extend(x), vc4cl_sign_extend(y), VC4CL_SIGNED)))
SIMPLE_2(uint, min, uint, x, uint, y, vc4cl_min(x, y, VC4CL_UNSIGNED))
SIMPLE_2_SCALAR(uint, min, uint, x, uint, y, vc4cl_min(x, y, VC4CL_UNSIGNED))
SIMPLE_2(int, min, int, x, int, y, vc4cl_min(x, y, VC4CL_SIGNED))
SIMPLE_2_SCALAR(int, min, int, x, int, y, vc4cl_min(x, y, VC4CL_SIGNED))

SIMPLE_INTEGER_2(mul_hi, a, b, vc4cl_mul_hi(a, b))

//Since the rotation is over all 32-bits, for smaller types we need to replicate the value, rotate it and truncate/sign extend the result afterwards
SIMPLE_2(uchar, rotate, uchar, x, uchar, y, vc4cl_pack_lsb(vc4cl_ror(vc4cl_replicate_lsb(x), -vc4cl_bitcast_int(vc4cl_zero_extend(y)))))
SIMPLE_2(char, rotate, char, x, char, y, vc4cl_bitcast_char(vc4cl_extend(vc4cl_bitcast_char(vc4cl_ror(vc4cl_replicate_lsb(x), -vc4cl_sign_extend(y))))))
SIMPLE_2(ushort, rotate, ushort, x, ushort, y, vc4cl_pack_truncate(vc4cl_ror(vc4cl_zero_extend(x) | (vc4cl_zero_extend(x) << 16), -vc4cl_bitcast_int(vc4cl_zero_extend(y)))))
SIMPLE_2(short, rotate, short, x, short, y, vc4cl_bitcast_short(vc4cl_extend(vc4cl_bitcast_short(vc4cl_ror((vc4cl_sign_extend(x) & (int) 0xFFFF) | (vc4cl_sign_extend(x) << 16), -vc4cl_sign_extend(y))))))
SIMPLE_2(uint, rotate, uint, x, uint, y, vc4cl_bitcast_uint(vc4cl_ror(x, -vc4cl_bitcast_int(y))))
SIMPLE_2(int, rotate, int, x, int, y, vc4cl_bitcast_int(vc4cl_ror(x, -y)))

SIMPLE_2(uchar, sub_sat, uchar, x, uchar, y, vc4cl_bitcast_uchar(clamp(vc4cl_extend(x) - vc4cl_extend(y), (uint) 0, (uint) UCHAR_MAX)))
SIMPLE_2(char, sub_sat, char, x, char, y, vc4cl_bitcast_char(clamp(vc4cl_extend(x) - vc4cl_extend(y), SCHAR_MIN, SCHAR_MAX)))
SIMPLE_2(ushort, sub_sat, ushort, x, ushort, y, vc4cl_bitcast_ushort(clamp(vc4cl_extend(x) - vc4cl_extend(y), (uint) 0, (uint) USHRT_MAX)))
//SIMPLE_2(short, sub_sat, short, x, short, y, vc4cl_bitcast_short(clamp(vc4cl_extend(x) - vc4cl_extend(y), SHRT_MIN, SHRT_MAX)))
SIMPLE_2(short, sub_sat, short, x, short, y, vc4cl_saturate_short(vc4cl_extend(x) - vc4cl_extend(y)))
//based on pocl (pocl/lib/kernel/sub_sat.cl)
SIMPLE_2(uint, sub_sat, uint, x, uint, y, x < y ? (result_t)0 : x - y)
SIMPLE_2(int, sub_sat, int, x, int, y, vc4cl_saturated_sub(x, y))

SIMPLE_2(short, upsample, char, hi, uchar, lo, vc4cl_bitcast_short((vc4cl_sign_extend(hi) << 8) | vc4cl_bitcast_int(vc4cl_zero_extend(lo))))
SIMPLE_2(ushort, upsample, uchar, hi, uchar, lo, vc4cl_bitcast_ushort((vc4cl_zero_extend(hi) << 8) | vc4cl_zero_extend(lo)))
SIMPLE_2(int, upsample, short, hi, ushort, lo, (vc4cl_sign_extend(hi) << 16) | vc4cl_bitcast_int(vc4cl_zero_extend(lo)))
SIMPLE_2(uint, upsample, ushort, hi, ushort, lo, (vc4cl_zero_extend(hi) << 16) | vc4cl_zero_extend(lo))

//" Returns the number of non-zero bits in x. "
SIMPLE_1(uint, _popcountByte, uint, val, vc4cl_extend(((val & 0x80) >> 7) + ((val & 0x40) >> 6) + ((val & 0x20) >> 5) + ((val & 0x10) >> 4) + ((val & 0x8) >> 3) + ((val & 0x4) >> 2) + ((val & 0x2) >> 1) + (val & 0x1)))
SIMPLE_1(int, _popcountByte, int, val, vc4cl_extend(((val & 0x80) >> 7) + ((val & 0x40) >> 6) + ((val & 0x20) >> 5) + ((val & 0x10) >> 4) + ((val & 0x8) >> 3) + ((val & 0x4) >> 2) + ((val & 0x2) >> 1) + (val & 0x1)))
SIMPLE_1(uchar, popcount, uchar, val, vc4cl_bitcast_uchar(_popcountByte(vc4cl_extend(val))))
SIMPLE_1(char, popcount, char, val,  vc4cl_bitcast_char(_popcountByte(vc4cl_extend(val))))
SIMPLE_1(ushort, popcount, ushort, val, vc4cl_bitcast_ushort(_popcountByte(vc4cl_extend(val & (arg_t) 0xFF)) + _popcountByte(vc4cl_extend((val >> 8) & (arg_t) 0xFF))))
SIMPLE_1(short, popcount, short, val,  vc4cl_bitcast_short(_popcountByte(vc4cl_extend(val & (arg_t) 0xFF)) + _popcountByte(vc4cl_extend((val >> 8) & (arg_t) 0xFF))))
SIMPLE_1(uint, popcount, uint, val, _popcountByte(val & (arg_t) 0xFF) + _popcountByte((val >> 8) & (arg_t) 0xFF) + _popcountByte((val >> 16) & (arg_t) 0xFF) + _popcountByte((val >> 24) & (arg_t) 0xFF))
SIMPLE_1(int, popcount, int, val, _popcountByte(val & (arg_t) 0xFF) + _popcountByte((val >> 8) & (arg_t) 0xFF) + _popcountByte((val >> 16) & (arg_t) 0xFF) + _popcountByte((val >> 24) & (arg_t) 0xFF))

SIMPLE_2(uchar, mul24, uchar, x, uchar, y, vc4cl_bitcast_uchar(vc4cl_mul24(x, y, VC4CL_UNSIGNED)))
SIMPLE_2(char, mul24, char, x, char, y, vc4cl_bitcast_char(vc4cl_mul24(x, y, VC4CL_SIGNED)))
SIMPLE_2(ushort, mul24, ushort, x, ushort, y, vc4cl_bitcast_ushort(vc4cl_mul24(x, y, VC4CL_UNSIGNED)))
SIMPLE_2(short, mul24, short, x, short, y, vc4cl_bitcast_short(vc4cl_mul24(x, y, VC4CL_SIGNED)))
SIMPLE_2(uint, mul24, uint, x, uint, y, vc4cl_mul24(x, y, VC4CL_UNSIGNED))
SIMPLE_2(int, mul24, int, x, int, y, vc4cl_mul24(x, y, VC4CL_SIGNED))
SIMPLE_INTEGER_3(mad24, a, b, c, mul24(a, b) + c)

#endif /* VC4CL_INTEGER_H */

