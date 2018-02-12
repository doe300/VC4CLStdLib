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
	/*
	 * "All work-items in a work-group executing the kernel on a processor must execute this function
	 *  before any are allowed to continue execution beyond the barrier."
	 *
	 * -> Make sure, any work-item blocks until all are ready
	 *
	 * Barriers are implemented via semaphores.
	 *
	 * At the beginning, all semaphores are assumed to be at the default-value of zero.
	 *
	 * When encountering the barrier-function, do the following:
	 * 1. increase all semaphores for all work-items once
	 * 2. decrease the semaphore for the current work-item as often as there are work-items
	 * -> This makes sure, that every work-item blocks until all other work-items increased its semaphore
	 *
	 * Since the semaphores have 4-bit (16 values) and there is a maximum of 12 work-items, they can't stall because of overflow beyond 15
	 */

	//1. calculate local index and size
	uchar localSize = mul24(mul24(vc4cl_local_size(0), vc4cl_local_size(1)), vc4cl_local_size(2));
	uchar localIndex = mul24(mul24(vc4cl_local_id(2), vc4cl_local_size(1)), vc4cl_local_size(0)) + mul24(vc4cl_local_id(1), vc4cl_local_size(0)) + vc4cl_local_id(0);

	if(localIndex != 0)
	{
		//all but the first work-item increment semaphore 0, so the first work-item can continue, wait on their own semaphore and then increment the next one (if any)
		vc4cl_semaphore_increment((uchar)0);
		switch(localIndex)
		{
			case 1:
				vc4cl_semaphore_decrement((uchar)1);
				if(localSize > 2)
					vc4cl_semaphore_increment((uchar)2);
				break;
			case 2:
				vc4cl_semaphore_decrement((uchar)2);
				if(localSize > 3)
					vc4cl_semaphore_increment((uchar)3);
				break;
			case 3:
				vc4cl_semaphore_decrement((uchar)3);
				if(localSize > 4)
					vc4cl_semaphore_increment((uchar)4);
				break;
			case 4:
				vc4cl_semaphore_decrement((uchar)4);
				if(localSize > 5)
					vc4cl_semaphore_increment((uchar)5);
				break;
			case 5:
				vc4cl_semaphore_decrement((uchar)5);
				if(localSize > 6)
					vc4cl_semaphore_increment((uchar)6);
				break;
			case 6:
				vc4cl_semaphore_decrement((uchar)6);
				if(localSize > 7)
					vc4cl_semaphore_increment((uchar)7);
				break;
			case 7:
				vc4cl_semaphore_decrement((uchar)7);
				if(localSize > 8)
					vc4cl_semaphore_increment((uchar)8);
				break;
			case 8:
				vc4cl_semaphore_decrement((uchar)8);
				if(localSize > 9)
					vc4cl_semaphore_increment((uchar)9);
				break;
			case 9:
				vc4cl_semaphore_decrement((uchar)9);
				if(localSize > 10)
					vc4cl_semaphore_increment((uchar)10);
				break;
			case 10:
				vc4cl_semaphore_decrement((uchar)10);
				if(localSize > 11)
					vc4cl_semaphore_increment((uchar)11);
				break;
			case 11:
				vc4cl_semaphore_decrement((uchar)11);
				break;
		}
	}
	else if(localSize != 1)	//localIndex == 0 and there are more than 1 work-item
	{
		//the first work-item waits on its semaphore for all other to have increased it (size -1 times) and then wakes up the second work-item
		for(uchar i = 0; i < localSize - 1; ++i)
		{
			vc4cl_semaphore_decrement((uchar)0);
		}
		vc4cl_semaphore_increment((uchar)1);
	}
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

