/*
 * Author: doe300
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef VC4CL_CONFIG_H
#define VC4CL_CONFIG_H

#ifndef CL_VERSION_1_0
#define CL_VERSION_1_0 100
#endif
#ifndef CL_VERSION_1_1
#define CL_VERSION_1_1 110
#endif
#ifndef CL_VERSION_1_2
#define CL_VERSION_1_2 120
#endif
#ifndef CL_VERSION_2_0
#define CL_VERSION_2_0 200
#endif
#ifndef CL_VERSION_2_1
#define CL_VERSION_2_1 210
#endif

#undef __OPENCL_VERSION__
#define __OPENCL_VERSION__ CL_VERSION_1_2
#undef __OPENCL_C_VERSION__
#define __OPENCL_C_VERSION__ CL_VERSION_1_2
#ifndef __ENDIAN_LITTLE__
#define __ENDIAN_LITTLE__ 1
#endif
#ifndef __EMBEDDED_PROFILE__
#define __EMBEDDED_PROFILE__ 1
#endif
//#ifndef __IMAGE_SUPPORT__
//#define __IMAGE_SUPPORT__ 1
//#endif
#undef __IMAGE_SUPPORT__

#ifndef cl_khr_global_int32_base_atomics
#define cl_khr_global_int32_base_atomics
#endif
#ifndef cl_khr_local_int32_base_atomics
#define cl_khr_local_int32_base_atomics
#endif
#ifndef cl_khr_global_int32_extended_atomics
#define cl_khr_global_int32_extended_atomics
#endif
#ifndef cl_khr_local_int32_extended_atomics
#define cl_khr_local_int32_extended_atomics
#endif
#ifndef cl_khr_byte_addressable_store
#define cl_khr_byte_addressable_store
#endif
#ifdef __IMAGE_SUPPORT__
#ifndef cl_khr_3d_image_writes
#define cl_khr_3d_image_writes
#endif
#ifndef cl_intel_packed_yuv
#define cl_intel_packed_yuv
#endif
#else
#undef cl_khr_3d_image_writes
#endif

//unsupported extensions or optional core features
#undef cl_khr_fp16
#undef cl_khr_fp64
#undef cl_khr_int64_base_atomics
#undef cl_khr_int64_extended_atomics
#undef cl_khr_depth_images
#undef cl_khr_gl_depth_images
#undef cl_khr_gl_msaa_sharing
//unsupported host-only extensions (disable for safety)
#undef cl_khr_gl_sharing
#undef cl_khr_gl_event
#undef cl_khr_d3d10_sharing
#undef cl_khr_dx9_media_sharing
#undef cl_khr_d3d11_sharing
#undef cl_khr_image2d_from_buffer
#undef cl_khr_initialize_memory
#undef cl_khr_terminate_context
#undef cl_khr_egl_image
#undef cl_khr_egl_event

#include "opencl-c.h"

#ifndef NULL
#define NULL ((void *)0)
#endif

/*
 * Math constants
 */
#define M_LOG210    3.01029995663981195214f /* log_2(10) */
#undef NAN
#define NAN         0x7fc00000U
#undef INF
#define INF			0x7f800000U

#define ALL_BITS_SET 0xFFFFFFFFU

#endif /* VC4CL_CONFIG_H */

