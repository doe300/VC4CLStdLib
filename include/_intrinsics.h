/* Declares interfaces for all intrinsic functions
 * 
 * Author: doe300
 *
 * See the file "LICENSE" for the full license governing this code.
 */


#ifndef VC4CL_INTRINSICS_H
#define VC4CL_INTRINSICS_H

#include "_overloads.h"

#define VC4CL_SIGNED 0
#define VC4CL_UNSIGNED 1

/*
 * ALU operations
 */
OVERLOAD_2(float, vc4cl_fmax, float, x, float, y)
OVERLOAD_2(float, vc4cl_fmin, float, x, float, y)
OVERLOAD_2(float, vc4cl_fmaxabs, float, x, float, y)
OVERLOAD_2(float, vc4cl_fminabs, float, x, float, y)
OVERLOAD_1(int, vc4cl_ftoi, float, val)
OVERLOAD_1(float, vc4cl_itof, int, val)

OVERLOAD_2(int, vc4cl_asr, uint, val, int, offset)
OVERLOAD_2(int, vc4cl_asr, int, val, int, offset)
OVERLOAD_2(uint, vc4cl_ror, uint, val, int, offset)
OVERLOAD_2(int, vc4cl_ror, int, val, int, offset)
OVERLOAD_3_SCALAR(int, vc4cl_min, int, x, int, y, uchar, sign)
OVERLOAD_3_SCALAR(uint, vc4cl_min, uint, x, uint, y, uchar, sign)
OVERLOAD_3_SCALAR(int, vc4cl_max, int, x, int, y, uchar, sign)
OVERLOAD_3_SCALAR(uint, vc4cl_max, uint, x, uint, y, uchar, sign)
OVERLOAD_2(uint, vc4cl_and, uchar, x, uchar, y)
OVERLOAD_2(int, vc4cl_and, char, x, char, y)
OVERLOAD_2(uint, vc4cl_and, ushort, x, ushort, y)
OVERLOAD_2(int, vc4cl_and, short, x, short, y)
SIMPLE_2(uint, vc4cl_and, uint, x, uint, y, x & y)
SIMPLE_2(int, vc4cl_and, int, x, int, y, x & y)
OVERLOAD_1(uint, vc4cl_clz, uint, val)
OVERLOAD_1(int, vc4cl_clz, int, val)

OVERLOAD_3_SCALAR(uint, vc4cl_mul24, uchar, x, uchar, y, uchar, sign)
OVERLOAD_3_SCALAR(int, vc4cl_mul24, char, x, char, y, uchar, sign)
OVERLOAD_3_SCALAR(uint, vc4cl_mul24, ushort, x, ushort, y, uchar, sign)
OVERLOAD_3_SCALAR(int, vc4cl_mul24, short, x, short, y, uchar, sign)
OVERLOAD_3_SCALAR(uint, vc4cl_mul24, uint, x, uint, y, uchar, sign)
OVERLOAD_3_SCALAR(int, vc4cl_mul24, int, x, int, y, uchar, sign)

/*
 * Pack/unpack modes
 */
//TODO ALU needs to consume float for this to work
//unpacks half to float (UNPACK 1: 16a -> 32)
//OVERLOAD_1(float, vc4cl_unpack_half, half, val)
//sign-extends short to int (UNPACK 1: 16a -> 32)
OVERLOAD_1(int, vc4cl_unpack_sext, short, val)
//unpacks first byte [0, 1] to float (UNPACK 4: 8a -> 32)
OVERLOAD_1(float, vc4cl_unpack_color_byte0, uchar, val)
//unpacks second byte [0, 1] to float (UNPACK 5: 8b -> 32)
OVERLOAD_1(float, vc4cl_unpack_color_byte1, uchar, val)
//unpacks third byte [0, 1] to float (UNPACK 6: 8c -> 32)
OVERLOAD_1(float, vc4cl_unpack_color_byte2, uchar, val)
//unpacks fourth byte [0, 1] to float (UNPACK 7: 8d -> 32)
OVERLOAD_1(float, vc4cl_unpack_color_byte3, uchar, val)
//zero-extend first byte to uint (UNPACK 4: 8a -> 32)
OVERLOAD_1(uint, vc4cl_unpack_byte0, uchar, val)
//zero-extend second byte to uint (UNPACK 5: 8b -> 32)
OVERLOAD_1(uint, vc4cl_unpack_byte1, uchar, val)
//zero-extend third byte to uint (UNPACK 6: 8c -> 32)
OVERLOAD_1(uint, vc4cl_unpack_byte2, uchar, val)
//zero-extend fourth byte to uint (UNPACK 7: 8d -> 32)
OVERLOAD_1(uint, vc4cl_unpack_byte3, uchar, val)

//TODO ALU needs to consume float for this to work
//packs float into half (PACK 1: 32 -> 16a)
//OVERLOAD_1(half, vc4cl_pack_half, float, val)
//converts to unsigned 16-bit integer, truncates the result (PACK 1: 32 -> 16a)
OVERLOAD_1(ushort, vc4cl_pack_truncate, int, val)
OVERLOAD_1(ushort, vc4cl_pack_truncate, uint, val)
//replicates the LSB into all four bytes (PACK 3: 32 -> 8888)
OVERLOAD_1(uint, vc4cl_replicate_lsb, char, val)
OVERLOAD_1(uint, vc4cl_replicate_lsb, uchar, val)
OVERLOAD_1(uint, vc4cl_replicate_lsb, uint, val)
//takes the LSB and writes it into LSB (PACK 4: 32 -> 8a)
OVERLOAD_1(uchar, vc4cl_pack_lsb, char, val)
OVERLOAD_1(uchar, vc4cl_pack_lsb, uchar, val)
OVERLOAD_1(uchar, vc4cl_pack_lsb, uint, val)
//calculates addition, but saturates the result afterwards (given the overflow/carry flag of addition) (uses PACK 8: 32 -> 32)
OVERLOAD_2(int, vc4cl_saturated_add, int, x, int, y)
//calculates subtraction, but saturates the result afterwards (given the overflow/carry flag of substraction) (uses PACK 8: 32 -> 32)
OVERLOAD_2(int, vc4cl_saturated_sub, int, x, int, y)
//converts to signed 16-bit integer, saturates the result (PACK 9: 32 -> 16a)
OVERLOAD_1(short, vc4cl_saturate_short, int, val)
//saturates to unsigned byte (PACK 12:  32 -> 8a)
OVERLOAD_1(uchar, vc4cl_saturate_lsb, uint, val)


/*
 * SFU calls
 */
OVERLOAD_1(float, vc4cl_sfu_recip, float, val)
OVERLOAD_1(float, vc4cl_sfu_rsqrt, float, val)
OVERLOAD_1(float, vc4cl_sfu_log2, float, val)
OVERLOAD_1(float, vc4cl_sfu_exp2, float, val)

/*
 * Periphery access
 */
void vc4cl_semaphore_increment(uchar semaphore);
void vc4cl_semaphore_decrement(uchar semaphore);
void vc4cl_mutex_lock(void);
void vc4cl_mutex_unlock(void);
//read DMA without locking the mutex
OVERLOAD_1(int, vc4cl_dma_read, volatile __global int, * ptr)
OVERLOAD_1(uint, vc4cl_dma_read, volatile __global uint, * ptr)
OVERLOAD_1(float, vc4cl_dma_read, volatile __global float, * ptr)
OVERLOAD_1(int, vc4cl_dma_read, volatile __local int, * ptr)
OVERLOAD_1(uint, vc4cl_dma_read, volatile __local uint, * ptr)
OVERLOAD_1(float, vc4cl_dma_read, volatile __local float, * ptr)
//write DMA without locking the mutex
OVERLOAD_2_RETURN_SCALAR(void, vc4cl_dma_write, volatile __global int, * ptr, int, val)
OVERLOAD_2_RETURN_SCALAR(void, vc4cl_dma_write, volatile __global uint, * ptr, uint, val)
OVERLOAD_2_RETURN_SCALAR(void, vc4cl_dma_write, volatile __global float, * ptr, float, val)
OVERLOAD_2_RETURN_SCALAR(void, vc4cl_dma_write, volatile __local int, * ptr, int, val)
OVERLOAD_2_RETURN_SCALAR(void, vc4cl_dma_write, volatile __local uint, * ptr, uint, val)
OVERLOAD_2_RETURN_SCALAR(void, vc4cl_dma_write, volatile __local float, * ptr, float, val)
//copy DMA without locking the mutex
OVERLOAD_3_SCALAR(int, vc4cl_dma_copy, __global uchar, *dest, const __local uchar, *src, size_t, num_elements)
OVERLOAD_3_SCALAR(int, vc4cl_dma_copy, __global char, *dest, const __local char, *src, size_t, num_elements)
OVERLOAD_3_SCALAR(int, vc4cl_dma_copy, __global ushort, *dest, const __local ushort, *src, size_t, num_elements)
OVERLOAD_3_SCALAR(int, vc4cl_dma_copy, __global short, *dest, const __local short, *src, size_t, num_elements)
OVERLOAD_3_SCALAR(int, vc4cl_dma_copy, __global uint, *dest, const __local uint, *src, size_t, num_elements)
OVERLOAD_3_SCALAR(int, vc4cl_dma_copy, __global int, *dest, const __local int, *src, size_t, num_elements)
OVERLOAD_3_SCALAR(int, vc4cl_dma_copy, __global float, *dest, const __local float, *src, size_t, num_elements)
OVERLOAD_3_SCALAR(int, vc4cl_dma_copy, __local uchar, *dest, const __global uchar, *src, size_t, num_elements)
OVERLOAD_3_SCALAR(int, vc4cl_dma_copy, __local char, *dest, const __global char, *src, size_t, num_elements)
OVERLOAD_3_SCALAR(int, vc4cl_dma_copy, __local ushort, *dest, const __global ushort, *src, size_t, num_elements)
OVERLOAD_3_SCALAR(int, vc4cl_dma_copy, __local short, *dest, const __global short, *src, size_t, num_elements)
OVERLOAD_3_SCALAR(int, vc4cl_dma_copy, __local uint, *dest, const __global uint, *src, size_t, num_elements)
OVERLOAD_3_SCALAR(int, vc4cl_dma_copy, __local int, *dest, const __global int, *src, size_t, num_elements)
OVERLOAD_3_SCALAR(int, vc4cl_dma_copy, __local float, *dest, const __global float, *src, size_t, num_elements)
//load into VPM without locking the mutex
OVERLOAD_2_SCALAR_RETURN_SCALAR(void, vc4cl_prefetch, const __global uchar, *ptr, size_t, num_elements)
OVERLOAD_2_SCALAR_RETURN_SCALAR(void, vc4cl_prefetch, const __global char, *ptr, size_t, num_elements)
OVERLOAD_2_SCALAR_RETURN_SCALAR(void, vc4cl_prefetch, const __global ushort, *ptr, size_t, num_elements)
OVERLOAD_2_SCALAR_RETURN_SCALAR(void, vc4cl_prefetch, const __global short, *ptr, size_t, num_elements)
OVERLOAD_2_SCALAR_RETURN_SCALAR(void, vc4cl_prefetch, const __global uint, *ptr, size_t, num_elements)
OVERLOAD_2_SCALAR_RETURN_SCALAR(void, vc4cl_prefetch, const __global int, *ptr, size_t, num_elements)
OVERLOAD_2_SCALAR_RETURN_SCALAR(void, vc4cl_prefetch, const __global float, *ptr, size_t, num_elements)
//load 32-bit scalar value(s) via TMU
int vc4cl_tmu_read(__global int* ptr) OVERLOADABLE;
int vc4cl_tmu_read(__local int* ptr) OVERLOADABLE;
int2 vc4cl_tmu_read(__global int* ptr0, __global int* ptr1) OVERLOADABLE;
int2 vc4cl_tmu_read(__local int* ptr0, __local int* ptr1) OVERLOADABLE;
int3 vc4cl_tmu_read(__global int* ptr0, __global int* ptr1, __global int* ptr2) OVERLOADABLE;
int3 vc4cl_tmu_read(__local int* ptr0, __local int* ptr1, __local int* ptr2) OVERLOADABLE;
int4 vc4cl_tmu_read(__global int* ptr0, __global int* ptr1, __global int* ptr2, __global int* ptr3) OVERLOADABLE;
int4 vc4cl_tmu_read(__local int* ptr0, __local int* ptr1, __local int* ptr2, __local int* ptr3) OVERLOADABLE;
int8 vc4cl_tmu_read(__global int* ptr0, __global int* ptr1, __global int* ptr2, __global int* ptr3, __global int* ptr4, __global int* ptr5, __global int* ptr6, __global int* ptr7) OVERLOADABLE;
int8 vc4cl_tmu_read(__local int* ptr0, __local int* ptr1, __local int* ptr2, __local int* ptr3, __local int* ptr4, __local int* ptr5, __local int* ptr6, __local int* ptr7) OVERLOADABLE;
int16 vc4cl_tmu_read(__global int* ptr0, __global int* ptr1, __global int* ptr2, __global int* ptr3, __global int* ptr4, __global int* ptr5, __global int* ptr6, __global int* ptr7, __global int* ptr8, __global int* ptr9, __global int* ptr10, __global int* ptr11, __global int* ptr12, __global int* ptr13, __global int* ptr14, __global int* ptr15) OVERLOADABLE;
int16 vc4cl_tmu_read(__local int* ptr0, __local int* ptr1, __local int* ptr2, __local int* ptr3, __local int* ptr4, __local int* ptr5, __local int* ptr6, __local int* ptr7, __local int* ptr8, __local int* ptr9, __local int* ptr10, __local int* ptr11, __local int* ptr12, __local int* ptr13, __local int* ptr14, __local int* ptr15) OVERLOADABLE;

/*
 * Work-item functions
 * Mapped to UNIFORM reads
 */
PURE uchar vc4cl_work_dimensions(void);
PURE uchar vc4cl_local_size(uint dim);
PURE uchar vc4cl_local_id(uint dim);
PURE uint vc4cl_num_groups(uint dim);
PURE uint vc4cl_group_id(uint dim);
PURE uint vc4cl_global_offset(uint dim);
PURE uint vc4cl_global_size(uint dim);
PURE uint vc4cl_global_id(uint dim);

/*
 * Image functions
 * In CLang, read_only and write_only image-types are separate types.
 * Also in CLang, OpenCL image-types are built-in opaque types
 */
#ifdef __IMAGE_SUPPORT__
/*
 * Texture Config Parameter 0
 * Broadcom specification, table 15
 *
 * 0 - 3   | 4 bits  | Number of mipmap levels minus 1
 * 4 - 7   | 4 bits  | texture data type (high bit is on config parameter 1)
 * 8       | 1 bit   | flip texture Y axis
 * 9       | 1 bit   | cube map mode
 * 10 - 11 | 2 bits  | cache swizzle
 * 12 - 31 | 20 bits | texture base pointer (multiple of 4KB)
 */
OVERLOAD_ALL_IMAGE_TYPES(CONST uint, vc4cl_image_basic_setup)
/*
 * Texture Config Parameter 1
 * Broadcom specification, table 16
 *
 * 0 - 1   | 2 bits  | S (x-coord) wrap mode (0 = repeat, 1 = clamp, 2 = mirror, 3 = border)
 * 2 - 3   | 2 bits  | T (y-coord) wrap mode (0 = repeat, 1 = clamp, 2 = mirror, 3 = border)
 * 4 - 6   | 3 bits  | minification filter (interpolation)
 * 7       | 1 bit   | magnification filter
 * 8 - 18  | 11 bits | image width (0 = 2048)
 * 19      | 1 bit   | flip ETC Y (per block)
 * 20 - 30 | 11 bits | image height (0 = 248)
 * 31      | 1 bit   | high bit of texture type
 */
OVERLOAD_ALL_IMAGE_TYPES(CONST uint, vc4cl_image_access_setup)
/*
 * Texture Config Parameters 2 and 3
 * Broadcom specification, table 17
 *
 * Cube map stride:
 * 0       | 1 bit   | disable automatic LOD, use bias only
 * 12 - 29 | 18 bits | cube map stride (in multiples of 4KB)
 * 30 - 31 | 2 bits  | value 1 for cube map stride
 *
 * Child image dimensions:
 * 0 - 10  | 11 bits | child image width (0 = 2048, does not work, see errata HW-2753)
 * 12 - 22 | 11 bits | child image height (0 = 2048, does not work, see errata HW-2753)
 * 30 - 31 | 2 bits  | value 2 for child image dimensions
 *
 * Child image offsets:
 * 0 - 10  | 11 bits | child image X offset
 * 12 - 22 | 11 bits | child image Y offset
 * 30 - 31 | 2 bits  | value 3 for child image offsets
 */
OVERLOAD_ALL_IMAGE_TYPES(CONST uint, vc4cl_image_extended_setup)
/*
 * To apply a sampler to an image, we need to override the image-access setup UNIFORM before a read with the magnification/minification filters and wrap modes to use
 */
OVERLOAD_ALL_IMAGE_TYPES_1(void, vc4cl_set_image_access_setup, uint, val)
CONST uint vc4cl_sampler_get_normalized_coords(sampler_t sampler);
CONST uint vc4cl_sampler_get_addressing_mode(sampler_t sampler);
CONST uint vc4cl_sampler_get_filter_mode(sampler_t sampler);
/*
 * Image read functions
 *
 * The coordinates need to be floating-values in the range [0, 1] and are scaled to the width/height of the image.
 * The returned data is not necessarily <4 x int32>, but up to 4 components with up to 32 bits each, loaded according to the byte-sizes and number of components specified in the channel_type_size and channel_order_size.
 *
 * So, this functions return the data in the native format (as stored in the image-buffer), but correctly distributed across the 4 components.
 */
int4 vc4cl_image_read(read_only image1d_t image, float coords, uint channel_type_size, uint channel_order_size) OVERLOADABLE;
int4 vc4cl_image_read(read_only image1d_buffer_t image, float coords, uint channel_type_size, uint channel_order_size) OVERLOADABLE;
int4 vc4cl_image_read(read_only image1d_array_t image, float coords, int imageIndex, uint channel_type_size, uint channel_order_size) OVERLOADABLE;
int4 vc4cl_image_read(read_only image2d_t image, float2 coords, uint channel_type_size, uint channel_order_size) OVERLOADABLE;
int4 vc4cl_image_read(read_only image2d_array_t image, float2 coords, int imageIndex, uint channel_type_size, uint channel_order_size) OVERLOADABLE;
int4 vc4cl_image_read(read_only image3d_t image, float4 coords, uint channel_type_size, uint channel_order_size) OVERLOADABLE;
#endif

/*
 * Type conversions
 */
//component-wise bitcasts
OVERLOAD_1(uchar, vc4cl_bitcast_uchar, uint, val)
OVERLOAD_1(uchar, vc4cl_bitcast_uchar, int, val)
OVERLOAD_1(char, vc4cl_bitcast_char, uint, val)
OVERLOAD_1(char, vc4cl_bitcast_char, int, val)
OVERLOAD_1(ushort, vc4cl_bitcast_ushort, uint, val)
OVERLOAD_1(ushort, vc4cl_bitcast_ushort, int, val)
OVERLOAD_1(short, vc4cl_bitcast_short, uint, val)
OVERLOAD_1(short, vc4cl_bitcast_short, int, val)
SIMPLE_1(uint, vc4cl_bitcast_uint, uint, val, val)
OVERLOAD_1(uint, vc4cl_bitcast_uint, int, val)
OVERLOAD_1(int, vc4cl_bitcast_int, uint, val)
SIMPLE_1(int, vc4cl_bitcast_int, int, val, val)

OVERLOAD_1(uint, vc4cl_bitcast_uint, float, val)
OVERLOAD_1(float, vc4cl_bitcast_float, uint, val)
OVERLOAD_1(int, vc4cl_bitcast_int, float, val)
OVERLOAD_1(float, vc4cl_bitcast_float, int, val)

SIMPLE_1(int, vc4cl_sign_extend, char, val, vc4cl_asr(vc4cl_and(val, (arg_t)0xFF) << 24, 24))
//SIMPLE_1(int, vc4cl_sign_extend, short, val, vc4cl_asr(vc4cl_and(val, (arg_t)0xFFFF) << 16, 16))
SIMPLE_1(int, vc4cl_sign_extend, short, val, vc4cl_unpack_sext(val))

SIMPLE_1(uint, vc4cl_zero_extend, uchar, val, vc4cl_and(val, (arg_t) (0xFFU)))
SIMPLE_1(uint, vc4cl_zero_extend, ushort, val, vc4cl_and(val, (arg_t) (0xFFFFU)))

SIMPLE_1(uint, vc4cl_extend, uchar, val, vc4cl_zero_extend(val))
SIMPLE_1(int, vc4cl_extend, char, val, vc4cl_sign_extend(val))
SIMPLE_1(uint, vc4cl_extend, ushort, val, vc4cl_zero_extend(val))
SIMPLE_1(int, vc4cl_extend, short, val, vc4cl_sign_extend(val))
SIMPLE_1(uint, vc4cl_extend, uint, val, val)
SIMPLE_1(int, vc4cl_extend, int, val, val)

/*
 * Other functions
 */
SIMPLE_1(uchar, vc4cl_msb_set, uchar, val, (val >> 7))
SIMPLE_1(char, vc4cl_msb_set, char, val, (val >> 7))
SIMPLE_1(ushort, vc4cl_msb_set, ushort, val, (val >> 15))
SIMPLE_1(short, vc4cl_msb_set, short, val, (val >> 15))
SIMPLE_1(uint, vc4cl_msb_set, uint, val, (val >> 31))
SIMPLE_1(int, vc4cl_msb_set, int, val, (val >> 31))

/*
 * Vector functions
 */
//Rotates the vector-elements according to the offset (-15 .. +15)
//an offset of 5 means rotate up 5 positions (e.g. x.s0 -> y.s5, x.s10 -> y.15, x.s12 -> y.s1
//NOTE: the rotation is always all 16 elements!! So functions with vector-size of less than 16 MUST not use the positions shifted in from the remaining vector-elements
OVERLOAD_2_SCALAR(uchar, vc4cl_vector_rotate, uchar, val, char, offset)
OVERLOAD_2_SCALAR(char, vc4cl_vector_rotate, char, val, char, offset)
OVERLOAD_2_SCALAR(ushort, vc4cl_vector_rotate, ushort, val, char, offset)
OVERLOAD_2_SCALAR(short, vc4cl_vector_rotate, short, val, char, offset)
OVERLOAD_2_SCALAR(uint, vc4cl_vector_rotate, uint, val, char, offset)
OVERLOAD_2_SCALAR(int, vc4cl_vector_rotate, int, val, char, offset)
OVERLOAD_2_SCALAR(float, vc4cl_vector_rotate, float, val, char, offset)

/*
 * For debugging purposes
 */
//The vector element number (0 .. 15)
CONST uchar16 vc4cl_element_number(void);
//the ID of the QPU (the processor)
CONST uchar vc4cl_qpu_number(void);

#endif /* VC4CL_INTRINSICS_H */

