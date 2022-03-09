/*
 * Author: doe300
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef VC4CL_WORK_ITEMS_H
#define VC4CL_WORK_ITEMS_H

#include "_intrinsics.h"
#include "_overloads.h"

INLINE uint get_work_dim(void) OVERLOADABLE CONST
{
	return vc4cl_work_dimensions();
}

INLINE size_t get_global_size(uint dim) OVERLOADABLE CONST
{
	return vc4cl_global_size(dim);
}

INLINE size_t get_global_id(uint dim) OVERLOADABLE CONST
{
	return vc4cl_global_id(dim);
}

INLINE size_t get_local_size(uint dim) OVERLOADABLE CONST
{
	return vc4cl_local_size(dim);
}

INLINE size_t get_enqueued_local_size(uint dimindx) OVERLOADABLE CONST
{
	// "Returns the same value as that returned by get_local_size(dimindx) if the kernel is executed with a uniform
	// work-group size."
	return vc4cl_local_size(dimindx);
}

INLINE size_t get_local_id(uint dim) OVERLOADABLE CONST
{
	return vc4cl_local_id(dim);
}

INLINE size_t get_num_groups(uint dim) OVERLOADABLE CONST
{
	return vc4cl_num_groups(dim);
}

INLINE size_t get_group_id(uint dim) OVERLOADABLE CONST
{
	return vc4cl_group_id(dim);
}

INLINE size_t get_global_offset(uint dim) OVERLOADABLE CONST
{
	return vc4cl_global_offset(dim);
}

INLINE size_t get_global_linear_id() OVERLOADABLE CONST
{
	return vc4cl_global_linear_id();
}

INLINE size_t get_local_linear_id() OVERLOADABLE CONST
{
	return vc4cl_local_linear_id();
}

#endif /* VC4CL_WORK_ITEMS_H */
