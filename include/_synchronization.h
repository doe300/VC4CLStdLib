/*
 * Author: doe300
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef VC4CL_SYNCHRONIZATION_H
#define VC4CL_SYNCHRONIZATION_H

#include "_config.h"
#include "_work_items.h"

INLINE void barrier(cl_mem_fence_flags flags) OVERLOADABLE
{
	vc4cl_barrier(flags);
}

/*
 * We do not declare read_mem_fence() and write_mem_fence(), since:
 * - The SPIRV-LLVM-Translator (in older versions, e.g. 7.0) can't handle them passing a non-const flags to the mem_fence() function
 * - We anyway handle mem_fence(), read_mem_fence() and write_mem_fence() in both front-ends the exact same way
 */
#endif /* VC4CL_SYNCHRONIZATION_H */

