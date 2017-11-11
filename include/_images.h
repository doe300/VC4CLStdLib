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

INLINE CONST float vc4cl_normalize_coordinates(int coordinate, int dimension) OVERLOADABLE
{
	return ((float)coordinate) / ((float)dimension);
}

INLINE CONST float2 vc4cl_normalize_coordinates(int2 coordinate, int2 dimension) OVERLOADABLE
{
	return convert_float2(coordinate) / convert_float2(dimension);
}

INLINE CONST float4 vc4cl_normalize_coordinates(int4 coordinate, int4 dimension) OVERLOADABLE
{
	return convert_float4(coordinate) / convert_float4(dimension);
}

INLINE CONST int vc4cl_get_array_index(float coord, int array_size)
{
	//see OpenCL 1.2 specification, section 8.4 "Selecting an Image from an Image Array"
	//TODO is the coordinate passed normalized or not?
	return clamp((int)rint(coord), 0, array_size - 1);
}

/*
 * Sets filter and wrap modes according to the values specified in the sampler
 */
INLINE CONST uint vc4cl_apply_sampler(uint access_setup, sampler_t sampler)
{
	//clear fields for filter and wrap-modes
	uint setup = access_setup & 0xFFFFFF00;
	//CLK_FILTER_LINEAR has value 0 for magnification and minification filter
	if(vc4cl_sampler_get_filter_mode(sampler) == CLK_FILTER_NEAREST)
	{
		//CLK_FILTER_NEAREST has value 1 for magnification and minification filter
		setup |= 0x00000090;
	}
	switch(vc4cl_sampler_get_addressing_mode(sampler))
	{
		//CLK_ADDRESS_NONE has unspecified value for coordinates exceeding the image, leave at 0
		//CLK_ADDRESS_REPEAT has value 0 for wrap modes, already set
		case CLK_ADDRESS_CLAMP:
			//Has value 1 for wrap modes
			setup |= 0x00000005;
			break;
		case CLK_ADDRESS_CLAMP_TO_EDGE:
			//Has value 3 for wrap modes
			//XXX CLK_ADDRESS_CLAMP_TO_EDGE == wrap mode border?
			setup |= 0x0000000F;
			break;
		case CLK_ADDRESS_MIRRORED_REPEAT:
			//Has value 2 for wrap modes
			setup |= 0x0000000A;
			break;
	}
	return setup;
}

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
	//TODO optimize by not setting the "default" sampler-modes, but setting them host-side? Need to set though, if image is accessed without and then with sampler!
	return read_imagef(image, (CLK_FILTER_NEAREST|CLK_NORMALIZED_COORDS_FALSE|CLK_ADDRESS_NONE), coord);
}

INLINE int4 read_imagei(read_only image1d_t image, int coord) OVERLOADABLE
{
	return read_imagei(image, (CLK_FILTER_NEAREST|CLK_NORMALIZED_COORDS_FALSE|CLK_ADDRESS_NONE), coord);
}

INLINE uint4 read_imageui(read_only image1d_t image, int coord) OVERLOADABLE
{
	return read_imageui(image, (CLK_FILTER_NEAREST|CLK_NORMALIZED_COORDS_FALSE|CLK_ADDRESS_NONE), coord);
}

INLINE float4 read_imagef(read_only image1d_buffer_t image, int coord) OVERLOADABLE
{
	vc4cl_set_image_access_setup(image, vc4cl_apply_sampler(vc4cl_image_access_setup(image), (CLK_FILTER_NEAREST|CLK_NORMALIZED_COORDS_FALSE|CLK_ADDRESS_NONE)));
	int channel_type = get_image_channel_data_type(image);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
	float xCoord = vc4cl_normalize_coordinates(coord, get_image_width(image));
	int4 image_color = vc4cl_image_read(image, xCoord, type_size, channel_size);
	return vc4cl_convert_from_image_to_float(image_color, channel_type);
}

INLINE int4 read_imagei(read_only image1d_buffer_t image, int coord) OVERLOADABLE
{
	vc4cl_set_image_access_setup(image, vc4cl_apply_sampler(vc4cl_image_access_setup(image), (CLK_FILTER_NEAREST|CLK_NORMALIZED_COORDS_FALSE|CLK_ADDRESS_NONE)));
	int channel_type = get_image_channel_data_type(image);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
	float xCoord = vc4cl_normalize_coordinates(coord, get_image_width(image));
	int4 image_color = vc4cl_image_read(image, xCoord, type_size, channel_size);
	return vc4cl_convert_from_image_to_int(image_color, channel_type);
}

INLINE uint4 read_imageui(read_only image1d_buffer_t image, int coord) OVERLOADABLE
{
	vc4cl_set_image_access_setup(image, vc4cl_apply_sampler(vc4cl_image_access_setup(image), (CLK_FILTER_NEAREST|CLK_NORMALIZED_COORDS_FALSE|CLK_ADDRESS_NONE)));
	int channel_type = get_image_channel_data_type(image);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
	float xCoord = vc4cl_normalize_coordinates(coord, get_image_width(image));
	int4 image_color = vc4cl_image_read(image, xCoord, type_size, channel_size);
	return vc4cl_convert_from_image_to_uint(image_color, channel_type);
}

INLINE float4 read_imagef(read_only image1d_array_t image, int2 coord) OVERLOADABLE
{
	return read_imagef(image, (CLK_FILTER_NEAREST|CLK_NORMALIZED_COORDS_FALSE|CLK_ADDRESS_NONE), coord);
}

INLINE int4 read_imagei(read_only image1d_array_t image, int2 coord) OVERLOADABLE
{
	return read_imagei(image, (CLK_FILTER_NEAREST|CLK_NORMALIZED_COORDS_FALSE|CLK_ADDRESS_NONE), coord);
}

INLINE uint4 read_imageui(read_only image1d_array_t image, int2 coord) OVERLOADABLE
{
	return read_imageui(image, (CLK_FILTER_NEAREST|CLK_NORMALIZED_COORDS_FALSE|CLK_ADDRESS_NONE), coord);
}

INLINE float4 read_imagef(read_only image2d_t image, int2 coord) OVERLOADABLE
{
	return read_imagef(image, (CLK_FILTER_NEAREST|CLK_NORMALIZED_COORDS_FALSE|CLK_ADDRESS_NONE), coord);
}

INLINE int4 read_imagei(read_only image2d_t image, int2 coord) OVERLOADABLE
{
	return read_imagei(image, (CLK_FILTER_NEAREST|CLK_NORMALIZED_COORDS_FALSE|CLK_ADDRESS_NONE), coord);
}

INLINE uint4 read_imageui(read_only image2d_t image, int2 coord) OVERLOADABLE
{
	return read_imageui(image, (CLK_FILTER_NEAREST|CLK_NORMALIZED_COORDS_FALSE|CLK_ADDRESS_NONE), coord);
}

INLINE float4 read_imagef(read_only image2d_array_t image, int4 coord) OVERLOADABLE
{
	return read_imagef(image, (CLK_FILTER_NEAREST|CLK_NORMALIZED_COORDS_FALSE|CLK_ADDRESS_NONE), coord);
}

INLINE int4 read_imagei(read_only image2d_array_t image, int4 coord) OVERLOADABLE
{
	return read_imagei(image, (CLK_FILTER_NEAREST|CLK_NORMALIZED_COORDS_FALSE|CLK_ADDRESS_NONE), coord);
}

INLINE uint4 read_imageui(read_only image2d_array_t image, int4 coord) OVERLOADABLE
{
	return read_imageui(image, (CLK_FILTER_NEAREST|CLK_NORMALIZED_COORDS_FALSE|CLK_ADDRESS_NONE), coord);
}

INLINE float4 read_imagef(read_only image3d_t image, int4 coord) OVERLOADABLE
{
	return read_imagef(image, (CLK_FILTER_NEAREST|CLK_NORMALIZED_COORDS_FALSE|CLK_ADDRESS_NONE), coord);
}

INLINE int4 read_imagei(read_only image3d_t image, int4 coord) OVERLOADABLE
{
	return read_imagei(image, (CLK_FILTER_NEAREST|CLK_NORMALIZED_COORDS_FALSE|CLK_ADDRESS_NONE), coord);
}

INLINE uint4 read_imageui(read_only image3d_t image, int4 coord) OVERLOADABLE
{
	return read_imageui(image, (CLK_FILTER_NEAREST|CLK_NORMALIZED_COORDS_FALSE|CLK_ADDRESS_NONE), coord);
}

/*
 * 6.12.14.2 Built-in Image Read Functions:
 *
 * "The following built-in function calls to read images with a sampler are supported.
 *
 * * The built-in function calls to read images with a sampler are not supported for image1d_buffer_t image types."
 *
 */
INLINE float4 read_imagef(read_only image1d_t image, sampler_t sampler, float coord) OVERLOADABLE
{
	vc4cl_set_image_access_setup(image, vc4cl_apply_sampler(vc4cl_image_access_setup(image), sampler));
	int channel_type = get_image_channel_data_type(image);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
	int4 image_color = vc4cl_image_read(image, coord, type_size, channel_size);
	return vc4cl_convert_from_image_to_float(image_color, channel_type);
}

INLINE float4 read_imagef(read_only image1d_t image, sampler_t sampler, int coord) OVERLOADABLE
{
	float coords = vc4cl_normalize_coordinates(coord, get_image_width(image));
	return read_imagef(image, sampler, coords);
}

INLINE int4 read_imagei(read_only image1d_t image, sampler_t sampler, float coord) OVERLOADABLE
{
	vc4cl_set_image_access_setup(image, vc4cl_apply_sampler(vc4cl_image_access_setup(image), sampler));
	int channel_type = get_image_channel_data_type(image);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
	int4 image_color = vc4cl_image_read(image, coord, type_size, channel_size);
	return vc4cl_convert_from_image_to_int(image_color, channel_type);
}

INLINE int4 read_imagei(read_only image1d_t image, sampler_t sampler, int coord) OVERLOADABLE
{
	float coords = vc4cl_normalize_coordinates(coord, get_image_width(image));
	return read_imagei(image, sampler, coords);
}

INLINE uint4 read_imageui(read_only image1d_t image, sampler_t sampler, float coord) OVERLOADABLE
{
	vc4cl_set_image_access_setup(image, vc4cl_apply_sampler(vc4cl_image_access_setup(image), sampler));
	int channel_type = get_image_channel_data_type(image);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
	int4 image_color = vc4cl_image_read(image, coord, type_size, channel_size);
	return vc4cl_convert_from_image_to_uint(image_color, channel_type);
}

INLINE uint4 read_imageui(read_only image1d_t image, sampler_t sampler, int coord) OVERLOADABLE
{
	float coords = vc4cl_normalize_coordinates(coord, get_image_width(image));
	return read_imageui(image, sampler, coords);
}

INLINE float4 read_imagef(read_only image1d_array_t image, sampler_t sampler, float2 coord) OVERLOADABLE
{
	vc4cl_set_image_access_setup(image, vc4cl_apply_sampler(vc4cl_image_access_setup(image), sampler));
	int channel_type = get_image_channel_data_type(image);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
	int4 image_color = vc4cl_image_read(image, coord.x, vc4cl_get_array_index(coord.y, get_image_array_size(image)), type_size, channel_size);
	return vc4cl_convert_from_image_to_float(image_color, channel_type);
}

INLINE float4 read_imagef(read_only image1d_array_t image, sampler_t sampler, int2 coord) OVERLOADABLE
{
	float coords = vc4cl_normalize_coordinates(coord, get_image_width(image));
	return read_imagef(image, sampler, coords);
}

INLINE int4 read_imagei(read_only image1d_array_t image, sampler_t sampler, float2 coord) OVERLOADABLE
{
	vc4cl_set_image_access_setup(image, vc4cl_apply_sampler(vc4cl_image_access_setup(image), sampler));
	int channel_type = get_image_channel_data_type(image);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
	int4 image_color = vc4cl_image_read(image, coord.x, vc4cl_get_array_index(coord.y, get_image_array_size(image)), type_size, channel_size);
	return vc4cl_convert_from_image_to_int(image_color, channel_type);
}

INLINE int4 read_imagei(read_only image1d_array_t image, sampler_t sampler, int2 coord) OVERLOADABLE
{
	float coords = vc4cl_normalize_coordinates(coord, get_image_width(image));
	return read_imagei(image, sampler, coords);
}

INLINE uint4 read_imageui(read_only image1d_array_t image, sampler_t sampler, float2 coord) OVERLOADABLE
{
	vc4cl_set_image_access_setup(image, vc4cl_apply_sampler(vc4cl_image_access_setup(image), sampler));
	int channel_type = get_image_channel_data_type(image);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
	int4 image_color = vc4cl_image_read(image, coord.x, vc4cl_get_array_index(coord.y, get_image_array_size(image)), type_size, channel_size);
	return vc4cl_convert_from_image_to_uint(image_color, channel_type);
}

INLINE uint4 read_imageui(read_only image1d_array_t image, sampler_t sampler, int2 coord) OVERLOADABLE
{
	float coords = vc4cl_normalize_coordinates(coord, get_image_width(image));
	return read_imageui(image, sampler, coords);
}

INLINE float4 read_imagef(read_only image2d_t image, sampler_t sampler, float2 coord) OVERLOADABLE
{
	vc4cl_set_image_access_setup(image, vc4cl_apply_sampler(vc4cl_image_access_setup(image), sampler));
	int channel_type = get_image_channel_data_type(image);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
	int4 image_color = vc4cl_image_read(image, coord, type_size, channel_size);
	return vc4cl_convert_from_image_to_float(image_color, channel_type);
}

INLINE float4 read_imagef(read_only image2d_t image, sampler_t sampler, int2 coord) OVERLOADABLE
{
	float2 coords = vc4cl_normalize_coordinates(coord, (int2)(get_image_width(image), get_image_height(image)));
	return read_imagef(image, sampler, coords);
}

INLINE int4 read_imagei(read_only image2d_t image, sampler_t sampler, float2 coord) OVERLOADABLE
{
	vc4cl_set_image_access_setup(image, vc4cl_apply_sampler(vc4cl_image_access_setup(image), sampler));
	int channel_type = get_image_channel_data_type(image);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
	int4 image_color = vc4cl_image_read(image, coord, type_size, channel_size);
	return vc4cl_convert_from_image_to_int(image_color, channel_type);
}

INLINE int4 read_imagei(read_only image2d_t image, sampler_t sampler, int2 coord) OVERLOADABLE
{
	float2 coords = vc4cl_normalize_coordinates(coord, (int2)(get_image_width(image), get_image_height(image)));
	return read_imagei(image, sampler, coords);
}

INLINE uint4 read_imageui(read_only image2d_t image, sampler_t sampler, float2 coord) OVERLOADABLE
{
	vc4cl_set_image_access_setup(image, vc4cl_apply_sampler(vc4cl_image_access_setup(image), sampler));
	int channel_type = get_image_channel_data_type(image);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
	int4 image_color = vc4cl_image_read(image, coord, type_size, channel_size);
	return vc4cl_convert_from_image_to_uint(image_color, channel_type);
}

INLINE uint4 read_imageui(read_only image2d_t image, sampler_t sampler, int2 coord) OVERLOADABLE
{
	float2 coords = vc4cl_normalize_coordinates(coord, (int2)(get_image_width(image), get_image_height(image)));
	return read_imageui(image, sampler, coords);
}

INLINE float4 read_imagef(read_only image2d_array_t image, sampler_t sampler, float4 coord) OVERLOADABLE
{
	vc4cl_set_image_access_setup(image, vc4cl_apply_sampler(vc4cl_image_access_setup(image), sampler));
	int channel_type = get_image_channel_data_type(image);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
	int4 image_color = vc4cl_image_read(image, coord.xy, vc4cl_get_array_index(coord.z, get_image_array_size(image)), type_size, channel_size);
	return vc4cl_convert_from_image_to_float(image_color, channel_type);
}

INLINE float4 read_imagef(read_only image2d_array_t image, sampler_t sampler, int4 coord) OVERLOADABLE
{
	float2 coords = vc4cl_normalize_coordinates(coord, (int2)(get_image_width(image), get_image_height(image)));
	return read_imagef(image, sampler, coords);
}

INLINE int4 read_imagei(read_only image2d_array_t image, sampler_t sampler, float4 coord) OVERLOADABLE
{
	vc4cl_set_image_access_setup(image, vc4cl_apply_sampler(vc4cl_image_access_setup(image), sampler));
	int channel_type = get_image_channel_data_type(image);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
	int4 image_color = vc4cl_image_read(image, coord.xy, vc4cl_get_array_index(coord.z, get_image_array_size(image)), type_size, channel_size);
	return vc4cl_convert_from_image_to_int(image_color, channel_type);
}

INLINE int4 read_imagei(read_only image2d_array_t image, sampler_t sampler, int4 coord) OVERLOADABLE
{
	float2 coords = vc4cl_normalize_coordinates(coord, (int2)(get_image_width(image), get_image_height(image)));
	return read_imagei(image, sampler, coords);
}

INLINE uint4 read_imageui(read_only image2d_array_t image, sampler_t sampler, float4 coord) OVERLOADABLE
{
	vc4cl_set_image_access_setup(image, vc4cl_apply_sampler(vc4cl_image_access_setup(image), sampler));
	int channel_type = get_image_channel_data_type(image);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
	int4 image_color = vc4cl_image_read(image, coord.xy, vc4cl_get_array_index(coord.z, get_image_array_size(image)), type_size, channel_size);
	return vc4cl_convert_from_image_to_uint(image_color, channel_type);
}

INLINE uint4 read_imageui(read_only image2d_array_t image, sampler_t sampler, int4 coord) OVERLOADABLE
{
	float2 coords = vc4cl_normalize_coordinates(coord, (int2)(get_image_width(image), get_image_height(image)));
	return read_imageui(image, sampler, coords);
}

INLINE float4 read_imagef(read_only image3d_t image, sampler_t sampler, float4 coord) OVERLOADABLE
{
	vc4cl_set_image_access_setup(image, vc4cl_apply_sampler(vc4cl_image_access_setup(image), sampler));
	int channel_type = get_image_channel_data_type(image);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
	int4 image_color = vc4cl_image_read(image, coord, type_size, channel_size);
	return vc4cl_convert_from_image_to_float(image_color, channel_type);
}

float4 read_imagef(read_only image3d_t image, sampler_t sampler, int4 coord) OVERLOADABLE
{
	float4 coords = vc4cl_normalize_coordinates(coord, (int4)(get_image_width(image), get_image_height(image), get_image_depth(image), 0));
	return read_imagef(image, sampler, coords);
}

INLINE int4 read_imagei(read_only image3d_t image, sampler_t sampler, float4 coord) OVERLOADABLE
{
	vc4cl_set_image_access_setup(image, vc4cl_apply_sampler(vc4cl_image_access_setup(image), sampler));
	int channel_type = get_image_channel_data_type(image);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
	int4 image_color = vc4cl_image_read(image, coord, type_size, channel_size);
	return vc4cl_convert_from_image_to_int(image_color, channel_type);
}

INLINE int4 read_imagei(read_only image3d_t image, sampler_t sampler, int4 coord) OVERLOADABLE
{
	float4 coords = vc4cl_normalize_coordinates(coord, (int4)(get_image_width(image), get_image_height(image), get_image_depth(image), 0));
	return read_imagei(image, sampler, coords);
}

INLINE uint4 read_imageui(read_only image3d_t image, sampler_t sampler, float4 coord) OVERLOADABLE
{
	vc4cl_set_image_access_setup(image, vc4cl_apply_sampler(vc4cl_image_access_setup(image), sampler));
	int channel_type = get_image_channel_data_type(image);
	uint type_size = vc4cl_get_channel_type_size(channel_type);
	uint channel_size = vc4cl_get_channel_order_size(get_image_channel_order(image));
	int4 image_color = vc4cl_image_read(image, coord, type_size, channel_size);
	return vc4cl_convert_from_image_to_uint(image_color, channel_type);
}

INLINE uint4 read_imageui(read_only image3d_t image, sampler_t sampler, int4 coord) OVERLOADABLE
{
	float4 coords = vc4cl_normalize_coordinates(coord, (int4)(get_image_width(image), get_image_height(image), get_image_depth(image), 0));
	return read_imageui(image, sampler, coords);
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

INLINE CONST int get_image_width(read_only image1d_t image) OVERLOADABLE
{
	int width = (vc4cl_image_access_setup(image) >> 8) & 0x7FF;
	return width == 0 ? 2048 : width;
}
INLINE CONST int get_image_width(read_only image1d_buffer_t image) OVERLOADABLE
{
	int width = (vc4cl_image_access_setup(image) >> 8) & 0x7FF;
	return width == 0 ? 2048 : width;
}
INLINE CONST int get_image_width(read_only image2d_t image) OVERLOADABLE
{
	int width = (vc4cl_image_access_setup(image) >> 8) & 0x7FF;
	return width == 0 ? 2048 : width;
}
INLINE CONST int get_image_width(read_only image3d_t image) OVERLOADABLE
{
	int width = (vc4cl_image_access_setup(image) >> 8) & 0x7FF;
	return width == 0 ? 2048 : width;
}
INLINE CONST int get_image_width(read_only image1d_array_t image) OVERLOADABLE
{
	int width = (vc4cl_image_access_setup(image) >> 8) & 0x7FF;
	return width == 0 ? 2048 : width;
}
INLINE CONST int get_image_width(read_only image2d_array_t image) OVERLOADABLE
{
	int width = (vc4cl_image_access_setup(image) >> 8) & 0x7FF;
	return width == 0 ? 2048 : width;
}
INLINE CONST int get_image_width(write_only image1d_t image) OVERLOADABLE
{
	int width = (vc4cl_image_access_setup(image) >> 8) & 0x7FF;
	return width == 0 ? 2048 : width;
}
INLINE CONST int get_image_width(write_only image1d_buffer_t image) OVERLOADABLE
{
	int width = (vc4cl_image_access_setup(image) >> 8) & 0x7FF;
	return width == 0 ? 2048 : width;
}
INLINE CONST int get_image_width(write_only image2d_t image) OVERLOADABLE
{
	int width = (vc4cl_image_access_setup(image) >> 8) & 0x7FF;
	return width == 0 ? 2048 : width;
}
INLINE CONST int get_image_width(write_only image3d_t image) OVERLOADABLE
{
	int width = (vc4cl_image_access_setup(image) >> 8) & 0x7FF;
	return width == 0 ? 2048 : width;
}
INLINE CONST int get_image_width(write_only image1d_array_t image) OVERLOADABLE
{
	int width = (vc4cl_image_access_setup(image) >> 8) & 0x7FF;
	return width == 0 ? 2048 : width;
}
INLINE CONST int get_image_width(write_only image2d_array_t image) OVERLOADABLE
{
	int width = (vc4cl_image_access_setup(image) >> 8) & 0x7FF;
	return width == 0 ? 2048 : width;
}

INLINE CONST int get_image_height(read_only image2d_t image) OVERLOADABLE
{
	int height = (vc4cl_image_access_setup(image) >> 20) & 0x7FF;
	return height == 0 ? 2048 : height;
}
INLINE CONST int get_image_height(read_only image3d_t image) OVERLOADABLE
{
	int height = (vc4cl_image_access_setup(image) >> 20) & 0x7FF;
	return height == 0 ? 2048 : height;
}
INLINE CONST int get_image_height(read_only image2d_array_t image) OVERLOADABLE
{
	int height = (vc4cl_image_access_setup(image) >> 20) & 0x7FF;
	return height == 0 ? 2048 : height;
}
INLINE CONST int get_image_height(write_only image2d_t image) OVERLOADABLE
{
	int height = (vc4cl_image_access_setup(image) >> 20) & 0x7FF;
	return height == 0 ? 2048 : height;
}
INLINE CONST int get_image_height(write_only image3d_t image) OVERLOADABLE
{
	int height = (vc4cl_image_access_setup(image) >> 20) & 0x7FF;
	return height == 0 ? 2048 : height;
}
INLINE CONST int get_image_height(write_only image2d_array_t image) OVERLOADABLE
{
	int height = (vc4cl_image_access_setup(image) >> 20) & 0x7FF;
	return height == 0 ? 2048 : height;
}

CONST int get_image_depth(read_only image3d_t image) OVERLOADABLE;
CONST int get_image_depth(write_only image3d_t image) OVERLOADABLE;

INLINE CONST int2 get_image_dim(read_only image2d_t image) OVERLOADABLE
{
	return (int2)(get_image_width(image), get_image_height(image));
}
INLINE CONST int2 get_image_dim(read_only image2d_array_t image) OVERLOADABLE
{
	return (int2)(get_image_width(image), get_image_height(image));
}
INLINE CONST int2 get_image_dim(write_only image2d_t image) OVERLOADABLE
{
	return (int2)(get_image_width(image), get_image_height(image));
}
INLINE CONST int2 get_image_dim(write_only image2d_array_t image) OVERLOADABLE
{
	return (int2)(get_image_width(image), get_image_height(image));
}
INLINE CONST int4 get_image_dim(read_only image3d_t image) OVERLOADABLE
{
	return (int4)(get_image_width(image), get_image_height(image), get_image_depth(image), 0);
}
INLINE CONST int4 get_image_dim(write_only image3d_t image) OVERLOADABLE
{
	return (int4)(get_image_width(image), get_image_height(image), get_image_depth(image), 0);
}

/*
 * Don't map following functions to intrinsics, since they are handled quite nicely by SPIR-V
 */
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


//TODO how to handle? Need extra parameter??
CONST size_t get_image_array_size(read_only image1d_array_t image_array) OVERLOADABLE;
CONST size_t get_image_array_size(read_only image2d_array_t image_array) OVERLOADABLE;
CONST size_t get_image_array_size(write_only image1d_array_t image_array) OVERLOADABLE;
CONST size_t get_image_array_size(write_only image2d_array_t image_array) OVERLOADABLE;

#endif /* __IMAGE_SUPPORT__ */

#endif /* VC4CL_IMAGES_H */

