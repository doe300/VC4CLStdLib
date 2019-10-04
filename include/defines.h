/*
 * Author: doe300
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef VC4CL_DEFINES_H
#define VC4CL_DEFINES_H

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
#ifndef CL_VERSION_2_2
#define CL_VERSION_2_2 220
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
#ifndef cl_khr_initialize_memory
#define cl_khr_initialize_memory
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
#undef cl_intel_packed_yuv
#endif

// additional supported extensions (need to set flag here, since the module is loaded too late)
#define cl_nv_pragma_unroll 1
#define cl_arm_core_id 1
#define cl_ext_atomic_counters_32 1
#define cl_arm_integer_dot_product_int8 1
#define cl_arm_integer_dot_product_accumulate_int8 1
#define cl_arm_integer_dot_product_accumulate_int16 1
#define cl_arm_integer_dot_product_accumulate_saturate_int8 1

// unsupported extensions or optional core features
#undef cl_khr_fp16
#undef cl_khr_fp64
#undef cl_khr_int64_base_atomics
#undef cl_khr_int64_extended_atomics
#undef cl_khr_depth_images
#undef cl_khr_gl_depth_images
#undef cl_khr_gl_msaa_sharing
#undef cl_amd_media_ops
#undef cl_amd_media_ops2
// unsupported host-only extensions (disable for safety)
#undef cl_khr_gl_sharing
#undef cl_khr_gl_event
#undef cl_khr_d3d10_sharing
#undef cl_khr_dx9_media_sharing
#undef cl_khr_d3d11_sharing
#undef cl_khr_image2d_from_buffer
#undef cl_khr_terminate_context
#undef cl_khr_egl_image
#undef cl_khr_egl_event

#endif /* VC4CL_DEFINES_H */