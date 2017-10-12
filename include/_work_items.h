/*
 * Author: doe300
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef VC4CL_WORK_ITEMS_H
#define VC4CL_WORK_ITEMS_H

#include "_overloads.h"
#include "_intrinsics.h"


/*
 * local values are stored in the a UNIFORM in this fashion:
 * | 0 | dim2 | dim1 | dim0 |
 * -> to read value of dimension x, calculate: (UNIFORM >> (dim * 8)) & 0xFF
 *
 * This can be compacted in such way, since for a maximum value of 12, the local ID and size fits into 1 Byte
 */

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

#endif /* VC4CL_WORK_ITEMS_H */

