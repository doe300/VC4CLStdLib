/*
 * Author: doe300
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef VC4CL_OPERATORS_H
#define VC4CL_OPERATORS_H

#include "_config.h"
#include "_intrinsics.h"

    /*
     *                             |     a[0]    .    a[1]     |
     *  *                          |     b[0]    .    b[1]     |
     * ---------------------------------------------------------
     * |xxxxxx.xxxxxx.xxxxxx.xxxxxx|      .      .      .      |
     * 
     *                             |        a[1] * b[1]        |
     *   +           |        a[1] * b[0]        |
     *   +           |        a[0] * b[1]        |
     * 
     *  = a[1] * b[1] + (a[1] * b[0]) << 16 + (a[0] * b[1]) << 16
     */
    SIMPLE_2(uchar, vc4cl_mul, uchar, x, uchar, y, vc4cl_bitcast_uchar(vc4cl_mul24(x, y, VC4CL_UNSIGNED)))
    SIMPLE_2(char, vc4cl_mul, char, x, char, y, vc4cl_bitcast_char(vc4cl_mul24(x, y, VC4CL_SIGNED)))
    SIMPLE_2(ushort, vc4cl_mul, ushort, x, ushort, y, vc4cl_bitcast_ushort(vc4cl_mul24(x, y, VC4CL_UNSIGNED)))
    SIMPLE_2(short, vc4cl_mul, short, x, short, y, vc4cl_bitcast_short(vc4cl_mul24(x, y, VC4CL_SIGNED)))
    SIMPLE_2(uint, vc4cl_mul, uint, x, uint, y, vc4cl_mul24(vc4cl_bitcast_ushort(x), vc4cl_bitcast_ushort(y), VC4CL_UNSIGNED) + (vc4cl_mul24(vc4cl_bitcast_ushort(x), vc4cl_bitcast_ushort(y >> 16), VC4CL_UNSIGNED) << 16) + (vc4cl_mul24(vc4cl_bitcast_ushort(x >> 16), vc4cl_bitcast_ushort(y), VC4CL_UNSIGNED) << 16))
    SIMPLE_2(int, vc4cl_mul, int, x, int, y, vc4cl_mul24(vc4cl_bitcast_short(x), vc4cl_bitcast_short(y), VC4CL_SIGNED) + (vc4cl_mul24(vc4cl_bitcast_short(x), vc4cl_bitcast_short(y >> 16), VC4CL_SIGNED) << 16) + (vc4cl_mul24(vc4cl_bitcast_short(x >> 16), vc4cl_bitcast_short(y), VC4CL_SIGNED) << 16))
    
    /*
     *                             |     a[0]    .    a[1]     |
     *  *                          |     b[0]    .    b[1]     |
     * ---------------------------------------------------------
     * |      .      .      .      |xxxxxx.xxxxxx.xxxxxx.xxxxxx|
     * 
     *               |        a[1] * b[0]        |
     *   +           |        a[0] * b[1]        |
     *+|        a[0] * b[0]        |
     * 
     *  = a[0] * b[0] + (a[1] * b[0]) >> 16 + (a[0] * b[1]) >> 16
     */
    SIMPLE_2(uchar, vc4cl_mul_hi, uchar, x, uchar, y, vc4cl_bitcast_uchar(vc4cl_mul24(x, y, VC4CL_UNSIGNED) >> 8))
    SIMPLE_2(char, vc4cl_mul_hi, char, x, char, y, vc4cl_bitcast_char(vc4cl_mul24(x, y, VC4CL_SIGNED) >> 8))
    SIMPLE_2(ushort, vc4cl_mul_hi, ushort, x, ushort, y, vc4cl_bitcast_ushort(vc4cl_mul24(x, y, VC4CL_UNSIGNED) >> 16))
    SIMPLE_2(short, vc4cl_mul_hi, short, x, short, y, vc4cl_bitcast_short(vc4cl_mul24(x, y, VC4CL_SIGNED) >> 16))
    SIMPLE_2(uint, vc4cl_mul_hi, uint, x, uint, y, vc4cl_mul24(vc4cl_bitcast_ushort(x >> 16), vc4cl_bitcast_ushort(y >> 16), VC4CL_UNSIGNED) + (vc4cl_mul24(vc4cl_bitcast_ushort(x), vc4cl_bitcast_ushort(y >> 16), VC4CL_UNSIGNED) >> 16) + (vc4cl_mul24(vc4cl_bitcast_ushort(x >> 16), vc4cl_bitcast_ushort(y), VC4CL_UNSIGNED) >> 16))
    SIMPLE_2(int, vc4cl_mul_hi, int, x, int, y, vc4cl_mul24(vc4cl_bitcast_short(x >> 16), vc4cl_bitcast_short(y >> 16), VC4CL_SIGNED) + (vc4cl_mul24(vc4cl_bitcast_short(x), vc4cl_bitcast_short(y >> 16), VC4CL_SIGNED) >> 16) + (vc4cl_mul24(vc4cl_bitcast_short(x >> 16), vc4cl_bitcast_short(y), VC4CL_SIGNED) >> 16))


#endif /* VC4CL_OPERATORS_H */

