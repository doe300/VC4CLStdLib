/*
 * Author: doe300
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef VC4CL_ASYNC_H
#define VC4CL_ASYNC_H

#include "_config.h"
#include "_overloads.h"


/*
 * This is a synchronous/blocking implementation.
 * The copy is "performed by all work-items in a work-group", so any work-item only has to copy a part of the area.
 * Or, since the copying of memory on different QPUs block each other, we can simply only execute the copying on the first work-item
 * (index 0, 0, 0). Idea taken from PoCL
 */

#define ASYNC_COPY_INTERNAL \
		if(vc4cl_local_id(0) == 0) \
		{ \
			vc4cl_mutex_lock(); \
			vc4cl_dma_copy(dst, src, num_elements); \
			vc4cl_mutex_unlock(); \
		}

#define ASYNC_COPY(type) \
		INLINE event_t async_work_group_copy(__local type * dst, const __global type * src, size_t num_elements, event_t event) OVERLOADABLE \
		{ \
			ASYNC_COPY_INTERNAL \
			return event; \
		} \
		INLINE event_t async_work_group_copy(__local type##2 * dst, const __global type##2 * src, size_t num_elements, event_t event) OVERLOADABLE \
		{ \
			ASYNC_COPY_INTERNAL \
			return event; \
		} \
		INLINE event_t async_work_group_copy(__local type##3 * dst, const __global type##3 * src, size_t num_elements, event_t event) OVERLOADABLE \
		{ \
			ASYNC_COPY_INTERNAL \
			return event; \
		} \
		INLINE event_t async_work_group_copy(__local type##4 * dst, const __global type##4 * src, size_t num_elements, event_t event) OVERLOADABLE \
		{ \
			ASYNC_COPY_INTERNAL \
			return event; \
		} \
		INLINE event_t async_work_group_copy(__local type##8 * dst, const __global type##8 * src, size_t num_elements, event_t event) OVERLOADABLE \
		{ \
			ASYNC_COPY_INTERNAL \
			return event; \
		} \
		INLINE event_t async_work_group_copy(__local type##16 * dst, const __global type##16 * src, size_t num_elements, event_t event) OVERLOADABLE \
		{ \
			ASYNC_COPY_INTERNAL \
			return event; \
		} \
		INLINE event_t async_work_group_copy(__global type * dst, const __local type * src, size_t num_elements, event_t event) OVERLOADABLE \
		{ \
			ASYNC_COPY_INTERNAL \
			return event; \
		} \
		INLINE event_t async_work_group_copy(__global type##2 * dst, const __local type##2 * src, size_t num_elements, event_t event) OVERLOADABLE \
		{ \
			ASYNC_COPY_INTERNAL \
			return event; \
		} \
		INLINE event_t async_work_group_copy(__global type##3 * dst, const __local type##3 * src, size_t num_elements, event_t event) OVERLOADABLE \
		{ \
			ASYNC_COPY_INTERNAL \
			return event; \
		} \
		INLINE event_t async_work_group_copy(__global type##4 * dst, const __local type##4 * src, size_t num_elements, event_t event) OVERLOADABLE \
		{ \
			ASYNC_COPY_INTERNAL \
			return event; \
		} \
		INLINE event_t async_work_group_copy(__global type##8 * dst, const __local type##8 * src, size_t num_elements, event_t event) OVERLOADABLE \
		{ \
			ASYNC_COPY_INTERNAL \
			return event; \
		} \
		INLINE event_t async_work_group_copy(__global type##16 * dst, const __local type##16 * src, size_t num_elements, event_t event) OVERLOADABLE \
		{ \
			ASYNC_COPY_INTERNAL \
			return event; \
		}

#define ASYNC_STRIDED_SOURCE_COPY_INTERNAL \
		for (size_t i = 0; i < num_elements; ++i) \
		dst[i] = src[i * src_stride];
//TODO better way, e.g. via vc4cl_dma_copy and stride-parameter?

#define ASYNC_STRIDED_DEST_COPY_INTERNAL \
		for (size_t i = 0; i < num_elements; ++i) \
		dst[i * dst_stride] = src[i];

#define ASYNC_STRIDED_COPY(type) \
		INLINE event_t async_work_group_strided_copy(__local type * dst, const __global type * src, size_t num_elements, size_t src_stride, event_t event) OVERLOADABLE \
		{ \
			ASYNC_STRIDED_SOURCE_COPY_INTERNAL \
			return event; \
		} \
		INLINE event_t async_work_group_strided_copy(__local type##2 * dst, const __global type##2 * src, size_t num_elements, size_t src_stride, event_t event) OVERLOADABLE \
		{ \
			ASYNC_STRIDED_SOURCE_COPY_INTERNAL \
			return event; \
		} \
		INLINE event_t async_work_group_strided_copy(__local type##3 * dst, const __global type##3 * src, size_t num_elements, size_t src_stride, event_t event) OVERLOADABLE \
		{ \
			ASYNC_STRIDED_SOURCE_COPY_INTERNAL \
			return event; \
		} \
		INLINE event_t async_work_group_strided_copy(__local type##4 * dst, const __global type##4 * src, size_t num_elements, size_t src_stride, event_t event) OVERLOADABLE \
		{ \
			ASYNC_STRIDED_SOURCE_COPY_INTERNAL \
			return event; \
		} \
		INLINE event_t async_work_group_strided_copy(__local type##8 * dst, const __global type##8 * src, size_t num_elements, size_t src_stride, event_t event) OVERLOADABLE \
		{ \
			ASYNC_STRIDED_SOURCE_COPY_INTERNAL \
			return event; \
		} \
		INLINE event_t async_work_group_strided_copy(__local type##16 * dst, const __global type##16 * src, size_t num_elements, size_t src_stride, event_t event) OVERLOADABLE \
		{ \
			ASYNC_STRIDED_SOURCE_COPY_INTERNAL \
			return event; \
		} \
		INLINE event_t async_work_group_strided_copy(__global type * dst, const __local type * src, size_t num_elements, size_t dst_stride, event_t event) OVERLOADABLE \
		{ \
			ASYNC_STRIDED_DEST_COPY_INTERNAL \
			return event; \
		} \
		INLINE event_t async_work_group_strided_copy(__global type##2 * dst, const __local type##2 * src, size_t num_elements, size_t dst_stride, event_t event) OVERLOADABLE \
		{ \
			ASYNC_STRIDED_DEST_COPY_INTERNAL \
			return event; \
		} \
		INLINE event_t async_work_group_strided_copy(__global type##3 * dst, const __local type##3 * src, size_t num_elements, size_t dst_stride, event_t event) OVERLOADABLE \
		{ \
			ASYNC_STRIDED_DEST_COPY_INTERNAL \
			return event; \
		} \
		INLINE event_t async_work_group_strided_copy(__global type##4 * dst, const __local type##4 * src, size_t num_elements, size_t dst_stride, event_t event) OVERLOADABLE \
		{ \
			ASYNC_STRIDED_DEST_COPY_INTERNAL \
			return event; \
		} \
		INLINE event_t async_work_group_strided_copy(__global type##8 * dst, const __local type##8 * src, size_t num_elements, size_t dst_stride, event_t event) OVERLOADABLE \
		{ \
			ASYNC_STRIDED_DEST_COPY_INTERNAL \
			return event; \
		} \
		INLINE event_t async_work_group_strided_copy(__global type##16 * dst, const __local type##16 * src, size_t num_elements, size_t dst_stride, event_t event) OVERLOADABLE \
		{ \
			ASYNC_STRIDED_DEST_COPY_INTERNAL \
			return event; \
		}

#define PREFETCH(type) \
		INLINE void prefetch(const __global type * ptr, size_t num_entries) OVERLOADABLE \
		{ \
			vc4cl_prefetch(ptr, num_entries); \
		} \
		INLINE void prefetch(const __global type##2 * ptr, size_t num_entries) OVERLOADABLE \
		{ \
			vc4cl_prefetch(ptr, num_entries); \
		} \
		INLINE void prefetch(const __global type##3 * ptr, size_t num_entries) OVERLOADABLE \
		{ \
			vc4cl_prefetch(ptr, num_entries); \
		} \
		INLINE void prefetch(const __global type##4 * ptr, size_t num_entries) OVERLOADABLE \
		{ \
			vc4cl_prefetch(ptr, num_entries); \
		} \
		INLINE void prefetch(const __global type##8 * ptr, size_t num_entries) OVERLOADABLE \
		{ \
			vc4cl_prefetch(ptr, num_entries); \
		} \
		INLINE void prefetch(const __global type##16 * ptr, size_t num_entries) OVERLOADABLE \
		{ \
			vc4cl_prefetch(ptr, num_entries); \
		}

/*
 * OpenCL 1.2, page 278:
 * "Perform an async copy of num_gentypes gentype elements from src to dst.
 *  The async copy is performed by all work-items in a work-group and this built-in
 *  function must therefore be encountered by all work-items in a work-group executing the kernel with the same argument values."
 */
ASYNC_COPY(uchar)
ASYNC_COPY(char)
ASYNC_COPY(ushort)
ASYNC_COPY(short)
ASYNC_COPY(uint)
ASYNC_COPY(int)
ASYNC_COPY(float)

ASYNC_STRIDED_COPY(uchar)
ASYNC_STRIDED_COPY(char)
ASYNC_STRIDED_COPY(ushort)
ASYNC_STRIDED_COPY(short)
ASYNC_STRIDED_COPY(uint)
ASYNC_STRIDED_COPY(int)
ASYNC_STRIDED_COPY(float)

/*
 * OpenCL 1.2, page 279:
 * "Wait for events that identify the async_work_group_copy operations to complete.
 *  The event objects specified in event_list will be released after the wait is performed."
 */
INLINE void wait_group_events(int num_events, event_t* event_list) OVERLOADABLE
{
	// async_work_group_copy is blocking, so we don't need to wait for any asynchronous operation to finish
	// But: Since the copy is only performed on the first work-item, we need to wait for it to finish
	barrier(CLK_GLOBAL_MEM_FENCE);
}

/*
 * OpenCL 1.2, page 280:
 * "Prefetch num_gentypes * sizeof(gentype) bytes into the global cache.
 * The prefetch instruction is applied to a work-item in a work-group and does not affect the functional behavior of the kernel."
 *
 * -> Since it doesn't affect the functional behavior, the implementation is a no-op
 */
PREFETCH(uchar)
PREFETCH(char)
PREFETCH(ushort)
PREFETCH(short)
PREFETCH(uint)
PREFETCH(int)
PREFETCH(float)

#endif /* VC4CL_ASYNC_H */

