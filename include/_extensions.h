/*
 * Author: doe300
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef VC4CL_EXTENSIONS_H
#define VC4CL_EXTENSIONS_H

#include "_config.h"
#include "_overloads.h"


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

#endif /* VC4CL_EXTENSIONS_H */

