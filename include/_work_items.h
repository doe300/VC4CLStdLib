/*
 * Author: doe300
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef VC4CL_WORK_ITEMS_H
#define VC4CL_WORK_ITEMS_H

#include "_overloads.h"
#include "_intrinsics.h"

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

/*
 * These functions are actually not part of OpenCL C 1.2, but are introduced in OpenCL C 2.0.
 * Anyway, they are useful helper, we can use for implementing the standard-library functions
 */
/*
 * Returns the work-­items 1­‐dimensional local ID.
 */
INLINE size_t get_local_linear_id() OVERLOADABLE CONST
{
	return vc4cl_mul24(vc4cl_mul24(vc4cl_local_id(2), vc4cl_local_size(1), VC4CL_UNSIGNED), vc4cl_bitcast_uint(vc4cl_local_size(0)), VC4CL_UNSIGNED) + vc4cl_mul24(vc4cl_local_id(1), vc4cl_local_size(0), VC4CL_UNSIGNED) + vc4cl_local_id(0);
}


#endif /* VC4CL_WORK_ITEMS_H */

