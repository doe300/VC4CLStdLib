/*
 * Author: doe300
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef VC4CL_EXTENSIONS_H
#define VC4CL_EXTENSIONS_H

#include "_config.h"
#include "_overloads.h"
#include "_intrinsics.h"


/*
 * Loop unroll pragma extension
 *
 * Defines "#pragma unroll <factor>"
 *
 * CLang supports this natively, so we do not need to do anything
 *
 * See https://www.khronos.org/registry/OpenCL/extensions/nv/cl_nv_pragma_unroll.txt
 * See https://clang.llvm.org/docs/AttributeReference.html#pragma-unroll-pragma-nounroll
 */
#ifndef cl_nv_pragma_unroll
#define cl_nv_pragma_unroll 1
#endif

/*
 * ARM core-ID extension
 *
 * Adds function
 * 	uint arm_get_core_id( void )
 * which returns the ID of the OpenCL Computation Unit, which is always zero
 *
 * See https://www.khronos.org/registry/OpenCL/extensions/arm/cl_arm_get_core_id.txt
 */
#ifndef cl_arm_core_id
#define cl_arm_core_id 1
#endif
uint arm_get_core_id(void);	//prototype, prevents warning
uint arm_get_core_id(void)
{
	return 0;
}

/*
 * 32-bit atomic counters
 *
 * Adds type
 *  counter_32_t
 * which is a 32-bit type for atomic counters. counter32_t can only be passed as kernel parameter and cannot be read/assigned.
 *
 * Adds functions
 *  uint atomic_inc(counter32_t counter)
 *  uint atomic_dec(counter32_t counter)
 * increments/decrements the given counter32_t value atomically.
 *
 * NOTE: Since the syntax/semantics is exactly the same as for the uint version of the standard atomic_inc/atomic_dec functions, counter32_t is used as typedef to an uint pointer.
 *
 * See https://www.khronos.org/registry/OpenCL/extensions/ext/cl_ext_atomic_counters_32.txt
 */
#ifndef cl_ext_atomic_counters_32
#define cl_ext_atomic_counters_32 1
#endif
typedef volatile __global uint* counter32_t;
//just the prototypes, the implementations reside in _atomics.h
uint atomic_inc(counter32_t counter) OVERLOADABLE;
uint atomic_dec(counter32_t counter) OVERLOADABLE;

/*
 * Integer dot products
 *
 * Adds functions
 *  int arm_dot(char4 a, char4 b)
 *  uint arm_dot(uchar4 a, uchar4 b)
 *  int arm_dot_acc(char4 a, char4 b, int acc)
 *  uint arm_dot_acc(uchar4 a, uchar4 b, uint acc)
 *  int arm_dot_acc(short2 a, short2 b, int acc)
 *  uint arm_dot_acc(ushort2 a, ushort2 b, uint acc)
 * calculate integer dot product (and additionally adds the scalar value)
 *
 * See https://www.khronos.org/registry/OpenCL/extensions/arm/cl_arm_integer_dot_product.txt
 */
#ifndef cl_arm_integer_dot_product_int8
#define cl_arm_integer_dot_product_int8 1
#endif
#ifndef cl_arm_integer_dot_product_accumulate_int8
#define cl_arm_integer_dot_product_accumulate_int8 1
#endif
#ifndef cl_arm_integer_dot_product_accumulate_int16
#define cl_arm_integer_dot_product_accumulate_int16 1
#endif

// prototypes to prevent warnings
int arm_dot(char4 a, char4 b)  OVERLOADABLE;
uint arm_dot(uchar4 a, uchar4 b)  OVERLOADABLE;
int arm_dot_acc(char4 a, char4 b, int acc) OVERLOADABLE;
uint arm_dot_acc(uchar4 a, uchar4 b, uint acc) OVERLOADABLE;
int arm_dot_acc(short2 a, short2 b, int acc) OVERLOADABLE;
uint arm_dot_acc(ushort2 a, ushort2 b, uint acc) OVERLOADABLE;

/**
 * (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w)
 */
int arm_dot(char4 a, char4 b)  OVERLOADABLE CONST
{
	int4 tmp = vc4cl_mul24(a, b, VC4CL_SIGNED);
	return tmp.s0 + tmp.s1 + tmp.s2 + tmp.s3;
}
uint arm_dot(uchar4 a, uchar4 b)  OVERLOADABLE CONST
{
	uint4 tmp = vc4cl_mul24(a, b, VC4CL_UNSIGNED);
	return tmp.s0 + tmp.s1 + tmp.s2 + tmp.s3;
}

/**
 * acc + [ (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w) ]
 */
int arm_dot_acc(char4 a, char4 b, int acc) OVERLOADABLE CONST
{
	int4 tmp = vc4cl_mul24(a, b, VC4CL_SIGNED);
	return acc + tmp.s0 + tmp.s1 + tmp.s2 + tmp.s3;
}

uint arm_dot_acc(uchar4 a, uchar4 b, uint acc) OVERLOADABLE CONST
{
	uint4 tmp = vc4cl_mul24(a, b, VC4CL_UNSIGNED);
	return acc + tmp.s0 + tmp.s1 + tmp.s2 + tmp.s3;
}

/**
 * acc + [ (a.x * b.x) + (a.y * b.y) ]
 */
int arm_dot_acc(short2 a, short2 b, int acc) OVERLOADABLE CONST
{
	int2 tmp = vc4cl_mul24(a, b, VC4CL_SIGNED);
	return acc + tmp.s0 + tmp.s1;
}

uint arm_dot_acc(ushort2 a, ushort2 b, uint acc) OVERLOADABLE CONST
{
	uint2 tmp = vc4cl_mul24(a, b, VC4CL_UNSIGNED);
	return acc + tmp.s0 + tmp.s1;
}

#endif /* VC4CL_EXTENSIONS_H */

