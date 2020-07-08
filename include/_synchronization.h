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

INLINE void read_mem_fence(cl_mem_fence_flags flags) OVERLOADABLE
{
	//read_mem_fence and write_mem_fence are not handled by the SPIR-V front-end, but mem_fence is
	mem_fence(flags);
}

INLINE void write_mem_fence(cl_mem_fence_flags flags) OVERLOADABLE
{
	//read_mem_fence and write_mem_fence are not handled by the SPIR-V front-end, but mem_fence is
	mem_fence(flags);
}
#endif /* VC4CL_SYNCHRONIZATION_H */

