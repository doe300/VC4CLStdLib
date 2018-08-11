/*
 * Author: doe300
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef VC4CL_CONVERSIONS_H
#define VC4CL_CONVERSIONS_H

#include "_config.h"
#include "_overloads.h"
#include "_intrinsics.h"

// trick to allow concatenating macro (content!!) to symbol
#define CC(A, B) CONCAT_(A, B)
#define CONCAT_(A, B) A##B

#define uchar_MIN 0
#define uchar_MAX UCHAR_MAX
#define char_MIN SCHAR_MIN
#define char_MAX SCHAR_MAX
#define ushort_MIN 0
#define ushort_MAX USHRT_MAX
#define short_MIN SHRT_MIN
#define short_MAX SHRT_MAX
#define uint_MIN 0
#define uint_MAX UINT_MAX
#define int_MIN INT_MIN
#define int_MAX INT_MAX
#define float_MAX (int)FLT_MAX
#define float_MIN (int)FLT_MIN

//some macro tricks to save us from writing 2*4*2*8 = 128 single functions
#define _sat 1
#define _rte 1
#define _rtz 2
#define _rtp 3
#define _rtn 4

#define SATURATE_INTEGER(destType, srcType, num, val) \
  /* special case for uint as destination type */ \
  (uint)CC(destType,_MAX) == (uint) UINT_MAX ? \
    /* only need to clamp at lower bound (zero) */ \
    vc4cl_bitcast_int(vc4cl_max(vc4cl_bitcast_int(vc4cl_extend(val)), 0, VC4CL_SIGNED)) : \
  /* special case for uint as source type */ \
  (uint)CC(srcType,_MAX) == (uint) UINT_MAX ? \
    /* need to remove high-bit (sign-bit) */ \
    vc4cl_bitcast_int(clamp(vc4cl_bitcast_uint(vc4cl_extend(val)) > (uint##num)0x7FFFFFFF ? (int##num)0x7FFFFFFF : vc4cl_bitcast_int(vc4cl_extend(val)), (int##num)destType##_MIN, (int##num)destType##_MAX)) : \
  vc4cl_bitcast_int(clamp(vc4cl_bitcast_int(vc4cl_extend(val)), (int##num)destType##_MIN, (int##num)destType##_MAX)) \

//TODO is this correct? Esp. the bitcast?!
#define SATURATE_FLOAT(destType, num, val) \
  (vc4cl_bitcast_int(vc4cl_extend(val)) < vc4cl_bitcast_int((int##num)(destType##_MIN)) ? \
    vc4cl_bitcast_int(vc4cl_extend(destType##_MIN)) : \
  (vc4cl_bitcast_int(vc4cl_extend(val)) > vc4cl_bitcast_int((int##num)(destType##_MAX)) ? \
    vc4cl_bitcast_int(vc4cl_extend(destType##_MAX)) : \
  vc4cl_bitcast_int(vc4cl_extend(val))))

#define ROUND_TO_INTEGER(mode, val) \
  (0##mode == _rte ? \
    rint(val) : \
  (0##mode == _rtz ? \
    trunc(val) : \
  (0##mode == _rtp ? \
    ceil(val) : \
  (0##mode == _rtn ? \
    floor(val) : \
  val))))

#define CONVERSION_WITH_SATURATION(destType, srcType, num, saturation, val) \
  0##saturation == _sat ? \
    SATURATE_INTEGER(destType, srcType, num, val) : \
  vc4cl_bitcast_int(vc4cl_extend(val))

#define CONVERSION_WITH_SATURATION_FLOAT(destType, num, saturation, val) \
  0##saturation == _sat ? \
    SATURATE_FLOAT(destType, num, val) : \
  vc4cl_bitcast_int(vc4cl_extend(val))

#define CONVERT_INTEGER(destType, srcType, saturation, rounding) \
        INLINE destType convert_##destType##saturation##rounding(srcType val) OVERLOADABLE CONST \
        { \
            return vc4cl_bitcast_##destType(CONVERSION_WITH_SATURATION(destType, srcType, /* scalar */, saturation, val)); \
        } \
        INLINE destType##2 convert_##destType##2##saturation##rounding(srcType##2 val) OVERLOADABLE CONST \
        { \
            return vc4cl_bitcast_##destType(CONVERSION_WITH_SATURATION(destType, srcType, 2, saturation, val)); \
        } \
        INLINE destType##3 convert_##destType##3##saturation##rounding(srcType##3 val) OVERLOADABLE CONST \
        { \
            return vc4cl_bitcast_##destType(CONVERSION_WITH_SATURATION(destType, srcType, 3, saturation, val)); \
        } \
        INLINE destType##4 convert_##destType##4##saturation##rounding(srcType##4 val) OVERLOADABLE CONST \
        { \
            return vc4cl_bitcast_##destType(CONVERSION_WITH_SATURATION(destType, srcType, 4, saturation, val)); \
        } \
        INLINE destType##8 convert_##destType##8##saturation##rounding(srcType##8 val) OVERLOADABLE CONST \
        { \
            return vc4cl_bitcast_##destType(CONVERSION_WITH_SATURATION(destType, srcType, 8, saturation, val)); \
        } \
        INLINE destType##16 convert_##destType##16##saturation##rounding(srcType##16 val) OVERLOADABLE CONST \
        { \
            return vc4cl_bitcast_##destType(CONVERSION_WITH_SATURATION(destType, srcType, 16, saturation, val)); \
        }

/*
* Out-of-range behavior of float-to-integer conversion is implementation specified (OpenCL 1.2, section 6.2.3.3)
* -> we always saturate
*/
#define CONVERT_FLOAT_TO_INTEGER(destType, saturation, rounding) \
        INLINE destType convert_##destType##saturation##rounding(float val) OVERLOADABLE CONST \
        { \
            int saturatedInt = val >= 2147483648.0f ? 0x7FFFFFFF : val <= -2147483648.0f ? 0x80000000 : vc4cl_ftoi(ROUND_TO_INTEGER(rounding, val)); \
            return vc4cl_bitcast_##destType(CONVERSION_WITH_SATURATION(destType, int, /* scalar */, saturation, saturatedInt)); \
        } \
        INLINE destType##2 convert_##destType##2##saturation##rounding(float##2 val) OVERLOADABLE CONST \
        { \
            int2 saturatedInt = val >= 2147483648.0f ? (int2)0x7FFFFFFF : val <= -2147483648.0f ? (int2)0x80000000 : vc4cl_ftoi(ROUND_TO_INTEGER(rounding, val)); \
            return vc4cl_bitcast_##destType(CONVERSION_WITH_SATURATION(destType, int, 2, saturation, saturatedInt)); \
        } \
        INLINE destType##3 convert_##destType##3##saturation##rounding(float##3 val) OVERLOADABLE CONST \
        { \
            int3 saturatedInt = val >= 2147483648.0f ? (int3)0x7FFFFFFF : val <= -2147483648.0f ? (int3)0x80000000 : vc4cl_ftoi(ROUND_TO_INTEGER(rounding, val)); \
            return vc4cl_bitcast_##destType(CONVERSION_WITH_SATURATION(destType, int, 3, saturation, saturatedInt)); \
        } \
        INLINE destType##4 convert_##destType##4##saturation##rounding(float##4 val) OVERLOADABLE CONST \
        { \
            int4 saturatedInt = val >= 2147483648.0f ? (int4)0x7FFFFFFF : val <= -2147483648.0f ? (int4)0x80000000 : vc4cl_ftoi(ROUND_TO_INTEGER(rounding, val)); \
            return vc4cl_bitcast_##destType(CONVERSION_WITH_SATURATION(destType, int, 4, saturation, saturatedInt)); \
        } \
        INLINE destType##8 convert_##destType##8##saturation##rounding(float##8 val) OVERLOADABLE CONST \
        { \
            int8 saturatedInt = val >= 2147483648.0f ? (int8)0x7FFFFFFF : val <= -2147483648.0f ? (int8)0x80000000 : vc4cl_ftoi(ROUND_TO_INTEGER(rounding, val)); \
            return vc4cl_bitcast_##destType(CONVERSION_WITH_SATURATION(destType, int, 8, saturation, saturatedInt)); \
        } \
        INLINE destType##16 convert_##destType##16##saturation##rounding(float##16 val) OVERLOADABLE CONST \
        { \
            int16 saturatedInt = val >= 2147483648.0f ? (int16)0x7FFFFFFF : val <= -2147483648.0f ? (int16)0x80000000 : vc4cl_ftoi(ROUND_TO_INTEGER(rounding, val)); \
            return vc4cl_bitcast_##destType(CONVERSION_WITH_SATURATION(destType, int, 16, saturation, saturatedInt)); \
        }

#define CONVERT_INTEGER_TO_FLOAT(srcType, saturation, rounding) \
        INLINE float convert_float##saturation##rounding(srcType val) OVERLOADABLE CONST \
        { \
            return vc4cl_itof(CONVERSION_WITH_SATURATION_FLOAT(float, /* scalar */, saturation, val)); \
        } \
        INLINE float##2 convert_float2##saturation##rounding(srcType##2 val) OVERLOADABLE CONST \
        { \
            return vc4cl_itof(CONVERSION_WITH_SATURATION_FLOAT(float, 2, saturation, val)); \
        } \
        INLINE float##3 convert_float3##saturation##rounding(srcType##3 val) OVERLOADABLE CONST \
        { \
            return vc4cl_itof(CONVERSION_WITH_SATURATION_FLOAT(float, 3, saturation, val)); \
        } \
        INLINE float##4 convert_float4##saturation##rounding(srcType##4 val) OVERLOADABLE CONST \
        { \
            return vc4cl_itof(CONVERSION_WITH_SATURATION_FLOAT(float, 4, saturation, val)); \
        } \
        INLINE float##8 convert_float8##saturation##rounding(srcType##8 val) OVERLOADABLE CONST \
        { \
            return vc4cl_itof(CONVERSION_WITH_SATURATION_FLOAT(float, 8, saturation, val)); \
        } \
        INLINE float##16 convert_float16##saturation##rounding(srcType##16 val) OVERLOADABLE CONST \
        { \
            return vc4cl_itof(CONVERSION_WITH_SATURATION_FLOAT(float, 16, saturation, val)); \
        }

#define CONVERT_FLOAT_TO_FLOAT(saturation, rounding) \
        INLINE float convert_float##saturation##rounding(float val) OVERLOADABLE CONST \
        { \
            return ROUND_TO_INTEGER(rounding, val); \
        } \
        INLINE float##2 convert_float2##saturation##rounding(float##2 val) OVERLOADABLE CONST \
        { \
            return ROUND_TO_INTEGER(rounding, val); \
        } \
        INLINE float##3 convert_float3##saturation##rounding(float##3 val) OVERLOADABLE CONST \
        { \
            return ROUND_TO_INTEGER(rounding, val); \
        } \
        INLINE float##4 convert_float4##saturation##rounding(float##4 val) OVERLOADABLE CONST \
        { \
            return ROUND_TO_INTEGER(rounding, val); \
        } \
        INLINE float##8 convert_float8##saturation##rounding(float##8 val) OVERLOADABLE CONST \
        { \
            return ROUND_TO_INTEGER(rounding, val); \
        } \
        INLINE float##16 convert_float16##saturation##rounding(float##16 val) OVERLOADABLE CONST \
        { \
            return ROUND_TO_INTEGER(rounding, val); \
        }

/*
 * Need special handling for values > INT_MAX, since itof() is signed
 */
//XXX could be wrong in very rare cases if "original" conversion would round differently due to cut off last bit
#define CONVERT_UINT_TO_FLOAT(saturation, rounding) \
        INLINE float convert_float##saturation##rounding(uint val) OVERLOADABLE CONST \
        { \
            /* For the rounding mode: find */ \
            return vc4cl_msb_set(val) ? vc4cl_itof(vc4cl_bitcast_int(val >> 1)) * 2.0f : vc4cl_itof(vc4cl_bitcast_int(val)); \
        } \
        INLINE float##2 convert_float2##saturation##rounding(uint2 val) OVERLOADABLE CONST \
        { \
            return vc4cl_msb_set(val) ? vc4cl_itof(vc4cl_bitcast_int(val >> 1)) * 2.0f : vc4cl_itof(vc4cl_bitcast_int(val)); \
        } \
        INLINE float##3 convert_float3##saturation##rounding(uint3 val) OVERLOADABLE CONST \
        { \
            return vc4cl_msb_set(val) ? vc4cl_itof(vc4cl_bitcast_int(val >> 1)) * 2.0f : vc4cl_itof(vc4cl_bitcast_int(val)); \
        } \
        INLINE float##4 convert_float4##saturation##rounding(uint4 val) OVERLOADABLE CONST \
        { \
            return vc4cl_msb_set(val) ? vc4cl_itof(vc4cl_bitcast_int(val >> 1)) * 2.0f : vc4cl_itof(vc4cl_bitcast_int(val)); \
        } \
        INLINE float##8 convert_float8##saturation##rounding(uint8 val) OVERLOADABLE CONST \
        { \
            return vc4cl_msb_set(val) ? vc4cl_itof(vc4cl_bitcast_int(val >> 1)) * 2.0f : vc4cl_itof(vc4cl_bitcast_int(val)); \
        } \
        INLINE float##16 convert_float16##saturation##rounding(uint16 val) OVERLOADABLE CONST \
        { \
            return vc4cl_msb_set(val) ? vc4cl_itof(vc4cl_bitcast_int(val >> 1)) * 2.0f : vc4cl_itof(vc4cl_bitcast_int(val)); \
        }

/*
 * Out-of-range behavior of float-to-integer conversion is implementation specified (OpenCL 1.2, section 6.2.3.3)
 * -> we always saturate
 */
//TODO depending on what the VC4 does with out-of-range floating-point values, this could be optimized
#define CONVERT_FLOAT_TO_UINT(saturation, rounding) \
        INLINE uint convert_uint##saturation##rounding(float val) OVERLOADABLE CONST \
        { \
              return val <= 0 ? \
                0 : \
              val >= 4294967296.0f ? \
                0xFFFFFFFFu : \
              val >= 2147483648.0f ? \
                vc4cl_bitcast_uint(vc4cl_ftoi(val * 0.5f)) << 1 : \
              vc4cl_bitcast_uint(vc4cl_ftoi(val)); \
        } \
        INLINE uint##2 convert_uint2##saturation##rounding(float2 val) OVERLOADABLE CONST \
        { \
            return val <= 0 ? \
                  0 : \
            val >= 4294967296.0f ? \
                  0xFFFFFFFFu : \
            val >= 2147483648.0f ? \
                  vc4cl_bitcast_uint(vc4cl_ftoi(val * 0.5f)) << 1 : \
            vc4cl_bitcast_uint(vc4cl_ftoi(val)); \
        } \
        INLINE uint##3 convert_uint3##saturation##rounding(float3 val) OVERLOADABLE CONST \
        { \
            return val <= 0 ? \
                  0 : \
            val >= 4294967296.0f ? \
                  0xFFFFFFFFu : \
            val >= 2147483648.0f ? \
                  vc4cl_bitcast_uint(vc4cl_ftoi(val * 0.5f)) << 1 : \
            vc4cl_bitcast_uint(vc4cl_ftoi(val)); \
        } \
        INLINE uint##4 convert_uint4##saturation##rounding(float4 val) OVERLOADABLE CONST \
        { \
            return val <= 0 ? \
                  0 : \
            val >= 4294967296.0f ? \
                  0xFFFFFFFFu : \
            val >= 2147483648.0f ? \
                  vc4cl_bitcast_uint(vc4cl_ftoi(val * 0.5f)) << 1 : \
            vc4cl_bitcast_uint(vc4cl_ftoi(val)); \
        } \
        INLINE uint##8 convert_uint8##saturation##rounding(float8 val) OVERLOADABLE CONST \
        { \
            return val <= 0 ? \
                  0 : \
            val >= 4294967296.0f ? \
                  0xFFFFFFFFu : \
            val >= 2147483648.0f ? \
                  vc4cl_bitcast_uint(vc4cl_ftoi(val * 0.5f)) << 1 : \
            vc4cl_bitcast_uint(vc4cl_ftoi(val)); \
        } \
        INLINE uint##16 convert_uint16##saturation##rounding(float16 val) OVERLOADABLE CONST \
        { \
            return val <= 0 ? \
                  0 : \
            val >= 4294967296.0f ? \
                  0xFFFFFFFFu : \
            val >= 2147483648.0f ? \
                  vc4cl_bitcast_uint(vc4cl_ftoi(val * 0.5f)) << 1 : \
            vc4cl_bitcast_uint(vc4cl_ftoi(val)); \
        }

/*
 * To uchar
 */
CONVERT_INTEGER(uchar, uchar, /* no saturation*/, /* no rounding */)
CONVERT_INTEGER(uchar, uchar, /* no saturation*/, _rte)
CONVERT_INTEGER(uchar, uchar, /* no saturation*/, _rtz)
CONVERT_INTEGER(uchar, uchar, /* no saturation*/, _rtp)
CONVERT_INTEGER(uchar, uchar, /* no saturation*/, _rtn)
CONVERT_INTEGER(uchar, uchar, _sat, /* no rounding */)
CONVERT_INTEGER(uchar, uchar, _sat, _rte)
CONVERT_INTEGER(uchar, uchar, _sat, _rtz)
CONVERT_INTEGER(uchar, uchar, _sat, _rtp)
CONVERT_INTEGER(uchar, uchar, _sat, _rtn)
CONVERT_INTEGER(uchar, char, /* no saturation*/, /* no rounding */)
CONVERT_INTEGER(uchar, char, /* no saturation*/, _rte)
CONVERT_INTEGER(uchar, char, /* no saturation*/, _rtz)
CONVERT_INTEGER(uchar, char, /* no saturation*/, _rtp)
CONVERT_INTEGER(uchar, char, /* no saturation*/, _rtn)
CONVERT_INTEGER(uchar, char, _sat, /* no rounding */)
CONVERT_INTEGER(uchar, char, _sat, _rte)
CONVERT_INTEGER(uchar, char, _sat, _rtz)
CONVERT_INTEGER(uchar, char, _sat, _rtp)
CONVERT_INTEGER(uchar, char, _sat, _rtn)
CONVERT_INTEGER(uchar, ushort, /* no saturation*/, /* no rounding */)
CONVERT_INTEGER(uchar, ushort, /* no saturation*/, _rte)
CONVERT_INTEGER(uchar, ushort, /* no saturation*/, _rtz)
CONVERT_INTEGER(uchar, ushort, /* no saturation*/, _rtp)
CONVERT_INTEGER(uchar, ushort, /* no saturation*/, _rtn)
CONVERT_INTEGER(uchar, ushort, _sat, /* no rounding */)
CONVERT_INTEGER(uchar, ushort, _sat, _rte)
CONVERT_INTEGER(uchar, ushort, _sat, _rtz)
CONVERT_INTEGER(uchar, ushort, _sat, _rtp)
CONVERT_INTEGER(uchar, ushort, _sat, _rtn)
CONVERT_INTEGER(uchar, short, /* no saturation*/, /* no rounding */)
CONVERT_INTEGER(uchar, short, /* no saturation*/, _rte)
CONVERT_INTEGER(uchar, short, /* no saturation*/, _rtz)
CONVERT_INTEGER(uchar, short, /* no saturation*/, _rtp)
CONVERT_INTEGER(uchar, short, /* no saturation*/, _rtn)
CONVERT_INTEGER(uchar, short, _sat, /* no rounding */)
CONVERT_INTEGER(uchar, short, _sat, _rte)
CONVERT_INTEGER(uchar, short, _sat, _rtz)
CONVERT_INTEGER(uchar, short, _sat, _rtp)
CONVERT_INTEGER(uchar, short, _sat, _rtn)
CONVERT_INTEGER(uchar, uint, /* no saturation*/, /* no rounding */)
CONVERT_INTEGER(uchar, uint, /* no saturation*/, _rte)
CONVERT_INTEGER(uchar, uint, /* no saturation*/, _rtz)
CONVERT_INTEGER(uchar, uint, /* no saturation*/, _rtp)
CONVERT_INTEGER(uchar, uint, /* no saturation*/, _rtn)
CONVERT_INTEGER(uchar, uint, _sat, /* no rounding */)
CONVERT_INTEGER(uchar, uint, _sat, _rte)
CONVERT_INTEGER(uchar, uint, _sat, _rtz)
CONVERT_INTEGER(uchar, uint, _sat, _rtp)
CONVERT_INTEGER(uchar, uint, _sat, _rtn)
CONVERT_INTEGER(uchar, int, /* no saturation*/, /* no rounding */)
CONVERT_INTEGER(uchar, int, /* no saturation*/, _rte)
CONVERT_INTEGER(uchar, int, /* no saturation*/, _rtz)
CONVERT_INTEGER(uchar, int, /* no saturation*/, _rtp)
CONVERT_INTEGER(uchar, int, /* no saturation*/, _rtn)
CONVERT_INTEGER(uchar, int, _sat, /* no rounding */)
CONVERT_INTEGER(uchar, int, _sat, _rte)
CONVERT_INTEGER(uchar, int, _sat, _rtz)
CONVERT_INTEGER(uchar, int, _sat, _rtp)
CONVERT_INTEGER(uchar, int, _sat, _rtn)
CONVERT_FLOAT_TO_INTEGER(uchar, /* no saturation*/, /* no rounding */)
CONVERT_FLOAT_TO_INTEGER(uchar, /* no saturation*/, _rte)
CONVERT_FLOAT_TO_INTEGER(uchar, /* no saturation*/, _rtz)
CONVERT_FLOAT_TO_INTEGER(uchar, /* no saturation*/, _rtp)
CONVERT_FLOAT_TO_INTEGER(uchar, /* no saturation*/, _rtn)
CONVERT_FLOAT_TO_INTEGER(uchar, _sat, /* no rounding */)
CONVERT_FLOAT_TO_INTEGER(uchar, _sat, _rte)
CONVERT_FLOAT_TO_INTEGER(uchar, _sat, _rtz)
CONVERT_FLOAT_TO_INTEGER(uchar, _sat, _rtp)
CONVERT_FLOAT_TO_INTEGER(uchar, _sat, _rtn)

/*
 * To char
 */
CONVERT_INTEGER(char, uchar, /* no saturation*/, /* no rounding */)
CONVERT_INTEGER(char, uchar, /* no saturation*/, _rte)
CONVERT_INTEGER(char, uchar, /* no saturation*/, _rtz)
CONVERT_INTEGER(char, uchar, /* no saturation*/, _rtp)
CONVERT_INTEGER(char, uchar, /* no saturation*/, _rtn)
CONVERT_INTEGER(char, uchar, _sat, /* no rounding */)
CONVERT_INTEGER(char, uchar, _sat, _rte)
CONVERT_INTEGER(char, uchar, _sat, _rtz)
CONVERT_INTEGER(char, uchar, _sat, _rtp)
CONVERT_INTEGER(char, uchar, _sat, _rtn)
CONVERT_INTEGER(char, char, /* no saturation*/, /* no rounding */)
CONVERT_INTEGER(char, char, /* no saturation*/, _rte)
CONVERT_INTEGER(char, char, /* no saturation*/, _rtz)
CONVERT_INTEGER(char, char, /* no saturation*/, _rtp)
CONVERT_INTEGER(char, char, /* no saturation*/, _rtn)
CONVERT_INTEGER(char, char, _sat, /* no rounding */)
CONVERT_INTEGER(char, char, _sat, _rte)
CONVERT_INTEGER(char, char, _sat, _rtz)
CONVERT_INTEGER(char, char, _sat, _rtp)
CONVERT_INTEGER(char, char, _sat, _rtn)
CONVERT_INTEGER(char, ushort, /* no saturation*/, /* no rounding */)
CONVERT_INTEGER(char, ushort, /* no saturation*/, _rte)
CONVERT_INTEGER(char, ushort, /* no saturation*/, _rtz)
CONVERT_INTEGER(char, ushort, /* no saturation*/, _rtp)
CONVERT_INTEGER(char, ushort, /* no saturation*/, _rtn)
CONVERT_INTEGER(char, ushort, _sat, /* no rounding */)
CONVERT_INTEGER(char, ushort, _sat, _rte)
CONVERT_INTEGER(char, ushort, _sat, _rtz)
CONVERT_INTEGER(char, ushort, _sat, _rtp)
CONVERT_INTEGER(char, ushort, _sat, _rtn)
CONVERT_INTEGER(char, short, /* no saturation*/, /* no rounding */)
CONVERT_INTEGER(char, short, /* no saturation*/, _rte)
CONVERT_INTEGER(char, short, /* no saturation*/, _rtz)
CONVERT_INTEGER(char, short, /* no saturation*/, _rtp)
CONVERT_INTEGER(char, short, /* no saturation*/, _rtn)
CONVERT_INTEGER(char, short, _sat, /* no rounding */)
CONVERT_INTEGER(char, short, _sat, _rte)
CONVERT_INTEGER(char, short, _sat, _rtz)
CONVERT_INTEGER(char, short, _sat, _rtp)
CONVERT_INTEGER(char, short, _sat, _rtn)
CONVERT_INTEGER(char, uint, /* no saturation*/, /* no rounding */)
CONVERT_INTEGER(char, uint, /* no saturation*/, _rte)
CONVERT_INTEGER(char, uint, /* no saturation*/, _rtz)
CONVERT_INTEGER(char, uint, /* no saturation*/, _rtp)
CONVERT_INTEGER(char, uint, /* no saturation*/, _rtn)
CONVERT_INTEGER(char, uint, _sat, /* no rounding */)
CONVERT_INTEGER(char, uint, _sat, _rte)
CONVERT_INTEGER(char, uint, _sat, _rtz)
CONVERT_INTEGER(char, uint, _sat, _rtp)
CONVERT_INTEGER(char, uint, _sat, _rtn)
CONVERT_INTEGER(char, int, /* no saturation*/, /* no rounding */)
CONVERT_INTEGER(char, int, /* no saturation*/, _rte)
CONVERT_INTEGER(char, int, /* no saturation*/, _rtz)
CONVERT_INTEGER(char, int, /* no saturation*/, _rtp)
CONVERT_INTEGER(char, int, /* no saturation*/, _rtn)
CONVERT_INTEGER(char, int, _sat, /* no rounding */)
CONVERT_INTEGER(char, int, _sat, _rte)
CONVERT_INTEGER(char, int, _sat, _rtz)
CONVERT_INTEGER(char, int, _sat, _rtp)
CONVERT_INTEGER(char, int, _sat, _rtn)
CONVERT_FLOAT_TO_INTEGER(char, /* no saturation*/, /* no rounding */)
CONVERT_FLOAT_TO_INTEGER(char, /* no saturation*/, _rte)
CONVERT_FLOAT_TO_INTEGER(char, /* no saturation*/, _rtz)
CONVERT_FLOAT_TO_INTEGER(char, /* no saturation*/, _rtp)
CONVERT_FLOAT_TO_INTEGER(char, /* no saturation*/, _rtn)
CONVERT_FLOAT_TO_INTEGER(char, _sat, /* no rounding */)
CONVERT_FLOAT_TO_INTEGER(char, _sat, _rte)
CONVERT_FLOAT_TO_INTEGER(char, _sat, _rtz)
CONVERT_FLOAT_TO_INTEGER(char, _sat, _rtp)
CONVERT_FLOAT_TO_INTEGER(char, _sat, _rtn)

/*
 * To ushort
 */
CONVERT_INTEGER(ushort, uchar, /* no saturation*/, /* no rounding */)
CONVERT_INTEGER(ushort, uchar, /* no saturation*/, _rte)
CONVERT_INTEGER(ushort, uchar, /* no saturation*/, _rtz)
CONVERT_INTEGER(ushort, uchar, /* no saturation*/, _rtp)
CONVERT_INTEGER(ushort, uchar, /* no saturation*/, _rtn)
CONVERT_INTEGER(ushort, uchar, _sat, /* no rounding */)
CONVERT_INTEGER(ushort, uchar, _sat, _rte)
CONVERT_INTEGER(ushort, uchar, _sat, _rtz)
CONVERT_INTEGER(ushort, uchar, _sat, _rtp)
CONVERT_INTEGER(ushort, uchar, _sat, _rtn)
CONVERT_INTEGER(ushort, char, /* no saturation*/, /* no rounding */)
CONVERT_INTEGER(ushort, char, /* no saturation*/, _rte)
CONVERT_INTEGER(ushort, char, /* no saturation*/, _rtz)
CONVERT_INTEGER(ushort, char, /* no saturation*/, _rtp)
CONVERT_INTEGER(ushort, char, /* no saturation*/, _rtn)
CONVERT_INTEGER(ushort, char, _sat, /* no rounding */)
CONVERT_INTEGER(ushort, char, _sat, _rte)
CONVERT_INTEGER(ushort, char, _sat, _rtz)
CONVERT_INTEGER(ushort, char, _sat, _rtp)
CONVERT_INTEGER(ushort, char, _sat, _rtn)
CONVERT_INTEGER(ushort, ushort, /* no saturation*/, /* no rounding */)
CONVERT_INTEGER(ushort, ushort, /* no saturation*/, _rte)
CONVERT_INTEGER(ushort, ushort, /* no saturation*/, _rtz)
CONVERT_INTEGER(ushort, ushort, /* no saturation*/, _rtp)
CONVERT_INTEGER(ushort, ushort, /* no saturation*/, _rtn)
CONVERT_INTEGER(ushort, ushort, _sat, /* no rounding */)
CONVERT_INTEGER(ushort, ushort, _sat, _rte)
CONVERT_INTEGER(ushort, ushort, _sat, _rtz)
CONVERT_INTEGER(ushort, ushort, _sat, _rtp)
CONVERT_INTEGER(ushort, ushort, _sat, _rtn)
CONVERT_INTEGER(ushort, short, /* no saturation*/, /* no rounding */)
CONVERT_INTEGER(ushort, short, /* no saturation*/, _rte)
CONVERT_INTEGER(ushort, short, /* no saturation*/, _rtz)
CONVERT_INTEGER(ushort, short, /* no saturation*/, _rtp)
CONVERT_INTEGER(ushort, short, /* no saturation*/, _rtn)
CONVERT_INTEGER(ushort, short, _sat, /* no rounding */)
CONVERT_INTEGER(ushort, short, _sat, _rte)
CONVERT_INTEGER(ushort, short, _sat, _rtz)
CONVERT_INTEGER(ushort, short, _sat, _rtp)
CONVERT_INTEGER(ushort, short, _sat, _rtn)
CONVERT_INTEGER(ushort, uint, /* no saturation*/, /* no rounding */)
CONVERT_INTEGER(ushort, uint, /* no saturation*/, _rte)
CONVERT_INTEGER(ushort, uint, /* no saturation*/, _rtz)
CONVERT_INTEGER(ushort, uint, /* no saturation*/, _rtp)
CONVERT_INTEGER(ushort, uint, /* no saturation*/, _rtn)
CONVERT_INTEGER(ushort, uint, _sat, /* no rounding */)
CONVERT_INTEGER(ushort, uint, _sat, _rte)
CONVERT_INTEGER(ushort, uint, _sat, _rtz)
CONVERT_INTEGER(ushort, uint, _sat, _rtp)
CONVERT_INTEGER(ushort, uint, _sat, _rtn)
CONVERT_INTEGER(ushort, int, /* no saturation*/, /* no rounding */)
CONVERT_INTEGER(ushort, int, /* no saturation*/, _rte)
CONVERT_INTEGER(ushort, int, /* no saturation*/, _rtz)
CONVERT_INTEGER(ushort, int, /* no saturation*/, _rtp)
CONVERT_INTEGER(ushort, int, /* no saturation*/, _rtn)
CONVERT_INTEGER(ushort, int, _sat, /* no rounding */)
CONVERT_INTEGER(ushort, int, _sat, _rte)
CONVERT_INTEGER(ushort, int, _sat, _rtz)
CONVERT_INTEGER(ushort, int, _sat, _rtp)
CONVERT_INTEGER(ushort, int, _sat, _rtn)
CONVERT_FLOAT_TO_INTEGER(ushort, /* no saturation*/, /* no rounding */)
CONVERT_FLOAT_TO_INTEGER(ushort, /* no saturation*/, _rte)
CONVERT_FLOAT_TO_INTEGER(ushort, /* no saturation*/, _rtz)
CONVERT_FLOAT_TO_INTEGER(ushort, /* no saturation*/, _rtp)
CONVERT_FLOAT_TO_INTEGER(ushort, /* no saturation*/, _rtn)
CONVERT_FLOAT_TO_INTEGER(ushort, _sat, /* no rounding */)
CONVERT_FLOAT_TO_INTEGER(ushort, _sat, _rte)
CONVERT_FLOAT_TO_INTEGER(ushort, _sat, _rtz)
CONVERT_FLOAT_TO_INTEGER(ushort, _sat, _rtp)
CONVERT_FLOAT_TO_INTEGER(ushort, _sat, _rtn)

/*
 * To short
 */
CONVERT_INTEGER(short, uchar, /* no saturation*/, /* no rounding */)
CONVERT_INTEGER(short, uchar, /* no saturation*/, _rte)
CONVERT_INTEGER(short, uchar, /* no saturation*/, _rtz)
CONVERT_INTEGER(short, uchar, /* no saturation*/, _rtp)
CONVERT_INTEGER(short, uchar, /* no saturation*/, _rtn)
CONVERT_INTEGER(short, uchar, _sat, /* no rounding */)
CONVERT_INTEGER(short, uchar, _sat, _rte)
CONVERT_INTEGER(short, uchar, _sat, _rtz)
CONVERT_INTEGER(short, uchar, _sat, _rtp)
CONVERT_INTEGER(short, uchar, _sat, _rtn)
CONVERT_INTEGER(short, char, /* no saturation*/, /* no rounding */)
CONVERT_INTEGER(short, char, /* no saturation*/, _rte)
CONVERT_INTEGER(short, char, /* no saturation*/, _rtz)
CONVERT_INTEGER(short, char, /* no saturation*/, _rtp)
CONVERT_INTEGER(short, char, /* no saturation*/, _rtn)
CONVERT_INTEGER(short, char, _sat, /* no rounding */)
CONVERT_INTEGER(short, char, _sat, _rte)
CONVERT_INTEGER(short, char, _sat, _rtz)
CONVERT_INTEGER(short, char, _sat, _rtp)
CONVERT_INTEGER(short, char, _sat, _rtn)
CONVERT_INTEGER(short, ushort, /* no saturation*/, /* no rounding */)
CONVERT_INTEGER(short, ushort, /* no saturation*/, _rte)
CONVERT_INTEGER(short, ushort, /* no saturation*/, _rtz)
CONVERT_INTEGER(short, ushort, /* no saturation*/, _rtp)
CONVERT_INTEGER(short, ushort, /* no saturation*/, _rtn)
CONVERT_INTEGER(short, ushort, _sat, /* no rounding */)
CONVERT_INTEGER(short, ushort, _sat, _rte)
CONVERT_INTEGER(short, ushort, _sat, _rtz)
CONVERT_INTEGER(short, ushort, _sat, _rtp)
CONVERT_INTEGER(short, ushort, _sat, _rtn)
CONVERT_INTEGER(short, short, /* no saturation*/, /* no rounding */)
CONVERT_INTEGER(short, short, /* no saturation*/, _rte)
CONVERT_INTEGER(short, short, /* no saturation*/, _rtz)
CONVERT_INTEGER(short, short, /* no saturation*/, _rtp)
CONVERT_INTEGER(short, short, /* no saturation*/, _rtn)
CONVERT_INTEGER(short, short, _sat, /* no rounding */)
CONVERT_INTEGER(short, short, _sat, _rte)
CONVERT_INTEGER(short, short, _sat, _rtz)
CONVERT_INTEGER(short, short, _sat, _rtp)
CONVERT_INTEGER(short, short, _sat, _rtn)
CONVERT_INTEGER(short, uint, /* no saturation*/, /* no rounding */)
CONVERT_INTEGER(short, uint, /* no saturation*/, _rte)
CONVERT_INTEGER(short, uint, /* no saturation*/, _rtz)
CONVERT_INTEGER(short, uint, /* no saturation*/, _rtp)
CONVERT_INTEGER(short, uint, /* no saturation*/, _rtn)
CONVERT_INTEGER(short, uint, _sat, /* no rounding */)
CONVERT_INTEGER(short, uint, _sat, _rte)
CONVERT_INTEGER(short, uint, _sat, _rtz)
CONVERT_INTEGER(short, uint, _sat, _rtp)
CONVERT_INTEGER(short, uint, _sat, _rtn)
CONVERT_INTEGER(short, int, /* no saturation*/, /* no rounding */)
CONVERT_INTEGER(short, int, /* no saturation*/, _rte)
CONVERT_INTEGER(short, int, /* no saturation*/, _rtz)
CONVERT_INTEGER(short, int, /* no saturation*/, _rtp)
CONVERT_INTEGER(short, int, /* no saturation*/, _rtn)
CONVERT_INTEGER(short, int, _sat, /* no rounding */)
CONVERT_INTEGER(short, int, _sat, _rte)
CONVERT_INTEGER(short, int, _sat, _rtz)
CONVERT_INTEGER(short, int, _sat, _rtp)
CONVERT_INTEGER(short, int, _sat, _rtn)
CONVERT_FLOAT_TO_INTEGER(short, /* no saturation*/, /* no rounding */)
CONVERT_FLOAT_TO_INTEGER(short, /* no saturation*/, _rte)
CONVERT_FLOAT_TO_INTEGER(short, /* no saturation*/, _rtz)
CONVERT_FLOAT_TO_INTEGER(short, /* no saturation*/, _rtp)
CONVERT_FLOAT_TO_INTEGER(short, /* no saturation*/, _rtn)
CONVERT_FLOAT_TO_INTEGER(short, _sat, /* no rounding */)
CONVERT_FLOAT_TO_INTEGER(short, _sat, _rte)
CONVERT_FLOAT_TO_INTEGER(short, _sat, _rtz)
CONVERT_FLOAT_TO_INTEGER(short, _sat, _rtp)
CONVERT_FLOAT_TO_INTEGER(short, _sat, _rtn)

/*
 * To uint
 */
CONVERT_INTEGER(uint, uchar, /* no saturation*/, /* no rounding */)
CONVERT_INTEGER(uint, uchar, /* no saturation*/, _rte)
CONVERT_INTEGER(uint, uchar, /* no saturation*/, _rtz)
CONVERT_INTEGER(uint, uchar, /* no saturation*/, _rtp)
CONVERT_INTEGER(uint, uchar, /* no saturation*/, _rtn)
CONVERT_INTEGER(uint, uchar, _sat, /* no rounding */)
CONVERT_INTEGER(uint, uchar, _sat, _rte)
CONVERT_INTEGER(uint, uchar, _sat, _rtz)
CONVERT_INTEGER(uint, uchar, _sat, _rtp)
CONVERT_INTEGER(uint, uchar, _sat, _rtn)
CONVERT_INTEGER(uint, char, /* no saturation*/, /* no rounding */)
CONVERT_INTEGER(uint, char, /* no saturation*/, _rte)
CONVERT_INTEGER(uint, char, /* no saturation*/, _rtz)
CONVERT_INTEGER(uint, char, /* no saturation*/, _rtp)
CONVERT_INTEGER(uint, char, /* no saturation*/, _rtn)
CONVERT_INTEGER(uint, char, _sat, /* no rounding */)
CONVERT_INTEGER(uint, char, _sat, _rte)
CONVERT_INTEGER(uint, char, _sat, _rtz)
CONVERT_INTEGER(uint, char, _sat, _rtp)
CONVERT_INTEGER(uint, char, _sat, _rtn)
CONVERT_INTEGER(uint, ushort, /* no saturation*/, /* no rounding */)
CONVERT_INTEGER(uint, ushort, /* no saturation*/, _rte)
CONVERT_INTEGER(uint, ushort, /* no saturation*/, _rtz)
CONVERT_INTEGER(uint, ushort, /* no saturation*/, _rtp)
CONVERT_INTEGER(uint, ushort, /* no saturation*/, _rtn)
CONVERT_INTEGER(uint, ushort, _sat, /* no rounding */)
CONVERT_INTEGER(uint, ushort, _sat, _rte)
CONVERT_INTEGER(uint, ushort, _sat, _rtz)
CONVERT_INTEGER(uint, ushort, _sat, _rtp)
CONVERT_INTEGER(uint, ushort, _sat, _rtn)
CONVERT_INTEGER(uint, short, /* no saturation*/, /* no rounding */)
CONVERT_INTEGER(uint, short, /* no saturation*/, _rte)
CONVERT_INTEGER(uint, short, /* no saturation*/, _rtz)
CONVERT_INTEGER(uint, short, /* no saturation*/, _rtp)
CONVERT_INTEGER(uint, short, /* no saturation*/, _rtn)
CONVERT_INTEGER(uint, short, _sat, /* no rounding */)
CONVERT_INTEGER(uint, short, _sat, _rte)
CONVERT_INTEGER(uint, short, _sat, _rtz)
CONVERT_INTEGER(uint, short, _sat, _rtp)
CONVERT_INTEGER(uint, short, _sat, _rtn)
CONVERT_INTEGER(uint, uint, /* no saturation*/, /* no rounding */)
CONVERT_INTEGER(uint, uint, /* no saturation*/, _rte)
CONVERT_INTEGER(uint, uint, /* no saturation*/, _rtz)
CONVERT_INTEGER(uint, uint, /* no saturation*/, _rtp)
CONVERT_INTEGER(uint, uint, /* no saturation*/, _rtn)
CONVERT_INTEGER(uint, uint, _sat, /* no rounding */)
CONVERT_INTEGER(uint, uint, _sat, _rte)
CONVERT_INTEGER(uint, uint, _sat, _rtz)
CONVERT_INTEGER(uint, uint, _sat, _rtp)
CONVERT_INTEGER(uint, uint, _sat, _rtn)
CONVERT_INTEGER(uint, int, /* no saturation*/, /* no rounding */)
CONVERT_INTEGER(uint, int, /* no saturation*/, _rte)
CONVERT_INTEGER(uint, int, /* no saturation*/, _rtz)
CONVERT_INTEGER(uint, int, /* no saturation*/, _rtp)
CONVERT_INTEGER(uint, int, /* no saturation*/, _rtn)
CONVERT_INTEGER(uint, int, _sat, /* no rounding */)
CONVERT_INTEGER(uint, int, _sat, _rte)
CONVERT_INTEGER(uint, int, _sat, _rtz)
CONVERT_INTEGER(uint, int, _sat, _rtp)
CONVERT_INTEGER(uint, int, _sat, _rtn)
CONVERT_FLOAT_TO_UINT(/* no saturation*/, /* no rounding */)
CONVERT_FLOAT_TO_UINT(/* no saturation*/, _rte)
CONVERT_FLOAT_TO_UINT(/* no saturation*/, _rtz)
CONVERT_FLOAT_TO_UINT(/* no saturation*/, _rtp)
CONVERT_FLOAT_TO_UINT(/* no saturation*/, _rtn)
CONVERT_FLOAT_TO_UINT(_sat, /* no rounding */)
CONVERT_FLOAT_TO_UINT(_sat, _rte)
CONVERT_FLOAT_TO_UINT(_sat, _rtz)
CONVERT_FLOAT_TO_UINT(_sat, _rtp)
CONVERT_FLOAT_TO_UINT(_sat, _rtn)

/*
 * To int
 */
CONVERT_INTEGER(int, uchar, /* no saturation*/, /* no rounding */)
CONVERT_INTEGER(int, uchar, /* no saturation*/, _rte)
CONVERT_INTEGER(int, uchar, /* no saturation*/, _rtz)
CONVERT_INTEGER(int, uchar, /* no saturation*/, _rtp)
CONVERT_INTEGER(int, uchar, /* no saturation*/, _rtn)
CONVERT_INTEGER(int, uchar, _sat, /* no rounding */)
CONVERT_INTEGER(int, uchar, _sat, _rte)
CONVERT_INTEGER(int, uchar, _sat, _rtz)
CONVERT_INTEGER(int, uchar, _sat, _rtp)
CONVERT_INTEGER(int, uchar, _sat, _rtn)
CONVERT_INTEGER(int, char, /* no saturation*/, /* no rounding */)
CONVERT_INTEGER(int, char, /* no saturation*/, _rte)
CONVERT_INTEGER(int, char, /* no saturation*/, _rtz)
CONVERT_INTEGER(int, char, /* no saturation*/, _rtp)
CONVERT_INTEGER(int, char, /* no saturation*/, _rtn)
CONVERT_INTEGER(int, char, _sat, /* no rounding */)
CONVERT_INTEGER(int, char, _sat, _rte)
CONVERT_INTEGER(int, char, _sat, _rtz)
CONVERT_INTEGER(int, char, _sat, _rtp)
CONVERT_INTEGER(int, char, _sat, _rtn)
CONVERT_INTEGER(int, ushort, /* no saturation*/, /* no rounding */)
CONVERT_INTEGER(int, ushort, /* no saturation*/, _rte)
CONVERT_INTEGER(int, ushort, /* no saturation*/, _rtz)
CONVERT_INTEGER(int, ushort, /* no saturation*/, _rtp)
CONVERT_INTEGER(int, ushort, /* no saturation*/, _rtn)
CONVERT_INTEGER(int, ushort, _sat, /* no rounding */)
CONVERT_INTEGER(int, ushort, _sat, _rte)
CONVERT_INTEGER(int, ushort, _sat, _rtz)
CONVERT_INTEGER(int, ushort, _sat, _rtp)
CONVERT_INTEGER(int, ushort, _sat, _rtn)
CONVERT_INTEGER(int, short, /* no saturation*/, /* no rounding */)
CONVERT_INTEGER(int, short, /* no saturation*/, _rte)
CONVERT_INTEGER(int, short, /* no saturation*/, _rtz)
CONVERT_INTEGER(int, short, /* no saturation*/, _rtp)
CONVERT_INTEGER(int, short, /* no saturation*/, _rtn)
CONVERT_INTEGER(int, short, _sat, /* no rounding */)
CONVERT_INTEGER(int, short, _sat, _rte)
CONVERT_INTEGER(int, short, _sat, _rtz)
CONVERT_INTEGER(int, short, _sat, _rtp)
CONVERT_INTEGER(int, short, _sat, _rtn)
CONVERT_INTEGER(int, uint, /* no saturation*/, /* no rounding */)
CONVERT_INTEGER(int, uint, /* no saturation*/, _rte)
CONVERT_INTEGER(int, uint, /* no saturation*/, _rtz)
CONVERT_INTEGER(int, uint, /* no saturation*/, _rtp)
CONVERT_INTEGER(int, uint, /* no saturation*/, _rtn)
CONVERT_INTEGER(int, uint, _sat, /* no rounding */)
CONVERT_INTEGER(int, uint, _sat, _rte)
CONVERT_INTEGER(int, uint, _sat, _rtz)
CONVERT_INTEGER(int, uint, _sat, _rtp)
CONVERT_INTEGER(int, uint, _sat, _rtn)
CONVERT_INTEGER(int, int, /* no saturation*/, /* no rounding */)
CONVERT_INTEGER(int, int, /* no saturation*/, _rte)
CONVERT_INTEGER(int, int, /* no saturation*/, _rtz)
CONVERT_INTEGER(int, int, /* no saturation*/, _rtp)
CONVERT_INTEGER(int, int, /* no saturation*/, _rtn)
CONVERT_INTEGER(int, int, _sat, /* no rounding */)
CONVERT_INTEGER(int, int, _sat, _rte)
CONVERT_INTEGER(int, int, _sat, _rtz)
CONVERT_INTEGER(int, int, _sat, _rtp)
CONVERT_INTEGER(int, int, _sat, _rtn)
CONVERT_FLOAT_TO_INTEGER(int, /* no saturation*/, /* no rounding */)
CONVERT_FLOAT_TO_INTEGER(int, /* no saturation*/, _rte)
CONVERT_FLOAT_TO_INTEGER(int, /* no saturation*/, _rtz)
CONVERT_FLOAT_TO_INTEGER(int, /* no saturation*/, _rtp)
CONVERT_FLOAT_TO_INTEGER(int, /* no saturation*/, _rtn)
CONVERT_FLOAT_TO_INTEGER(int, _sat, /* no rounding */)
CONVERT_FLOAT_TO_INTEGER(int, _sat, _rte)
CONVERT_FLOAT_TO_INTEGER(int, _sat, _rtz)
CONVERT_FLOAT_TO_INTEGER(int, _sat, _rtp)
CONVERT_FLOAT_TO_INTEGER(int, _sat, _rtn)

/*
 * To float
 */
CONVERT_INTEGER_TO_FLOAT(uchar, /* no saturation*/, /* no rounding */)
CONVERT_INTEGER_TO_FLOAT(uchar, /* no saturation*/, _rte)
CONVERT_INTEGER_TO_FLOAT(uchar, /* no saturation*/, _rtz)
CONVERT_INTEGER_TO_FLOAT(uchar, /* no saturation*/, _rtp)
CONVERT_INTEGER_TO_FLOAT(uchar, /* no saturation*/, _rtn)
CONVERT_INTEGER_TO_FLOAT(uchar, _sat, /* no rounding */)
CONVERT_INTEGER_TO_FLOAT(uchar, _sat, _rte)
CONVERT_INTEGER_TO_FLOAT(uchar, _sat, _rtz)
CONVERT_INTEGER_TO_FLOAT(uchar, _sat, _rtp)
CONVERT_INTEGER_TO_FLOAT(uchar, _sat, _rtn)
CONVERT_INTEGER_TO_FLOAT(char, /* no saturation*/, /* no rounding */)
CONVERT_INTEGER_TO_FLOAT(char, /* no saturation*/, _rte)
CONVERT_INTEGER_TO_FLOAT(char, /* no saturation*/, _rtz)
CONVERT_INTEGER_TO_FLOAT(char, /* no saturation*/, _rtp)
CONVERT_INTEGER_TO_FLOAT(char, /* no saturation*/, _rtn)
CONVERT_INTEGER_TO_FLOAT(char, _sat, /* no rounding */)
CONVERT_INTEGER_TO_FLOAT(char, _sat, _rte)
CONVERT_INTEGER_TO_FLOAT(char, _sat, _rtz)
CONVERT_INTEGER_TO_FLOAT(char, _sat, _rtp)
CONVERT_INTEGER_TO_FLOAT(char, _sat, _rtn)
CONVERT_INTEGER_TO_FLOAT(ushort, /* no saturation*/, /* no rounding */)
CONVERT_INTEGER_TO_FLOAT(ushort, /* no saturation*/, _rte)
CONVERT_INTEGER_TO_FLOAT(ushort, /* no saturation*/, _rtz)
CONVERT_INTEGER_TO_FLOAT(ushort, /* no saturation*/, _rtp)
CONVERT_INTEGER_TO_FLOAT(ushort, /* no saturation*/, _rtn)
CONVERT_INTEGER_TO_FLOAT(ushort, _sat, /* no rounding */)
CONVERT_INTEGER_TO_FLOAT(ushort, _sat, _rte)
CONVERT_INTEGER_TO_FLOAT(ushort, _sat, _rtz)
CONVERT_INTEGER_TO_FLOAT(ushort, _sat, _rtp)
CONVERT_INTEGER_TO_FLOAT(ushort, _sat, _rtn)
CONVERT_INTEGER_TO_FLOAT(short, /* no saturation*/, /* no rounding */)
CONVERT_INTEGER_TO_FLOAT(short, /* no saturation*/, _rte)
CONVERT_INTEGER_TO_FLOAT(short, /* no saturation*/, _rtz)
CONVERT_INTEGER_TO_FLOAT(short, /* no saturation*/, _rtp)
CONVERT_INTEGER_TO_FLOAT(short, /* no saturation*/, _rtn)
CONVERT_INTEGER_TO_FLOAT(short, _sat, /* no rounding */)
CONVERT_INTEGER_TO_FLOAT(short, _sat, _rte)
CONVERT_INTEGER_TO_FLOAT(short, _sat, _rtz)
CONVERT_INTEGER_TO_FLOAT(short, _sat, _rtp)
CONVERT_INTEGER_TO_FLOAT(short, _sat, _rtn)
CONVERT_UINT_TO_FLOAT(/* no saturation*/, /* no rounding */)
CONVERT_UINT_TO_FLOAT(/* no saturation*/, _rte)
CONVERT_UINT_TO_FLOAT(/* no saturation*/, _rtz)
CONVERT_UINT_TO_FLOAT(/* no saturation*/, _rtp)
CONVERT_UINT_TO_FLOAT(/* no saturation*/, _rtn)
CONVERT_UINT_TO_FLOAT(_sat, /* no rounding */)
CONVERT_UINT_TO_FLOAT(_sat, _rte)
CONVERT_UINT_TO_FLOAT(_sat, _rtz)
CONVERT_UINT_TO_FLOAT(_sat, _rtp)
CONVERT_UINT_TO_FLOAT(_sat, _rtn)
CONVERT_INTEGER_TO_FLOAT(int, /* no saturation*/, /* no rounding */)
CONVERT_INTEGER_TO_FLOAT(int, /* no saturation*/, _rte)
CONVERT_INTEGER_TO_FLOAT(int, /* no saturation*/, _rtz)
CONVERT_INTEGER_TO_FLOAT(int, /* no saturation*/, _rtp)
CONVERT_INTEGER_TO_FLOAT(int, /* no saturation*/, _rtn)
CONVERT_INTEGER_TO_FLOAT(int, _sat, /* no rounding */)
CONVERT_INTEGER_TO_FLOAT(int, _sat, _rte)
CONVERT_INTEGER_TO_FLOAT(int, _sat, _rtz)
CONVERT_INTEGER_TO_FLOAT(int, _sat, _rtp)
CONVERT_INTEGER_TO_FLOAT(int, _sat, _rtn)
CONVERT_FLOAT_TO_FLOAT(/* no saturation*/, /* no rounding */)
CONVERT_FLOAT_TO_FLOAT(/* no saturation*/, _rte)
CONVERT_FLOAT_TO_FLOAT(/* no saturation*/, _rtz)
CONVERT_FLOAT_TO_FLOAT(/* no saturation*/, _rtp)
CONVERT_FLOAT_TO_FLOAT(/* no saturation*/, _rtn)
CONVERT_FLOAT_TO_FLOAT(_sat, /* no rounding */)
CONVERT_FLOAT_TO_FLOAT(_sat, _rte)
CONVERT_FLOAT_TO_FLOAT(_sat, _rtz)
CONVERT_FLOAT_TO_FLOAT(_sat, _rtp)
CONVERT_FLOAT_TO_FLOAT(_sat, _rtn)

/*
 * A newer version of the opencl-c.h header defines all as_<type> as macros to __builtin_astype wich generates the same LLVM IR as this custom implementation
 */
/*
#define AS_TYPE(dstType, srcType) \
    INLINE dstType as_##dstType(srcType val) OVERLOADABLE CONST \
    { \
        return ((union { srcType src; dstType dst; }) { .src = val}).dst; \
    }

AS_TYPE(uchar, uchar)
AS_TYPE(uchar, char)
AS_TYPE(uchar2, uchar2)
AS_TYPE(uchar2, char2)
AS_TYPE(uchar2, ushort)
AS_TYPE(uchar2, short)
AS_TYPE(uchar3, uchar3)
AS_TYPE(uchar3, char3)
AS_TYPE(uchar3, uchar4)
AS_TYPE(uchar3, char4)
AS_TYPE(uchar3, ushort2)
AS_TYPE(uchar3, short2)
AS_TYPE(uchar3, uint)
AS_TYPE(uchar3, int)
AS_TYPE(uchar3, float)
AS_TYPE(uchar4, uchar3)
AS_TYPE(uchar4, char3)
AS_TYPE(uchar4, uchar4)
AS_TYPE(uchar4, char4)
AS_TYPE(uchar4, ushort2)
AS_TYPE(uchar4, short2)
AS_TYPE(uchar4, uint)
AS_TYPE(uchar4, int)
AS_TYPE(uchar4, float)
AS_TYPE(uchar8, uchar8)
AS_TYPE(uchar8, char8)
AS_TYPE(uchar8, ushort3)
AS_TYPE(uchar8, short3)
AS_TYPE(uchar8, ushort4)
AS_TYPE(uchar8, short4)
AS_TYPE(uchar8, uint2)
AS_TYPE(uchar8, int2)
AS_TYPE(uchar8, float2)
AS_TYPE(uchar16, uchar16)
AS_TYPE(uchar16, char16)
AS_TYPE(uchar16, ushort8)
AS_TYPE(uchar16, short8)
AS_TYPE(uchar16, uint3)
AS_TYPE(uchar16, int3)
AS_TYPE(uchar16, float3)
AS_TYPE(uchar16, uint4)
AS_TYPE(uchar16, int4)
AS_TYPE(uchar16, float4)

AS_TYPE(char, uchar)
AS_TYPE(char, char)
AS_TYPE(char2, uchar2)
AS_TYPE(char2, char2)
AS_TYPE(char2, ushort)
AS_TYPE(char2, short)
AS_TYPE(char3, uchar3)
AS_TYPE(char3, char3)
AS_TYPE(char3, uchar4)
AS_TYPE(char3, char4)
AS_TYPE(char3, ushort2)
AS_TYPE(char3, short2)
AS_TYPE(char3, uint)
AS_TYPE(char3, int)
AS_TYPE(char3, float)
AS_TYPE(char4, uchar3)
AS_TYPE(char4, char3)
AS_TYPE(char4, uchar4)
AS_TYPE(char4, char4)
AS_TYPE(char4, ushort2)
AS_TYPE(char4, short2)
AS_TYPE(char4, uint)
AS_TYPE(char4, int)
AS_TYPE(char4, float)
AS_TYPE(char8, uchar8)
AS_TYPE(char8, char8)
AS_TYPE(char8, ushort3)
AS_TYPE(char8, short3)
AS_TYPE(char8, ushort4)
AS_TYPE(char8, short4)
AS_TYPE(char8, uint2)
AS_TYPE(char8, int2)
AS_TYPE(char8, float2)
AS_TYPE(char16, uchar16)
AS_TYPE(char16, char16)
AS_TYPE(char16, ushort8)
AS_TYPE(char16, short8)
AS_TYPE(char16, uint3)
AS_TYPE(char16, int3)
AS_TYPE(char16, float3)
AS_TYPE(char16, uint4)
AS_TYPE(char16, int4)
AS_TYPE(char16, float4)

AS_TYPE(ushort, uchar2)
AS_TYPE(ushort, char2)
AS_TYPE(ushort, ushort)
AS_TYPE(ushort, short)
AS_TYPE(ushort2, uchar3)
AS_TYPE(ushort2, char3)
AS_TYPE(ushort2, uchar4)
AS_TYPE(ushort2, char4)
AS_TYPE(ushort2, ushort2)
AS_TYPE(ushort2, short2)
AS_TYPE(ushort2, uint)
AS_TYPE(ushort2, int)
AS_TYPE(ushort2, float)
AS_TYPE(ushort3, uchar8)
AS_TYPE(ushort3, char8)
AS_TYPE(ushort3, ushort3)
AS_TYPE(ushort3, short3)
AS_TYPE(ushort3, ushort4)
AS_TYPE(ushort3, short4)
AS_TYPE(ushort3, uint2)
AS_TYPE(ushort3, int2)
AS_TYPE(ushort3, float2)
AS_TYPE(ushort4, uchar8)
AS_TYPE(ushort4, char8)
AS_TYPE(ushort4, ushort3)
AS_TYPE(ushort4, short3)
AS_TYPE(ushort4, ushort4)
AS_TYPE(ushort4, short4)
AS_TYPE(ushort4, uint2)
AS_TYPE(ushort4, int2)
AS_TYPE(ushort4, float2)
AS_TYPE(ushort8, uchar16)
AS_TYPE(ushort8, char16)
AS_TYPE(ushort8, ushort8)
AS_TYPE(ushort8, short8)
AS_TYPE(ushort8, uint3)
AS_TYPE(ushort8, int3)
AS_TYPE(ushort8, float3)
AS_TYPE(ushort8, uint4)
AS_TYPE(ushort8, int4)
AS_TYPE(ushort8, float4)
AS_TYPE(ushort16, ushort16)
AS_TYPE(ushort16, short16)
AS_TYPE(ushort16, uint8)
AS_TYPE(ushort16, int8)
AS_TYPE(ushort16, float8)

AS_TYPE(short, uchar2)
AS_TYPE(short, char2)
AS_TYPE(short, ushort)
AS_TYPE(short, short)
AS_TYPE(short2, uchar3)
AS_TYPE(short2, char3)
AS_TYPE(short2, uchar4)
AS_TYPE(short2, char4)
AS_TYPE(short2, ushort2)
AS_TYPE(short2, short2)
AS_TYPE(short2, uint)
AS_TYPE(short2, int)
AS_TYPE(short2, float)
AS_TYPE(short3, uchar8)
AS_TYPE(short3, char8)
AS_TYPE(short3, ushort3)
AS_TYPE(short3, short3)
AS_TYPE(short3, ushort4)
AS_TYPE(short3, short4)
AS_TYPE(short3, uint2)
AS_TYPE(short3, int2)
AS_TYPE(short3, float2)
AS_TYPE(short4, uchar8)
AS_TYPE(short4, char8)
AS_TYPE(short4, ushort3)
AS_TYPE(short4, short3)
AS_TYPE(short4, ushort4)
AS_TYPE(short4, short4)
AS_TYPE(short4, uint2)
AS_TYPE(short4, int2)
AS_TYPE(short4, float2)
AS_TYPE(short8, uchar16)
AS_TYPE(short8, char16)
AS_TYPE(short8, ushort8)
AS_TYPE(short8, short8)
AS_TYPE(short8, uint3)
AS_TYPE(short8, int3)
AS_TYPE(short8, float3)
AS_TYPE(short8, uint4)
AS_TYPE(short8, int4)
AS_TYPE(short8, float4)
AS_TYPE(short16, ushort16)
AS_TYPE(short16, short16)
AS_TYPE(short16, uint8)
AS_TYPE(short16, int8)
AS_TYPE(short16, float8)

AS_TYPE(uint, uchar3)
AS_TYPE(uint, char3)
AS_TYPE(uint, uchar4)
AS_TYPE(uint, char4)
AS_TYPE(uint, ushort2)
AS_TYPE(uint, short2)
AS_TYPE(uint, uint)
AS_TYPE(uint, int)
AS_TYPE(uint, float)
AS_TYPE(uint2, uchar8)
AS_TYPE(uint2, char8)
AS_TYPE(uint2, ushort3)
AS_TYPE(uint2, short3)
AS_TYPE(uint2, ushort4)
AS_TYPE(uint2, short4)
AS_TYPE(uint2, uint2)
AS_TYPE(uint2, int2)
AS_TYPE(uint2, float2)
AS_TYPE(uint3, uchar16)
AS_TYPE(uint3, char16)
AS_TYPE(uint3, ushort8)
AS_TYPE(uint3, short8)
AS_TYPE(uint3, uint3)
AS_TYPE(uint3, int3)
AS_TYPE(uint3, float3)
AS_TYPE(uint3, uint4)
AS_TYPE(uint3, int4)
AS_TYPE(uint3, float4)
AS_TYPE(uint4, uchar16)
AS_TYPE(uint4, char16)
AS_TYPE(uint4, ushort8)
AS_TYPE(uint4, short8)
AS_TYPE(uint4, uint3)
AS_TYPE(uint4, int3)
AS_TYPE(uint4, float3)
AS_TYPE(uint4, uint4)
AS_TYPE(uint4, int4)
AS_TYPE(uint4, float4)
AS_TYPE(uint8, ushort16)
AS_TYPE(uint8, short16)
AS_TYPE(uint8, uint8)
AS_TYPE(uint8, int8)
AS_TYPE(uint8, float8)
AS_TYPE(uint16, uint16)
AS_TYPE(uint16, int16)
AS_TYPE(uint16, float16)

AS_TYPE(int, uchar3)
AS_TYPE(int, char3)
AS_TYPE(int, uchar4)
AS_TYPE(int, char4)
AS_TYPE(int, ushort2)
AS_TYPE(int, short2)
AS_TYPE(int, uint)
AS_TYPE(int, int)
AS_TYPE(int, float)
AS_TYPE(int2, uchar8)
AS_TYPE(int2, char8)
AS_TYPE(int2, ushort3)
AS_TYPE(int2, short3)
AS_TYPE(int2, ushort4)
AS_TYPE(int2, short4)
AS_TYPE(int2, uint2)
AS_TYPE(int2, int2)
AS_TYPE(int2, float2)
AS_TYPE(int3, uchar16)
AS_TYPE(int3, char16)
AS_TYPE(int3, ushort8)
AS_TYPE(int3, short8)
AS_TYPE(int3, uint3)
AS_TYPE(int3, int3)
AS_TYPE(int3, float3)
AS_TYPE(int3, uint4)
AS_TYPE(int3, int4)
AS_TYPE(int3, float4)
AS_TYPE(int4, uchar16)
AS_TYPE(int4, char16)
AS_TYPE(int4, ushort8)
AS_TYPE(int4, short8)
AS_TYPE(int4, uint3)
AS_TYPE(int4, int3)
AS_TYPE(int4, float3)
AS_TYPE(int4, uint4)
AS_TYPE(int4, int4)
AS_TYPE(int4, float4)
AS_TYPE(int8, ushort16)
AS_TYPE(int8, short16)
AS_TYPE(int8, uint8)
AS_TYPE(int8, int8)
AS_TYPE(int8, float8)
AS_TYPE(int16, uint16)
AS_TYPE(int16, int16)
AS_TYPE(int16, float16)

AS_TYPE(float, uchar3)
AS_TYPE(float, char3)
AS_TYPE(float, uchar4)
AS_TYPE(float, char4)
AS_TYPE(float, ushort2)
AS_TYPE(float, short2)
AS_TYPE(float, uint)
AS_TYPE(float, int)
AS_TYPE(float, float)
AS_TYPE(float2, uchar8)
AS_TYPE(float2, char8)
AS_TYPE(float2, ushort3)
AS_TYPE(float2, short3)
AS_TYPE(float2, ushort4)
AS_TYPE(float2, short4)
AS_TYPE(float2, uint2)
AS_TYPE(float2, int2)
AS_TYPE(float2, float2)
AS_TYPE(float3, uchar16)
AS_TYPE(float3, char16)
AS_TYPE(float3, ushort8)
AS_TYPE(float3, short8)
AS_TYPE(float3, uint3)
AS_TYPE(float3, int3)
AS_TYPE(float3, float3)
AS_TYPE(float3, uint4)
AS_TYPE(float3, int4)
AS_TYPE(float3, float4)
AS_TYPE(float4, uchar16)
AS_TYPE(float4, char16)
AS_TYPE(float4, ushort8)
AS_TYPE(float4, short8)
AS_TYPE(float4, uint3)
AS_TYPE(float4, int3)
AS_TYPE(float4, float3)
AS_TYPE(float4, uint4)
AS_TYPE(float4, int4)
AS_TYPE(float4, float4)
AS_TYPE(float8, ushort16)
AS_TYPE(float8, short16)
AS_TYPE(float8, uint8)
AS_TYPE(float8, int8)
AS_TYPE(float8, float8)
AS_TYPE(float16, uint16)
AS_TYPE(float16, int16)
AS_TYPE(float16, float16)
*/

#undef uchar_MIN
#undef uchar_MAX
#undef char_MIN
#undef char_MAX
#undef ushort_MIN
#undef ushort_MAX
#undef short_MIN
#undef short_MAX
#undef uint_MIN
#undef uint_MAX
#undef int_MIN
#undef int_MAX
#undef float_MAX
#undef float_MIN

#undef _sat
#undef _rte
#undef _rtz
#undef _rtp
#undef _rtn
#undef SATURATE_INTEGER
#undef SATURATE_FLOAT
#undef ROUND_TO_INTEGER
#undef CONVERSION_WITH_SATURATION
#undef CONVERSION_WITH_SATURATION_FLOAT
#undef CONVERT_INTEGER
#undef CONVERT_FLOAT_TO_INTEGER
#undef CONVERT_INTEGER_TO_FLOAT
#undef CONVERT_FLOAT_TO_FLOAT
#undef CONVERT_UINT_TO_FLOAT
#undef CONVERT_FLOAT_TO_UINT

#undef CONCAT_
#undef CC

#endif /* VC4CL_CONVERSIONS_H */

