/*
 * Author: doe300
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef VC4CL_IMAGES_H
#define VC4CL_IMAGES_H

#include "_config.h"
#include "_overloads.h"

#ifdef __IMAGE_SUPPORT__

/*
 * Calculates the byte-size of a single component
 */
INLINE CONST uint vc4cl_get_channel_type_size(int channel_type)
{
	switch(channel_type)
	{
		case CLK_SNORM_INT8:
		case CLK_UNORM_INT8:
		case CLK_SIGNED_INT8:
		case CLK_UNSIGNED_INT8:
			return 1;
		case CLK_SNORM_INT16:
		case CLK_UNORM_INT16:
		case CLK_UNORM_SHORT_565:
		case CLK_UNORM_SHORT_555:
		case CLK_SIGNED_INT16:
		case CLK_UNSIGNED_INT16:
		case CLK_HALF_FLOAT:
			return 2;
		case CLK_UNORM_INT24:
			return 3;
		case CLK_UNORM_INT_101010:
		case CLK_SIGNED_INT32:
		case CLK_UNSIGNED_INT32:
		case CLK_FLOAT:
			return 4;
		default:
			return 0;
	}
}

/*
 * Calculates the number of elements in a single pixel
 */
INLINE CONST uint vc4cl_get_channel_order_size(int channel_order)
{
	switch(channel_order)
	{
		case CLK_R:
		case CLK_A:
		case CLK_INTENSITY:
		case CLK_LUMINANCE:
		case CLK_DEPTH:
			return 1;
		case CLK_RG:
		case CLK_RA:
		case CLK_Rx:
		case CLK_DEPTH_STENCIL:
			return 2;
		case CLK_RGB:
		case CLK_RGx:
			return 3;
		case CLK_RGBA:
		case CLK_BGRA:
		case CLK_ARGB:
		case CLK_RGBx:
			return 4;
		default:
			return 0;
	}
}

INLINE CONST float4 vc4cl_convert_from_image_to_float(int4 color, int channel_type)
{
	//see OpenCL 1.2, page 297:
	/*
	 * read_imagef returns floating-point values in the range [0.0 ... 1.0] for image objects created with image_channel_data_type set to one of the predefined packed formats or
	 *  CL_UNORM_INT8, or CL_UNORM_INT16.
	 *
	 * read_imagef returns floating-point values in the range [-1.0 ... 1.0] for image objects created with image_channel_data_type set to
	 *  CL_SNORM_INT8, or CL_SNORM_INT16.
	 *
	 * read_imagef returns floating-point values for image objects created with image_channel_data_type set to
	 *  CL_HALF_FLOAT or CL_FLOAT.
	 */

	//see OpenCL 1.2, page 336+
	switch(channel_type)
	{
		case CLK_UNORM_INT8:
			return convert_float4(vc4cl_bitcast_uchar(color)) / 255.0f;
		case CLK_UNORM_INT_101010:
			return convert_float4(vc4cl_bitcast_uint(color)) / 1023.0f;
		case CLK_UNORM_INT16:
			return convert_float4(vc4cl_bitcast_ushort(color)) / 65535.0f;
		case CLK_SNORM_INT8:
			return max(-1.0f, convert_float4(vc4cl_bitcast_char(color)) / 127.0f);
		case CLK_SNORM_INT16:
			return max(-1.0f, convert_float4(vc4cl_bitcast_short(color)) / 32767.0f);
		case CLK_HALF_FLOAT:
			//TODO
		case CLK_FLOAT:
			return vc4cl_bitcast_float(color);
		default:
			return 0.0f;
	}
}

INLINE CONST int4 vc4cl_convert_from_image_to_int(int4 color, int channel_type)
{
	//see OpenCL 1.2, page 298:
	/*
	 * read_imagei can only be used with image objects created with image_channel_data_type set to one of the following values:
	 *  CL_SIGNED_INT8, CL_SIGNED_INT16 and CL_SIGNED_INT32."
	 */

	//see OpenCL 1.2, page 340
	switch(channel_type)
	{
		case CLK_SIGNED_INT8:
			return vc4cl_extend(vc4cl_bitcast_char(color));
		case CLK_SIGNED_INT16:
			return vc4cl_extend(vc4cl_bitcast_short(color));
		case CLK_SIGNED_INT32:
			return color;
		default:
			return 0;
	}
}

INLINE CONST uint4 vc4cl_convert_from_image_to_uint(int4 color, int channel_type)
{
	//see OpenCL 1.2, page 298:
	/*
	 * read_imageui can only be used with image objects created with image_channel_data_type set to one of the following values:
	 *  CL_UNSIGNED_INT8, CL_UNSIGNED_INT16 and CL_UNSIGNED_INT32."
	 */

	//see OpenCL 1.2, page 340
	switch(channel_type)
	{
		case CLK_UNSIGNED_INT8:
			return vc4cl_extend(vc4cl_bitcast_uchar(color));
		case CLK_UNSIGNED_INT16:
			return vc4cl_extend(vc4cl_bitcast_ushort(color));
		case CLK_UNSIGNED_INT32:
			return vc4cl_bitcast_uint(color);
		default:
			return 0;
	}
}

INLINE CONST int4 vc4cl_convert_to_image_type(float4 color, int channel_type) OVERLOADABLE
		{
	//see OpenCL 1.2, page 311:
	/*
	 * "write_imagef can only be used with image objects created with image_channel_data_type set to one of the predefined packed formats or set to
	 *  CL_SNORM_INT8, CL_UNORM_INT8, CL_SNORM_INT16, CL_UNORM_INT16, CL_HALF_FLOAT or CL_FLOAT."
	 */
	//see OpenCL 1.2, page 336+
	switch(channel_type)
	{
		case CLK_SNORM_INT8:
			return vc4cl_extend(convert_char4_sat_rte(color * 127.0f));
		case CLK_UNORM_INT8:
			return vc4cl_bitcast_int(vc4cl_extend(convert_uchar4_sat_rte(color * 255.0f)));
		case CLK_SNORM_INT16:
			return vc4cl_extend(convert_short4_sat_rte(color * 32767.0f));
		case CLK_UNORM_INT16:
			return vc4cl_bitcast_int(vc4cl_extend(convert_ushort4_sat_rte(color * 65535.0f)));
		case CLK_HALF_FLOAT:
			//TODO
			//return convert_half(color);
		case CLK_UNORM_INT_101010:
			return vc4cl_bitcast_int(vc4cl_extend(min(convert_ushort4_sat_rte(color * 1023.0f), (ushort4)0x3ff)));
		case CLK_FLOAT:
			return vc4cl_bitcast_int(color);
		default:
			return 0;
	}
		}

INLINE CONST int4 vc4cl_convert_to_image_type(int4 color, int channel_type) OVERLOADABLE
		{
	//see OpenCL 1.2, page 311:
	/*
	 * "write_imagei can only be used with image objects created with image_channel_data_type set to one of the following values:
	 *  CL_SIGNED_INT8, CL_SIGNED_INT16 and CL_SIGNED_INT32."
	 */
	//see OpenCL 1.2, page 340
	switch(channel_type)
	{
		case CLK_SIGNED_INT8:
			return vc4cl_extend(convert_char4_sat(color));
		case CLK_SIGNED_INT16:
			return vc4cl_extend(convert_short4_sat(color));
		case CLK_SIGNED_INT32:
			return color;
		default:
			return 0;
	}
		}

INLINE CONST int4 vc4cl_convert_to_image_type(uint4 color, int channel_type) OVERLOADABLE
		{
	//see OpenCL 1.2, page 311:
	/*
	 * "write_imageui can only be used with image objects created with image_channel_data_type set to one of the following values:
	 *  CL_UNSIGNED_INT8, CL_UNSIGNED_INT16 and CL_UNSIGNED_INT32."
	 */
	//see OpenCL 1.2, page 340
	switch(channel_type)
	{
		case CLK_UNSIGNED_INT8:
			return vc4cl_bitcast_int(vc4cl_extend(convert_uchar4_sat(color)));
		case CLK_UNSIGNED_INT16:
			return vc4cl_bitcast_int(vc4cl_extend(convert_ushort4_sat(color)));
		case CLK_UNSIGNED_INT32:
			return vc4cl_bitcast_int(color);
		default:
			return 0;
	}
		}

/*
 * De-normalizes float-coordinates, if specified in the sampler
 */
INLINE CONST float vc4cl_denormalize_coordinates(read_only image1d_t image, sampler_t sampler, float coord) OVERLOADABLE
		{
	if(vc4cl_sampler_get_normalized_coords(sampler) == CLK_NORMALIZED_COORDS_TRUE)
	{
		return get_image_width(image) * coord;
	}
	return coord;
		}

INLINE CONST float2 vc4cl_denormalize_coordinates(read_only image2d_t image, sampler_t sampler, float2 coord) OVERLOADABLE
		{
	if(vc4cl_sampler_get_normalized_coords(sampler) == CLK_NORMALIZED_COORDS_TRUE)
	{
		return (float2)(get_image_width(image) * coord.x, get_image_height(image) * coord.y);
	}
	return coord;
		}

INLINE CONST float4 vc4cl_denormalize_coordinates(read_only image3d_t image, sampler_t sampler, float4 coord) OVERLOADABLE
		{
	if(vc4cl_sampler_get_normalized_coords(sampler) == CLK_NORMALIZED_COORDS_TRUE)
	{
		return (float4)(get_image_width(image) * coord.x, get_image_height(image) * coord.y, get_image_depth(image) * coord.z, 0.0f);
	}
	return coord;
		}

/*
 * For CLK_FILTER_NEAREST, maps the float-coordinates to integer coordinates
 */
INLINE CONST int vc4cl_filter_nearest(float coord) OVERLOADABLE
		{
	//see OpenCL 1.2, page 329
	return (int)floor(coord);
		}

INLINE CONST int2 vc4cl_filter_nearest(float2 coord) OVERLOADABLE
		{
	//see OpenCL 1.2, page 329
	return convert_int2(floor(coord));
		}

INLINE CONST int4 vc4cl_filter_nearest(float4 coord) OVERLOADABLE
		{
	//see OpenCL 1.2, page 329
	return convert_int4(floor(coord));
		}

/*
 * Clamps the coordinates to the valid range
 */
INLINE CONST int vc4cl_clamp_coordinates(read_only image1d_t image, sampler_t sampler, int coord) OVERLOADABLE
		{
	//see OpenCL 1.2, page 295
	/*
	 * "CLK_ADDRESS_MIRRORED_REPEAT - [...] This addressing mode can only be used with normalized coordinates." -> skip for integer coordinates (de-normalized)
	 * "CLK_ADDRESS_REPEAT - [...] This addressing mode can only be used with normalized coordinates." -> skip for integer coordinates (de-normalized)
	 * "CLK_ADDRESS_CLAMP_TO_EDGE - out-of-range image coordinates are clamped to the extent."
	 * "CLK_ADDRESS_CLAMP - out-of-range image coordinates will return a border color."
	 * "CLK_ADDRESS_NONE"
	 */
	//see OpenCL 1.2, page 330
	if(vc4cl_sampler_get_addressing_mode(sampler) == CLK_ADDRESS_CLAMP_TO_EDGE)
	{
		return clamp(coord, 0, get_image_width(image) - 1);
	}
	if(vc4cl_sampler_get_addressing_mode(sampler) == CLK_ADDRESS_CLAMP)
	{
		return clamp(coord, -1, get_image_width(image));
	}
	return coord;
		}

INLINE CONST int2 vc4cl_clamp_coordinates(read_only image2d_t image, sampler_t sampler, int2 coord) OVERLOADABLE
		{
	if(vc4cl_sampler_get_addressing_mode(sampler) == CLK_ADDRESS_CLAMP_TO_EDGE)
	{
		return clamp(coord, (int2)0, (int2)(get_image_width(image) - 1, get_image_height(image) - 1));
	}
	if(vc4cl_sampler_get_addressing_mode(sampler) == CLK_ADDRESS_CLAMP)
	{
		return clamp(coord, (int2)-1, (int2)(get_image_width(image), get_image_height(image)));
	}
	return coord;
		}

INLINE CONST int4 vc4cl_clamp_coordinates(read_only image3d_t image, sampler_t sampler, int4 coord) OVERLOADABLE
		{
	if(vc4cl_sampler_get_addressing_mode(sampler) == CLK_ADDRESS_CLAMP_TO_EDGE)
	{
		return clamp(coord, (int4)0, (int4)(get_image_width(image) - 1, get_image_height(image) - 1, get_image_depth(image) - 1, coord.w));
	}
	if(vc4cl_sampler_get_addressing_mode(sampler) == CLK_ADDRESS_CLAMP)
	{
		return clamp(coord, (int4)-1, (int4)(get_image_width(image), get_image_height(image), get_image_depth(image), coord.w));
	}
	return coord;
		}

/*
 * Calculates the offset (in bytes) from the start of the data-segment to the pixel given by the coordinates
 */
INLINE CONST uint vc4cl_calculate_coordinate_offset(read_only image1d_t image, int coord) OVERLOADABLE
		{
	return vc4cl_bitcast_uint(coord) * vc4cl_get_channel_type_size(get_image_channel_data_type(image)) * vc4cl_get_channel_order_size(get_image_channel_order(image));
		}

INLINE CONST uint vc4cl_calculate_coordinate_offset(write_only image1d_t image, int coord) OVERLOADABLE
		{
	return vc4cl_bitcast_uint(coord) * vc4cl_get_channel_type_size(get_image_channel_data_type(image)) * vc4cl_get_channel_order_size(get_image_channel_order(image));
		}

INLINE CONST uint vc4cl_calculate_coordinate_offset(read_only image1d_buffer_t image, int coord) OVERLOADABLE
		{
	return vc4cl_bitcast_uint(coord) * vc4cl_get_channel_type_size(get_image_channel_data_type(image)) * vc4cl_get_channel_order_size(get_image_channel_order(image));
		}

INLINE CONST uint vc4cl_calculate_coordinate_offset(write_only image1d_buffer_t image, int coord) OVERLOADABLE
		{
	return vc4cl_bitcast_uint(coord) * vc4cl_get_channel_type_size(get_image_channel_data_type(image)) * vc4cl_get_channel_order_size(get_image_channel_order(image));
		}

INLINE CONST uint vc4cl_calculate_coordinate_offset(read_only image1d_array_t image, int2 coord) OVERLOADABLE
		{
	uint2 pitches = vc4cl_image_get_pitches(image);
	return pitches.x * vc4cl_bitcast_uint(coord).y + vc4cl_bitcast_uint(coord).x * vc4cl_get_channel_type_size(get_image_channel_data_type(image)) * vc4cl_get_channel_order_size(get_image_channel_order(image));
		}

INLINE CONST uint vc4cl_calculate_coordinate_offset(write_only image1d_array_t image, int2 coord) OVERLOADABLE
		{
	uint2 pitches = vc4cl_image_get_pitches(image);
	return pitches.x * vc4cl_bitcast_uint(coord).y + vc4cl_bitcast_uint(coord).x * vc4cl_get_channel_type_size(get_image_channel_data_type(image)) * vc4cl_get_channel_order_size(get_image_channel_order(image));
		}

INLINE CONST uint vc4cl_calculate_coordinate_offset(read_only image2d_t image, int2 coord) OVERLOADABLE
		{
	uint2 pitches = vc4cl_image_get_pitches(image);
	return pitches.x * vc4cl_bitcast_uint(coord).y + vc4cl_bitcast_uint(coord).x * vc4cl_get_channel_type_size(get_image_channel_data_type(image)) * vc4cl_get_channel_order_size(get_image_channel_order(image));
		}

INLINE CONST uint vc4cl_calculate_coordinate_offset(write_only image2d_t image, int2 coord) OVERLOADABLE
		{
	uint2 pitches = vc4cl_image_get_pitches(image);
	return pitches.x * vc4cl_bitcast_uint(coord).y + vc4cl_bitcast_uint(coord).x * vc4cl_get_channel_type_size(get_image_channel_data_type(image)) * vc4cl_get_channel_order_size(get_image_channel_order(image));
		}

INLINE CONST uint vc4cl_calculate_coordinate_offset(read_only image2d_array_t image, int4 coord) OVERLOADABLE
		{
	uint2 pitches = vc4cl_image_get_pitches(image);
	return pitches.x * vc4cl_bitcast_uint(coord).y + pitches.y * vc4cl_bitcast_uint(coord).z + vc4cl_bitcast_uint(coord).x * vc4cl_get_channel_type_size(get_image_channel_data_type(image)) * vc4cl_get_channel_order_size(get_image_channel_order(image));
		}

INLINE CONST uint vc4cl_calculate_coordinate_offset(write_only image2d_array_t image, int4 coord) OVERLOADABLE
		{
	uint2 pitches = vc4cl_image_get_pitches(image);
	return pitches.x * vc4cl_bitcast_uint(coord).y + pitches.y * vc4cl_bitcast_uint(coord).z + vc4cl_bitcast_uint(coord).x * vc4cl_get_channel_type_size(get_image_channel_data_type(image)) * vc4cl_get_channel_order_size(get_image_channel_order(image));
		}

INLINE CONST uint vc4cl_calculate_coordinate_offset(read_only image3d_t image, int4 coord) OVERLOADABLE
		{
	uint2 pitches = vc4cl_image_get_pitches(image);
	return pitches.x * vc4cl_bitcast_uint(coord).y + pitches.y * vc4cl_bitcast_uint(coord).z + vc4cl_bitcast_uint(coord).x * vc4cl_get_channel_type_size(get_image_channel_data_type(image)) * vc4cl_get_channel_order_size(get_image_channel_order(image));
		}

INLINE CONST uint vc4cl_calculate_coordinate_offset(write_only image3d_t image, int4 coord) OVERLOADABLE
		{
	uint2 pitches = vc4cl_image_get_pitches(image);
	return pitches.x * vc4cl_bitcast_uint(coord).y + pitches.y * vc4cl_bitcast_uint(coord).z + vc4cl_bitcast_uint(coord).x * vc4cl_get_channel_type_size(get_image_channel_data_type(image)) * vc4cl_get_channel_order_size(get_image_channel_order(image));
		}

/*
 * General process of reading image:
 * 1. convert coordinates to de-normalized
 * 2. check if coordinates are within in limits of image-borders
 * 2.1 apply method defined in sampler to handle overflows
 * 3. access the values at/around the given coordinates
 * 3.1 calculate address from coordinates, channel-type and channel-order
 * 3.2 interpolate with the method specified in sampler
 * 4. convert the result to the return-type
 * 4.1 fill vector-elements with corresponding values
 */

INLINE float4 read_imagef(read_only image2d_t image, sampler_t sampler, int2 coord) OVERLOADABLE
		{
	//see OpenCL 1.2, pages 297/298
	/*
	 * "The read_imagef (read_image[u]i) calls that take integer coordinates must use a sampler with filter mode set to
	 *  CLK_FILTER_NEAREST, normalized coordinates set to CLK_NORMALIZED_COORDS_FALSE and addressing mode set to
	 *  CLK_ADDRESS_CLAMP_TO_EDGE, CLK_ADDRESS_CLAMP or CLK_ADDRESS_NONE; otherwise the values returned are undefined."
	 *  -> For integer coordinates, no interpolation/de-normalization is required
	 */
	int channel_type = get_image_channel_data_type(image);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
	int2 clampedCoord = vc4cl_clamp_coordinates(image, sampler, coord);
	uint offset = vc4cl_calculate_coordinate_offset(image, clampedCoord);
	void* addr = vc4cl_image_get_data_address(image) + offset;
	int4 image_color = vc4cl_image_read_pixel(image, addr, type_size, channel_size);
	return vc4cl_convert_from_image_to_float(image_color, channel_type);
		}

//    INLINE float4 read_imagef(read_only image2d_t image, sampler_t sampler, float2 coord) OVERLOADABLE
//	{
//    	int channel_type = get_image_channel_data_type(image);
//		uint type_size = vc4cl_get_channel_type_size(channel_type);
//		uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
//		int2 filteredCoord;
//		if(vc4cl_sampler_get_filter_mode(sampler) == CLK_FILTER_NEAREST)
//		{
//			filteredCoord = vc4cl_filter_nearest(coord);
//			int2 clampedCoord = vc4cl_clamp_coordinates(image, sampler, filteredCoord);
//			uint offset = vc4cl_calculate_coordinate_offset(image, clampedCoord);
//			void* addr = vc4cl_image_get_data_address(image) + offset;
//			int4 image_color = vc4cl_image_read_pixel(image, addr, type_size, channel_size);
//			return vc4cl_convert_from_image_to_float(image_color, channel_type);
//		}
//		else
//		{
//			//CLK_FILTER_LINEAR -> interpolate
//			//see OpenCL 1.2, page 331
//			//TODO rewrite to handle according to combination of filter and addressing-mode
//		}
//	}

INLINE int4 read_imagei(read_only image2d_t image, sampler_t sampler, int2 coord) OVERLOADABLE
		{
	int channel_type = get_image_channel_data_type(image);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
	int2 clampedCoord = vc4cl_clamp_coordinates(image, sampler, coord);
	uint offset = vc4cl_calculate_coordinate_offset(image, clampedCoord);
	void* addr = vc4cl_image_get_data_address(image) + offset;
	int4 image_color = vc4cl_image_read_pixel(image, addr, type_size, channel_size);
	return vc4cl_convert_from_image_to_int(image_color, channel_type);
		}

int4 read_imagei(read_only image2d_t image, sampler_t sampler, float2 coord) OVERLOADABLE;

INLINE uint4 read_imageui(read_only image2d_t image, sampler_t sampler, int2 coord) OVERLOADABLE
		{
	int channel_type = get_image_channel_data_type(image);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
	int2 clampedCoord = vc4cl_clamp_coordinates(image, sampler, coord);
	uint offset = vc4cl_calculate_coordinate_offset(image, clampedCoord);
	void* addr = vc4cl_image_get_data_address(image) + offset;
	int4 image_color = vc4cl_image_read_pixel(image, addr, type_size, channel_size);
	return vc4cl_convert_from_image_to_uint(image_color, channel_type);
		}

uint4 read_imageui(read_only image2d_t image, sampler_t sampler, float2 coord) OVERLOADABLE;

INLINE float4 read_imagef(read_only image3d_t image, sampler_t sampler, int4 coord) OVERLOADABLE
		{
	int channel_type = get_image_channel_data_type(image);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
	int4 clampedCoord = vc4cl_clamp_coordinates(image, sampler, coord);
	uint offset = vc4cl_calculate_coordinate_offset(image, clampedCoord);
	void* addr = vc4cl_image_get_data_address(image) + offset;
	int4 image_color = vc4cl_image_read_pixel(image, addr, type_size, channel_size);
	return vc4cl_convert_from_image_to_float(image_color, channel_type);
		}

float4 read_imagef(read_only image3d_t image, sampler_t sampler, float4 coord) OVERLOADABLE;

INLINE int4 read_imagei(read_only image3d_t image, sampler_t sampler, int4 coord) OVERLOADABLE
		{
	int channel_type = get_image_channel_data_type(image);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
	int4 clampedCoord = vc4cl_clamp_coordinates(image, sampler, coord);
	uint offset = vc4cl_calculate_coordinate_offset(image, clampedCoord);
	void* addr = vc4cl_image_get_data_address(image) + offset;
	int4 image_color = vc4cl_image_read_pixel(image, addr, type_size, channel_size);
	return vc4cl_convert_from_image_to_int(image_color, channel_type);
		}

int4 read_imagei(read_only image3d_t image, sampler_t sampler, float4 coord) OVERLOADABLE;

INLINE uint4 read_imageui(read_only image3d_t image, sampler_t sampler, int4 coord) OVERLOADABLE
		{
	int channel_type = get_image_channel_data_type(image);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
	int4 clampedCoord = vc4cl_clamp_coordinates(image, sampler, coord);
	uint offset = vc4cl_calculate_coordinate_offset(image, clampedCoord);
	void* addr = vc4cl_image_get_data_address(image) + offset;
	int4 image_color = vc4cl_image_read_pixel(image, addr, type_size, channel_size);
	return vc4cl_convert_from_image_to_uint(image_color, channel_type);
		}

uint4 read_imageui(read_only image3d_t image, sampler_t sampler, float4 coord) OVERLOADABLE;

float4 read_imagef(read_only image2d_array_t image_array, sampler_t sampler, int4 coord) OVERLOADABLE;
float4 read_imagef(read_only image2d_array_t image_array, sampler_t sampler, float4 coord) OVERLOADABLE;

int4 read_imagei(read_only image2d_array_t image_array, sampler_t sampler, int4 coord) OVERLOADABLE;
int4 read_imagei(read_only image2d_array_t image_array, sampler_t sampler, float4 coord) OVERLOADABLE;
uint4 read_imageui(read_only image2d_array_t image_array, sampler_t sampler, int4 coord) OVERLOADABLE;
uint4 read_imageui(read_only image2d_array_t image_array, sampler_t sampler, float4 coord) OVERLOADABLE;

INLINE float4 read_imagef(read_only image1d_t image, sampler_t sampler, int coord) OVERLOADABLE
		{
	int channel_type = get_image_channel_data_type(image);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
	int clampedCoord = vc4cl_clamp_coordinates(image, sampler, coord);
	uint offset = vc4cl_calculate_coordinate_offset(image, clampedCoord);
	void* addr = vc4cl_image_get_data_address(image) + offset;
	int4 image_color = vc4cl_image_read_pixel(image, addr, type_size, channel_size);
	return vc4cl_convert_from_image_to_float(image_color, channel_type);
		}

float4 read_imagef(read_only image1d_t image, sampler_t sampler, float coord) OVERLOADABLE;

INLINE int4 read_imagei(read_only image1d_t image, sampler_t sampler, int coord) OVERLOADABLE
		{
	int channel_type = get_image_channel_data_type(image);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
	int clampedCoord = vc4cl_clamp_coordinates(image, sampler, coord);
	uint offset = vc4cl_calculate_coordinate_offset(image, clampedCoord);
	void* addr = vc4cl_image_get_data_address(image) + offset;
	int4 image_color = vc4cl_image_read_pixel(image, addr, type_size, channel_size);
	return vc4cl_convert_from_image_to_int(image_color, channel_type);
		}

int4 read_imagei(read_only image1d_t image, sampler_t sampler, float coord) OVERLOADABLE;

INLINE uint4 read_imageui(read_only image1d_t image, sampler_t sampler, int coord) OVERLOADABLE
		{
	int channel_type = get_image_channel_data_type(image);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
	int clampedCoord = vc4cl_clamp_coordinates(image, sampler, coord);
	uint offset = vc4cl_calculate_coordinate_offset(image, clampedCoord);
	void* addr = vc4cl_image_get_data_address(image) + offset;
	int4 image_color = vc4cl_image_read_pixel(image, addr, type_size, channel_size);
	return vc4cl_convert_from_image_to_uint(image_color, channel_type);
		}
uint4 read_imageui(read_only image1d_t image, sampler_t sampler, float coord) OVERLOADABLE;

float4 read_imagef(read_only image1d_array_t image_array, sampler_t sampler, int2 coord) OVERLOADABLE;
float4 read_imagef(read_only image1d_array_t image_array, sampler_t sampler, float2 coord) OVERLOADABLE;

int4 read_imagei(read_only image1d_array_t image_array, sampler_t sampler, int2 coord) OVERLOADABLE;
int4 read_imagei(read_only image1d_array_t image_array, sampler_t sampler, float2 coord) OVERLOADABLE;
uint4 read_imageui(read_only image1d_array_t image_array, sampler_t sampler, int2 coord) OVERLOADABLE;
uint4 read_imageui(read_only image1d_array_t image_array, sampler_t sampler, float2 coord) OVERLOADABLE;

/*
 * 6.12.14.3 Built-in Image Sampler-less Read Functions
 *
 * "The following built-in function calls to read images with a sampler are supported.
 *  The sampler-less read image functions behave exactly as the corresponding read image functions described
 *  in section 6.12.14.2 that take integer coordinates and a sampler with
 *  filter mode set to CLK_FILTER_NEAREST,
 *  normalized coordinates set to CLK_NORMALIZED_COORDS_FALSE and
 *  addressing mode to CLK_ADDRESS_NONE."
 */

INLINE float4 read_imagef(read_only image1d_t image, int coord) OVERLOADABLE
		{
	int channel_type = get_image_channel_data_type(image);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
	uint offset = vc4cl_calculate_coordinate_offset(image, coord);
	void* addr = vc4cl_image_get_data_address(image) + offset;
	int4 image_color = vc4cl_image_read_pixel(image, addr, type_size, channel_size);
	return vc4cl_convert_from_image_to_float(image_color, channel_type);
		}

INLINE int4 read_imagei(read_only image1d_t image, int coord) OVERLOADABLE
		{
	int channel_type = get_image_channel_data_type(image);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
	uint offset = vc4cl_calculate_coordinate_offset(image, coord);
	void* addr = vc4cl_image_get_data_address(image) + offset;
	int4 image_color = vc4cl_image_read_pixel(image, addr, type_size, channel_size);
	return vc4cl_convert_from_image_to_int(image_color, channel_type);
		}

INLINE uint4 read_imageui(read_only image1d_t image, int coord) OVERLOADABLE
		{
	int channel_type = get_image_channel_data_type(image);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
	uint offset = vc4cl_calculate_coordinate_offset(image, coord);
	void* addr = vc4cl_image_get_data_address(image) + offset;
	int4 image_color = vc4cl_image_read_pixel(image, addr, type_size, channel_size);
	return vc4cl_convert_from_image_to_uint(image_color, channel_type);
		}

INLINE float4 read_imagef(read_only image1d_buffer_t image, int coord) OVERLOADABLE
		{
	int channel_type = get_image_channel_data_type(image);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
	uint offset = vc4cl_calculate_coordinate_offset(image, coord);
	void* addr = vc4cl_image_get_data_address(image) + offset;
	int4 image_color = vc4cl_image_read_pixel(image, addr, type_size, channel_size);
	return vc4cl_convert_from_image_to_float(image_color, channel_type);
		}

INLINE int4 read_imagei(read_only image1d_buffer_t image, int coord) OVERLOADABLE
		{
	int channel_type = get_image_channel_data_type(image);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
	uint offset = vc4cl_calculate_coordinate_offset(image, coord);
	void* addr = vc4cl_image_get_data_address(image) + offset;
	int4 image_color = vc4cl_image_read_pixel(image, addr, type_size, channel_size);
	return vc4cl_convert_from_image_to_int(image_color, channel_type);
		}

INLINE uint4 read_imageui(read_only image1d_buffer_t image, int coord) OVERLOADABLE
		{
	int channel_type = get_image_channel_data_type(image);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
	uint offset = vc4cl_calculate_coordinate_offset(image, coord);
	void* addr = vc4cl_image_get_data_address(image) + offset;
	int4 image_color = vc4cl_image_read_pixel(image, addr, type_size, channel_size);
	return vc4cl_convert_from_image_to_uint(image_color, channel_type);
		}

INLINE float4 read_imagef(read_only image1d_array_t image, int2 coord) OVERLOADABLE
		{
	int channel_type = get_image_channel_data_type(image);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
	uint offset = vc4cl_calculate_coordinate_offset(image, coord);
	void* addr = vc4cl_image_get_data_address(image) + offset;
	int4 image_color = vc4cl_image_read_pixel(image, addr, type_size, channel_size);
	return vc4cl_convert_from_image_to_float(image_color, channel_type);
		}

INLINE int4 read_imagei(read_only image1d_array_t image, int2 coord) OVERLOADABLE
		{
	int channel_type = get_image_channel_data_type(image);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
	uint offset = vc4cl_calculate_coordinate_offset(image, coord);
	void* addr = vc4cl_image_get_data_address(image) + offset;
	int4 image_color = vc4cl_image_read_pixel(image, addr, type_size, channel_size);
	return vc4cl_convert_from_image_to_int(image_color, channel_type);
		}

INLINE uint4 read_imageui(read_only image1d_array_t image, int2 coord) OVERLOADABLE
		{
	int channel_type = get_image_channel_data_type(image);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
	uint offset = vc4cl_calculate_coordinate_offset(image, coord);
	void* addr = vc4cl_image_get_data_address(image) + offset;
	int4 image_color = vc4cl_image_read_pixel(image, addr, type_size, channel_size);
	return vc4cl_convert_from_image_to_uint(image_color, channel_type);
		}

INLINE float4 read_imagef(read_only image2d_t image, int2 coord) OVERLOADABLE
		{
	int channel_type = get_image_channel_data_type(image);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
	uint offset = vc4cl_calculate_coordinate_offset(image, coord);
	void* addr = vc4cl_image_get_data_address(image) + offset;
	int4 image_color = vc4cl_image_read_pixel(image, addr, type_size, channel_size);
	return vc4cl_convert_from_image_to_float(image_color, channel_type);
		}

INLINE int4 read_imagei(read_only image2d_t image, int2 coord) OVERLOADABLE
		{
	int channel_type = get_image_channel_data_type(image);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
	uint offset = vc4cl_calculate_coordinate_offset(image, coord);
	void* addr = vc4cl_image_get_data_address(image) + offset;
	int4 image_color = vc4cl_image_read_pixel(image, addr, type_size, channel_size);
	return vc4cl_convert_from_image_to_int(image_color, channel_type);
		}

INLINE uint4 read_imageui(read_only image2d_t image, int2 coord) OVERLOADABLE
		{
	int channel_type = get_image_channel_data_type(image);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
	uint offset = vc4cl_calculate_coordinate_offset(image, coord);
	void* addr = vc4cl_image_get_data_address(image) + offset;
	int4 image_color = vc4cl_image_read_pixel(image, addr, type_size, channel_size);
	return vc4cl_convert_from_image_to_uint(image_color, channel_type);
		}

INLINE float4 read_imagef(read_only image2d_array_t image, int4 coord) OVERLOADABLE
		{
	int channel_type = get_image_channel_data_type(image);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
	uint offset = vc4cl_calculate_coordinate_offset(image, coord);
	void* addr = vc4cl_image_get_data_address(image) + offset;
	int4 image_color = vc4cl_image_read_pixel(image, addr, type_size, channel_size);
	return vc4cl_convert_from_image_to_float(image_color, channel_type);
		}

INLINE int4 read_imagei(read_only image2d_array_t image, int4 coord) OVERLOADABLE
		{
	int channel_type = get_image_channel_data_type(image);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
	uint offset = vc4cl_calculate_coordinate_offset(image, coord);
	void* addr = vc4cl_image_get_data_address(image) + offset;
	int4 image_color = vc4cl_image_read_pixel(image, addr, type_size, channel_size);
	return vc4cl_convert_from_image_to_int(image_color, channel_type);
		}

INLINE uint4 read_imageui(read_only image2d_array_t image, int4 coord) OVERLOADABLE
		{
	int channel_type = get_image_channel_data_type(image);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
	uint offset = vc4cl_calculate_coordinate_offset(image, coord);
	void* addr = vc4cl_image_get_data_address(image) + offset;
	int4 image_color = vc4cl_image_read_pixel(image, addr, type_size, channel_size);
	return vc4cl_convert_from_image_to_uint(image_color, channel_type);
		}

INLINE float4 read_imagef(read_only image3d_t image, int4 coord) OVERLOADABLE
		{
	int channel_type = get_image_channel_data_type(image);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
	uint offset = vc4cl_calculate_coordinate_offset(image, coord);
	void* addr = vc4cl_image_get_data_address(image) + offset;
	int4 image_color = vc4cl_image_read_pixel(image, addr, type_size, channel_size);
	return vc4cl_convert_from_image_to_float(image_color, channel_type);
		}

INLINE int4 read_imagei(read_only image3d_t image, int4 coord) OVERLOADABLE
		{
	int channel_type = get_image_channel_data_type(image);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
	uint offset = vc4cl_calculate_coordinate_offset(image, coord);
	void* addr = vc4cl_image_get_data_address(image) + offset;
	int4 image_color = vc4cl_image_read_pixel(image, addr, type_size, channel_size);
	return vc4cl_convert_from_image_to_int(image_color, channel_type);
		}

INLINE uint4 read_imageui(read_only image3d_t image, int4 coord) OVERLOADABLE
		{
	int channel_type = get_image_channel_data_type(image);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
	uint offset = vc4cl_calculate_coordinate_offset(image, coord);
	void* addr = vc4cl_image_get_data_address(image) + offset;
	int4 image_color = vc4cl_image_read_pixel(image, addr, type_size, channel_size);
	return vc4cl_convert_from_image_to_uint(image_color, channel_type);
		}

/*
 * General process of writing image (OpenCL 1.2, pages 310+):
 * 1. convert value to format specified in image,
 *  "Appropriate data format conversion to the specified image format is done before writing the color value."
 * 2. no need to convert coordinates, "coord.x, [coord.y and coord.z] are considered to be unnormalized coordinates and must be in the range [...]"
 * 2.1 calculate address from coordinates, channel-type and channel-order
 * 3. write into memory at given location
 */
INLINE void write_imagef(write_only image2d_t image, int2 coord, float4 color) OVERLOADABLE
		{
	int channel_type = get_image_channel_data_type(image);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	int4 converted_color = vc4cl_convert_to_image_type(color, channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
	uint offset = vc4cl_calculate_coordinate_offset(image, coord);
	void* addr = vc4cl_image_get_data_address(image) + offset;
	vc4cl_image_write_pixel(image, addr, converted_color, type_size, channel_size);
		}

INLINE void write_imagei(write_only image2d_t image, int2 coord, int4 color) OVERLOADABLE
		{
	int channel_type = get_image_channel_data_type(image);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	int4 converted_color = vc4cl_convert_to_image_type(color, channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
	uint offset = vc4cl_calculate_coordinate_offset(image, coord);
	void* addr = vc4cl_image_get_data_address(image) + offset;
	vc4cl_image_write_pixel(image, addr, converted_color, type_size, channel_size);
		}

INLINE void write_imageui(write_only image2d_t image, int2 coord, uint4 color) OVERLOADABLE
		{
	int channel_type = get_image_channel_data_type(image);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	int4 converted_color = vc4cl_convert_to_image_type(color, channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
	uint offset = vc4cl_calculate_coordinate_offset(image, coord);
	void* addr = vc4cl_image_get_data_address(image) + offset;
	vc4cl_image_write_pixel(image, addr, converted_color, type_size, channel_size);
		}

INLINE void write_imagef(write_only image2d_array_t image_array, int4 coord, float4 color) OVERLOADABLE
		{
	int channel_type = get_image_channel_data_type(image_array);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	int4 converted_color = vc4cl_convert_to_image_type(color, channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image_array));
	uint offset = vc4cl_calculate_coordinate_offset(image_array, coord);
	void* addr = vc4cl_image_get_data_address(image_array) + offset;
	vc4cl_image_write_pixel(image_array, addr, converted_color, type_size, channel_size);
		}

INLINE void write_imagei(write_only image2d_array_t image_array, int4 coord, int4 color) OVERLOADABLE
		{
	int channel_type = get_image_channel_data_type(image_array);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	int4 converted_color = vc4cl_convert_to_image_type(color, channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image_array));
	uint offset = vc4cl_calculate_coordinate_offset(image_array, coord);
	void* addr = vc4cl_image_get_data_address(image_array) + offset;
	vc4cl_image_write_pixel(image_array, addr, converted_color, type_size, channel_size);
		}

INLINE void write_imageui(write_only image2d_array_t image_array, int4 coord, uint4 color) OVERLOADABLE
		{
	int channel_type = get_image_channel_data_type(image_array);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	int4 converted_color = vc4cl_convert_to_image_type(color, channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image_array));
	uint offset = vc4cl_calculate_coordinate_offset(image_array, coord);
	void* addr = vc4cl_image_get_data_address(image_array) + offset;
	vc4cl_image_write_pixel(image_array, addr, converted_color, type_size, channel_size);
		}

INLINE void write_imagef(write_only image1d_t image, int coord, float4 color) OVERLOADABLE
		{
	int channel_type = get_image_channel_data_type(image);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	int4 converted_color = vc4cl_convert_to_image_type(color, channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
	uint offset = vc4cl_calculate_coordinate_offset(image, coord);
	void* addr = vc4cl_image_get_data_address(image) + offset;
	vc4cl_image_write_pixel(image, addr, converted_color, type_size, channel_size);
		}

INLINE void write_imagei(write_only image1d_t image, int coord, int4 color) OVERLOADABLE
		{
	int channel_type = get_image_channel_data_type(image);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	int4 converted_color = vc4cl_convert_to_image_type(color, channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
	uint offset = vc4cl_calculate_coordinate_offset(image, coord);
	void* addr = vc4cl_image_get_data_address(image) + offset;
	vc4cl_image_write_pixel(image, addr, converted_color, type_size, channel_size);
		}

INLINE void write_imageui(write_only image1d_t image, int coord, uint4 color) OVERLOADABLE
		{
	int channel_type = get_image_channel_data_type(image);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	int4 converted_color = vc4cl_convert_to_image_type(color, channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
	uint offset = vc4cl_calculate_coordinate_offset(image, coord);
	void* addr = vc4cl_image_get_data_address(image) + offset;
	vc4cl_image_write_pixel(image, addr, converted_color, type_size, channel_size);
		}

INLINE void write_imagef(write_only image1d_buffer_t image, int coord, float4 color) OVERLOADABLE
		{
	int channel_type = get_image_channel_data_type(image);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	int4 converted_color = vc4cl_convert_to_image_type(color, channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
	uint offset = vc4cl_calculate_coordinate_offset(image, coord);
	void* addr = vc4cl_image_get_data_address(image) + offset;
	vc4cl_image_write_pixel(image, addr, converted_color, type_size, channel_size);
		}

INLINE void write_imagei(write_only image1d_buffer_t image, int coord, int4 color) OVERLOADABLE
		{
	int channel_type = get_image_channel_data_type(image);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	int4 converted_color = vc4cl_convert_to_image_type(color, channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
	uint offset = vc4cl_calculate_coordinate_offset(image, coord);
	void* addr = vc4cl_image_get_data_address(image) + offset;
	vc4cl_image_write_pixel(image, addr, converted_color, type_size, channel_size);
		}

INLINE void write_imageui(write_only image1d_buffer_t image, int coord, uint4 color) OVERLOADABLE
		{
	int channel_type = get_image_channel_data_type(image);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	int4 converted_color = vc4cl_convert_to_image_type(color, channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
	uint offset = vc4cl_calculate_coordinate_offset(image, coord);
	void* addr = vc4cl_image_get_data_address(image) + offset;
	vc4cl_image_write_pixel(image, addr, converted_color, type_size, channel_size);
		}

INLINE void write_imagef(write_only image1d_array_t image_array, int2 coord, float4 color) OVERLOADABLE
		{
	int channel_type = get_image_channel_data_type(image_array);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	int4 converted_color = vc4cl_convert_to_image_type(color, channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image_array));
	uint offset = vc4cl_calculate_coordinate_offset(image_array, coord);
	void* addr = vc4cl_image_get_data_address(image_array) + offset;
	vc4cl_image_write_pixel(image_array, addr, converted_color, type_size, channel_size);
		}

INLINE void write_imagei(write_only image1d_array_t image_array, int2 coord, int4 color) OVERLOADABLE
		{
	int channel_type = get_image_channel_data_type(image_array);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	int4 converted_color = vc4cl_convert_to_image_type(color, channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image_array));
	uint offset = vc4cl_calculate_coordinate_offset(image_array, coord);
	void* addr = vc4cl_image_get_data_address(image_array) + offset;
	vc4cl_image_write_pixel(image_array, addr, converted_color, type_size, channel_size);
		}

INLINE void write_imageui(write_only image1d_array_t image_array, int2 coord, uint4 color) OVERLOADABLE
		{
	int channel_type = get_image_channel_data_type(image_array);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	int4 converted_color = vc4cl_convert_to_image_type(color, channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image_array));
	uint offset = vc4cl_calculate_coordinate_offset(image_array, coord);
	void* addr = vc4cl_image_get_data_address(image_array) + offset;
	vc4cl_image_write_pixel(image_array, addr, converted_color, type_size, channel_size);
		}

INLINE void write_imagef(write_only image3d_t image, int4 coord, float4 color) OVERLOADABLE
		{
	int channel_type = get_image_channel_data_type(image);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	int4 converted_color = vc4cl_convert_to_image_type(color, channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
	uint offset = vc4cl_calculate_coordinate_offset(image, coord);
	void* addr = vc4cl_image_get_data_address(image) + offset;
	vc4cl_image_write_pixel(image, addr, converted_color, type_size, channel_size);
		}

INLINE void write_imagei(write_only image3d_t image, int4 coord, int4 color) OVERLOADABLE
		{
	int channel_type = get_image_channel_data_type(image);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	int4 converted_color = vc4cl_convert_to_image_type(color, channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
	uint offset = vc4cl_calculate_coordinate_offset(image, coord);
	void* addr = vc4cl_image_get_data_address(image) + offset;
	vc4cl_image_write_pixel(image, addr, converted_color, type_size, channel_size);
		}

INLINE void write_imageui(write_only image3d_t image, int4 coord, uint4 color) OVERLOADABLE
		{
	int channel_type = get_image_channel_data_type(image);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	int4 converted_color = vc4cl_convert_to_image_type(color, channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
	uint offset = vc4cl_calculate_coordinate_offset(image, coord);
	void* addr = vc4cl_image_get_data_address(image) + offset;
	vc4cl_image_write_pixel(image, addr, converted_color, type_size, channel_size);
		}

/*
 * Don't map following functions to intrinsics, since they are handled quite nicely by SPIR-V
 */

 CONST int get_image_width(read_only image1d_t image) OVERLOADABLE;
CONST int get_image_width(read_only image1d_buffer_t image) OVERLOADABLE;
CONST int get_image_width(read_only image2d_t image) OVERLOADABLE;
CONST int get_image_width(read_only image3d_t image) OVERLOADABLE;
CONST int get_image_width(read_only image1d_array_t image) OVERLOADABLE;
CONST int get_image_width(read_only image2d_array_t image) OVERLOADABLE;
CONST int get_image_width(write_only image1d_t image) OVERLOADABLE;
CONST int get_image_width(write_only image1d_buffer_t image) OVERLOADABLE;
CONST int get_image_width(write_only image2d_t image) OVERLOADABLE;
CONST int get_image_width(write_only image3d_t image) OVERLOADABLE;
CONST int get_image_width(write_only image1d_array_t image) OVERLOADABLE;
CONST int get_image_width(write_only image2d_array_t image) OVERLOADABLE;

CONST int get_image_height(read_only image2d_t image) OVERLOADABLE;
CONST int get_image_height(read_only image3d_t image) OVERLOADABLE;
CONST int get_image_height(read_only image2d_array_t image) OVERLOADABLE;
CONST int get_image_height(write_only image2d_t image) OVERLOADABLE;
CONST int get_image_height(write_only image3d_t image) OVERLOADABLE;
CONST int get_image_height(write_only image2d_array_t image) OVERLOADABLE;

CONST int get_image_depth(read_only image3d_t image) OVERLOADABLE;
CONST int get_image_depth(write_only image3d_t image) OVERLOADABLE;

CONST int get_image_channel_data_type(read_only image1d_t image) OVERLOADABLE;
CONST int get_image_channel_data_type(read_only image1d_buffer_t image) OVERLOADABLE;
CONST int get_image_channel_data_type(read_only image2d_t image) OVERLOADABLE;
CONST int get_image_channel_data_type(read_only image3d_t image) OVERLOADABLE;
CONST int get_image_channel_data_type(read_only image1d_array_t image) OVERLOADABLE;
CONST int get_image_channel_data_type(read_only image2d_array_t image) OVERLOADABLE;
CONST int get_image_channel_data_type(write_only image1d_t image) OVERLOADABLE;
CONST int get_image_channel_data_type(write_only image1d_buffer_t image) OVERLOADABLE;
CONST int get_image_channel_data_type(write_only image2d_t image) OVERLOADABLE;
CONST int get_image_channel_data_type(write_only image3d_t image) OVERLOADABLE;
CONST int get_image_channel_data_type(write_only image1d_array_t image) OVERLOADABLE;
CONST int get_image_channel_data_type(write_only image2d_array_t image) OVERLOADABLE;

CONST int get_image_channel_order(read_only image1d_t image) OVERLOADABLE;
CONST int get_image_channel_order(read_only image1d_buffer_t image) OVERLOADABLE;
CONST int get_image_channel_order(read_only image2d_t image) OVERLOADABLE;
CONST int get_image_channel_order(read_only image3d_t image) OVERLOADABLE;
CONST int get_image_channel_order(read_only image1d_array_t image) OVERLOADABLE;
CONST int get_image_channel_order(read_only image2d_array_t image) OVERLOADABLE;
CONST int get_image_channel_order(write_only image1d_t image) OVERLOADABLE;
CONST int get_image_channel_order(write_only image1d_buffer_t image) OVERLOADABLE;
CONST int get_image_channel_order(write_only image2d_t image) OVERLOADABLE;
CONST int get_image_channel_order(write_only image3d_t image) OVERLOADABLE;
CONST int get_image_channel_order(write_only image1d_array_t image) OVERLOADABLE;
CONST int get_image_channel_order(write_only image2d_array_t image) OVERLOADABLE;

CONST int2 get_image_dim(read_only image2d_t image) OVERLOADABLE;
CONST int2 get_image_dim(read_only image2d_array_t image) OVERLOADABLE;
CONST int2 get_image_dim(write_only image2d_t image) OVERLOADABLE;
CONST int2 get_image_dim(write_only image2d_array_t image) OVERLOADABLE;
CONST int4 get_image_dim(read_only image3d_t image) OVERLOADABLE;
CONST int4 get_image_dim(write_only image3d_t image) OVERLOADABLE;

CONST size_t get_image_array_size(read_only image1d_array_t image_array) OVERLOADABLE;
CONST size_t get_image_array_size(read_only image2d_array_t image_array) OVERLOADABLE;
CONST size_t get_image_array_size(write_only image1d_array_t image_array) OVERLOADABLE;
CONST size_t get_image_array_size(write_only image2d_array_t image_array) OVERLOADABLE;

#endif /* __IMAGE_SUPPORT__ */

#endif /* VC4CL_IMAGES_H */

