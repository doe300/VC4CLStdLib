/*
 * Author: doe300
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef VC4CL_OPERATORS_H
#define VC4CL_OPERATORS_H

#include "_config.h"
#include "_intrinsics.h"
#include "_flags.h"

/*
 *                             | a[0] .        a[1]        |
 *  *                          | b[0] .        b[1]        |
 * ---------------------------------------------------------
 * |xxxxxx.xxxxxx.xxxxxx.xxxxxx|      .      .      .      |
 *
 *                             |        a[1] * b[1]        |
 *   +    |        a[1] * b[0]        |
 *   +    |        a[0] * b[1]        |
 *
 */
SIMPLE_2(uchar, vc4cl_mul, uchar, x, uchar, y, vc4cl_bitcast_uchar(vc4cl_mul24(x, y, VC4CL_UNSIGNED)))
SIMPLE_2(char, vc4cl_mul, char, x, char, y, vc4cl_bitcast_char(vc4cl_mul24(x, y, VC4CL_SIGNED)))
SIMPLE_2(ushort, vc4cl_mul, ushort, x, ushort, y, vc4cl_bitcast_ushort(vc4cl_mul24(x, y, VC4CL_UNSIGNED)))
SIMPLE_2(short, vc4cl_mul, short, x, short, y, vc4cl_bitcast_short(vc4cl_mul24(x, y, VC4CL_SIGNED)))
SIMPLE_2(uint, vc4cl_mul, uint, x, uint, y, vc4cl_mul24(vc4cl_bitcast_ushort(x), vc4cl_bitcast_ushort(y), VC4CL_UNSIGNED) + (vc4cl_mul24(vc4cl_bitcast_ushort(x), vc4cl_bitcast_ushort(y >> 24), VC4CL_UNSIGNED) << 24) + (vc4cl_mul24(vc4cl_bitcast_ushort(x >> 24), vc4cl_bitcast_ushort(y), VC4CL_UNSIGNED) << 24))
SIMPLE_2(int, vc4cl_mul, int, x, int, y, vc4cl_mul24(vc4cl_bitcast_short(x), vc4cl_bitcast_short(y), VC4CL_SIGNED) + (vc4cl_mul24(vc4cl_bitcast_short(x), vc4cl_bitcast_short(y >> 24), VC4CL_SIGNED) << 24) + (vc4cl_mul24(vc4cl_bitcast_short(x >> 24), vc4cl_bitcast_short(y), VC4CL_SIGNED) << 24))

/*
 *                             |     a[0]    .    a[1]     |
 *  *                          |     b[0]    .    b[1]     |
 * ---------------------------------------------------------
 * |      .      .      .      |xxxxxx.xxxxxx.xxxxxx.xxxxxx|
 *
 *                             |        a[1] * b[1]        |
 *               |        a[1] * b[0]        |
 *   +           |        a[0] * b[1]        |
 *+|        a[0] * b[0]        |
 *
 *  = (((a[1] * b[1]) >> 16 + a[1] * b[0] + a[0] * b[1]) >> 16) + a[0] * b[0] 
 */
SIMPLE_2(uchar, vc4cl_mul_hi, uchar, x, uchar, y, vc4cl_bitcast_uchar(vc4cl_mul24(x, y, VC4CL_UNSIGNED) >> 8))
SIMPLE_2(char, vc4cl_mul_hi, char, x, char, y, vc4cl_bitcast_char(vc4cl_asr(vc4cl_mul24(vc4cl_sign_extend(x), vc4cl_sign_extend(y), VC4CL_SIGNED), 8)))
SIMPLE_2(ushort, vc4cl_mul_hi, ushort, x, ushort, y, vc4cl_bitcast_ushort(vc4cl_mul24(x, y, VC4CL_UNSIGNED) >> 16))
//SIMPLE_2(short, vc4cl_mul_hi, short, x, short, y, vc4cl_bitcast_short(vc4cl_asr(vc4cl_mul24(vc4cl_sign_extend(x), vc4cl_sign_extend(y), VC4CL_SIGNED), 16)))
SIMPLE_2(short, vc4cl_mul_hi, short, x, short, y, vc4cl_bitcast_short(vc4cl_asr(vc4cl_mul24(x, y, VC4CL_SIGNED), 16)))
COMPLEX_2(uint, vc4cl_mul_hi, uint, x, uint, y, {
  arg0_t a0 = x >> 16;
  arg0_t a1 = x & 0xFFFF;
  arg1_t b0 = y >> 16;
  arg1_t b1 = y & 0xFFFF;
  arg0_t lower = vc4cl_mul24(a1, b1, VC4CL_UNSIGNED) >> 16;
  arg0_t m1 = vc4cl_mul24(a1, b0, VC4CL_UNSIGNED);
  arg0_t m2 = vc4cl_mul24(a0, b1, VC4CL_UNSIGNED);
  // off by exactly 2^16 if these additions would overflow, so we need to add the carry bit afterwards
  //TODO still off by 2^16 for some values, carry check not working (in emulator)??
  arg0_t add0 = vc4cl_add_flags(lower, m1);
  arg0_t carry0 = vc4cl_flag_cond(1 << 16, 0, FLAG_CARRY_SET);
  arg0_t add1 = vc4cl_add_flags(add0, m2) >> 16;
  arg0_t carry1 = vc4cl_flag_cond(1 << 16, 0, FLAG_CARRY_SET);
  arg0_t middle = add1 + (carry0 | carry1);
  return middle + vc4cl_mul24(a0, b0, VC4CL_UNSIGNED);
})
SIMPLE_2(int, vc4cl_mul_hi, int, x, int, y, vc4cl_mul24(vc4cl_asr(x, 16), vc4cl_asr(y, 16), VC4CL_SIGNED) + vc4cl_asr(vc4cl_mul24(x, vc4cl_asr(y, 16), VC4CL_SIGNED), 16) + vc4cl_asr(vc4cl_mul24(vc4cl_asr(x, 16), y, VC4CL_SIGNED), 16))


#endif /* VC4CL_OPERATORS_H */

