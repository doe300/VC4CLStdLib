/*
 * Author: doe300
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef VC4CL_FLAGS_H
#define VC4CL_FLAGS_H

#include "_overloads.h"

enum Flag
{
	FLAG_ZERO_SET = 'z',
	FLAG_ZERO_CLEAR = 'Z',
	FLAG_NEGATIVE_SET = 'n',
	FLAG_NEGATIVE_CLEAR = 'N',
	FLAG_CARRY_SET = 'c',
	FLAG_CARRY_CLEAR = 'C'
};

/* Returns the given value of the flag is set, the alternative otherwise: flag ? ifSet : ifClear */
OVERLOAD_3_SCALAR(uchar, vc4cl_flag_cond, uchar, ifSet, uchar, ifClear, enum Flag, flag)
OVERLOAD_3_SCALAR(char, vc4cl_flag_cond, char, ifSet, char, ifClear, enum Flag, flag)
OVERLOAD_3_SCALAR(ushort, vc4cl_flag_cond, ushort, ifSet, ushort, ifClear, enum Flag, flag)
OVERLOAD_3_SCALAR(short, vc4cl_flag_cond, short, ifSet, short, ifClear, enum Flag, flag)
OVERLOAD_3_SCALAR(uint, vc4cl_flag_cond, uint, ifSet, uint, ifClear, enum Flag, flag)
OVERLOAD_3_SCALAR(int, vc4cl_flag_cond, int, ifSet, int, ifClear, enum Flag, flag)
OVERLOAD_3_SCALAR(float, vc4cl_flag_cond, float, ifSet, float, ifClear, enum Flag, flag)

/* Carries out the operation and sets the flags */
OVERLOAD_2(int, vc4cl_add_flags, int, x, int, y)
OVERLOAD_2(uint, vc4cl_add_flags, uint, x, uint, y)
OVERLOAD_2(int, vc4cl_sub_flags, int, x, int, y)

#endif /* VC4CL_FLAGS_H */