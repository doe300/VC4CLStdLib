/*
 * Author: doe300
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef VC4CL_VECTOR_H
#define VC4CL_VECTOR_H

#include "_config.h"
#include "_overloads.h"

#define VECTOR_LOAD(type) \
	INLINE type##2 vload2(size_t offset, const __global type * ptr) OVERLOADABLE \
	{ \
		return *((const __global type##2 *)(ptr + offset * 2)); \
	} \
	INLINE type##3 vload3(size_t offset, const __global type * ptr) OVERLOADABLE \
	{ \
		return *((const __global type##3 *)(ptr + offset * 3)); \
	} \
	INLINE type##4 vload4(size_t offset, const __global type * ptr) OVERLOADABLE \
	{ \
		return *((const __global type##4 *)(ptr + offset * 4)); \
	} \
	INLINE type##8 vload8(size_t offset, const __global type * ptr) OVERLOADABLE \
	{ \
		return *((const __global type##8 *)(ptr + offset * 8)); \
	} \
	INLINE type##16 vload16(size_t offset, const __global type * ptr) OVERLOADABLE \
	{ \
		return *((const __global type##16 *)(ptr + offset * 16)); \
	} \
	INLINE type##2 vload2(size_t offset, const __local type * ptr) OVERLOADABLE \
	{ \
		return *((const __local type##2 *)(ptr + offset * 2)); \
	} \
	INLINE type##3 vload3(size_t offset, const __local type * ptr) OVERLOADABLE \
	{ \
		return *((const __local type##3 *)(ptr + offset * 3)); \
	} \
	INLINE type##4 vload4(size_t offset, const __local type * ptr) OVERLOADABLE \
	{ \
		return *((const __local type##4 *)(ptr + offset * 4)); \
	} \
	INLINE type##8 vload8(size_t offset, const __local type * ptr) OVERLOADABLE \
	{ \
		return *((const __local type##8 *)(ptr + offset * 8)); \
	} \
	INLINE type##16 vload16(size_t offset, const __local type * ptr) OVERLOADABLE \
	{ \
		return *((const __local type##16 *)(ptr + offset * 16)); \
	} \
	INLINE type##2 vload2(size_t offset, const __constant type * ptr) OVERLOADABLE \
	{ \
		return *((const __constant type##2 *)(ptr + offset * 2)); \
	} \
	INLINE type##3 vload3(size_t offset, const __constant type * ptr) OVERLOADABLE \
	{ \
		return *((const __constant type##3 *)(ptr + offset * 3)); \
	} \
	INLINE type##4 vload4(size_t offset, const __constant type * ptr) OVERLOADABLE \
	{ \
		return *((const __constant type##4 *)(ptr + offset * 4)); \
	} \
	INLINE type##8 vload8(size_t offset, const __constant type * ptr) OVERLOADABLE \
	{ \
		return *((const __constant type##8 *)(ptr + offset * 8)); \
	} \
	INLINE type##16 vload16(size_t offset, const __constant type * ptr) OVERLOADABLE \
	{ \
		return *((const __constant type##16 *)(ptr + offset * 16)); \
	} \
	INLINE type##2 vload2(size_t offset, const __private type * ptr) OVERLOADABLE \
	{ \
		return *((const __private type##2 *)(ptr + offset * 2)); \
	} \
	INLINE type##3 vload3(size_t offset, const __private type * ptr) OVERLOADABLE \
	{ \
		return *((const __private type##3 *)(ptr + offset * 3)); \
	} \
	INLINE type##4 vload4(size_t offset, const __private type * ptr) OVERLOADABLE \
	{ \
		return *((const __private type##4 *)(ptr + offset * 4)); \
	} \
	INLINE type##8 vload8(size_t offset, const __private type * ptr) OVERLOADABLE \
	{ \
		return *((const __private type##8 *)(ptr + offset * 8)); \
	} \
	INLINE type##16 vload16(size_t offset, const __private type * ptr) OVERLOADABLE \
	{ \
		return *((const __private type##16 *)(ptr + offset * 16)); \
	}

#define VECTOR_STORE(type) \
	INLINE void vstore2(type##2 data, size_t offset, __global type * ptr) OVERLOADABLE \
	{ \
		*((__global type##2 *)(ptr + offset * 2)) = data; \
	} \
	INLINE void vstore3(type##3 data, size_t offset, __global type * ptr) OVERLOADABLE \
	{ \
		*((__global type##3 *)(ptr + offset * 3)) = data; \
	} \
	INLINE void vstore4(type##4 data, size_t offset, __global type * ptr) OVERLOADABLE \
	{ \
		*((__global type##4 *)(ptr + offset * 4)) = data; \
	} \
	INLINE void vstore8(type##8 data, size_t offset, __global type * ptr) OVERLOADABLE \
	{ \
		*((__global type##8 *)(ptr + offset * 8)) = data; \
	} \
	INLINE void vstore16(type##16 data, size_t offset, __global type * ptr) OVERLOADABLE \
	{ \
		*((__global type##16 *)(ptr + offset * 16)) = data; \
	} \
	INLINE void vstore2(type##2 data, size_t offset, __local type * ptr) OVERLOADABLE \
	{ \
		*((__local type##2 *)(ptr + offset * 2)) = data; \
	} \
	INLINE void vstore3(type##3 data, size_t offset, __local type * ptr) OVERLOADABLE \
	{ \
		*((__local type##3 *)(ptr + offset * 3)) = data; \
	} \
	INLINE void vstore4(type##4 data, size_t offset, __local type * ptr) OVERLOADABLE \
	{ \
		*((__local type##4 *)(ptr + offset * 4)) = data; \
	} \
	INLINE void vstore8(type##8 data, size_t offset, __local type * ptr) OVERLOADABLE \
	{ \
		*((__local type##8 *)(ptr + offset * 8)) = data; \
	} \
	INLINE void vstore16(type##16 data, size_t offset, __local type * ptr) OVERLOADABLE \
	{ \
		*((__local type##16 *)(ptr + offset * 16)) = data; \
	} \
	INLINE void vstore2(type##2 data, size_t offset, __private type * ptr) OVERLOADABLE \
	{ \
		*((__private type##2 *)(ptr + offset * 2)) = data; \
	} \
	INLINE void vstore3(type##3 data, size_t offset, __private type * ptr) OVERLOADABLE \
	{ \
		*((__private type##3 *)(ptr + offset * 3)) = data; \
	} \
	INLINE void vstore4(type##4 data, size_t offset, __private type * ptr) OVERLOADABLE \
	{ \
		*((__private type##4 *)(ptr + offset * 4)) = data; \
	} \
	INLINE void vstore8(type##8 data, size_t offset, __private type * ptr) OVERLOADABLE \
	{ \
		*((__private type##8 *)(ptr + offset * 8)) = data; \
	} \
	INLINE void vstore16(type##16 data, size_t offset, __private type * ptr) OVERLOADABLE \
	{ \
		*((__private type##16 *)(ptr + offset * 16)) = data; \
	}

#define VECTOR_SHUFFLE_2_INTERNAL(type, maskType, num) \
	INLINE type##2 shuffle2(type##num x, type##num y, maskType##2 mask) OVERLOADABLE \
	{ \
		return __builtin_shufflevector(x, y, mask.x, mask.y); \
	} \
	INLINE type##4 shuffle2(type##num x, type##num y, maskType##4 mask) OVERLOADABLE \
	{ \
		return __builtin_shufflevector(x, y, mask.x, mask.y, mask.z, mask.w); \
	} \
	INLINE type##8 shuffle2(type##num x, type##num y, maskType##8 mask) OVERLOADABLE \
	{ \
		return __builtin_shufflevector(x, y, mask.s0, mask.s1, mask.s2, mask.s3, mask.s4, mask.s5, mask.s6, mask.s7); \
	} \
	INLINE type##16 shuffle2(type##num x, type##num y, maskType##16 mask) OVERLOADABLE \
	{ \
		return __builtin_shufflevector(x, y, mask.s0, mask.s1, mask.s2, mask.s3, mask.s4, mask.s5, mask.s6, mask.s7, mask.s8, mask.s9, mask.sa, mask.sb, mask.sc, mask.sd, mask.se, mask.sf); \
	} \

#define VECTOR_SHUFFLE_2(type, maskType) \
	VECTOR_SHUFFLE_2_INTERNAL(type, maskType, 2) \
	VECTOR_SHUFFLE_2_INTERNAL(type, maskType, 4) \
	VECTOR_SHUFFLE_2_INTERNAL(type, maskType, 8) \
	VECTOR_SHUFFLE_2_INTERNAL(type, maskType, 16)

#define VECTOR_SHUFFLE_INTERNAL(type, maskType, num) \
	INLINE type##2 shuffle(type##num val, maskType##2 mask) OVERLOADABLE \
	{ \
		return shuffle2(val, val, mask); \
	} \
	INLINE type##4 shuffle(type##num val, maskType##4 mask) OVERLOADABLE \
	{ \
		return shuffle2(val, val, mask); \
	} \
	INLINE type##8 shuffle(type##num val, maskType##8 mask) OVERLOADABLE \
	{ \
		return shuffle2(val, val, mask); \
	} \
	INLINE type##16 shuffle(type##num val, maskType##16 mask) OVERLOADABLE \
	{ \
		return shuffle2(val, val, mask); \
	} \

#define VECTOR_SHUFFLE(type, maskType) \
	VECTOR_SHUFFLE_INTERNAL(type, maskType, 2) \
	VECTOR_SHUFFLE_INTERNAL(type, maskType, 4) \
	VECTOR_SHUFFLE_INTERNAL(type, maskType, 8) \
	VECTOR_SHUFFLE_INTERNAL(type, maskType, 16)

VECTOR_LOAD(uchar)
VECTOR_LOAD(char)
VECTOR_LOAD(ushort)
VECTOR_LOAD(short)
VECTOR_LOAD(uint)
VECTOR_LOAD(int)
VECTOR_LOAD(float)

// Add some compatilibity symbol.
// See https://github.com/doe300/VC4CL/issues/31#issuecomment-393132343
float4 _Z6vload4jPKU3AS1f(size_t offset, const __global float * ptr)
{
	return vload4(offset, ptr);
}

VECTOR_STORE(uchar)
VECTOR_STORE(char)
VECTOR_STORE(ushort)
VECTOR_STORE(short)
VECTOR_STORE(uint)
VECTOR_STORE(int)
VECTOR_STORE(float)

//TODO vload(a)_half, vload(a)_halfn (+rounding) (load half and return converted to float, possible with unpack-modes)
//TODO vstore(a)_half, vstore(a)_halfn (+rounding) (store float as half in memory, possible with pack modes)

/*
 * TODO shuffle2, but LLVM fails, since the indices for the __builtin intrinsic need to be constant integers!
VECTOR_SHUFFLE_2(uchar, uchar)
VECTOR_SHUFFLE_2(char, uchar)
VECTOR_SHUFFLE_2(ushort, ushort)
VECTOR_SHUFFLE_2(short, ushort)
VECTOR_SHUFFLE_2(uint, uint)
VECTOR_SHUFFLE_2(int, uint)
VECTOR_SHUFFLE_2(float, uint)
*/

VECTOR_SHUFFLE(uchar, uchar)
VECTOR_SHUFFLE(char, uchar)
VECTOR_SHUFFLE(ushort, ushort)
VECTOR_SHUFFLE(short, ushort)
VECTOR_SHUFFLE(uint, uint)
VECTOR_SHUFFLE(int, uint)
VECTOR_SHUFFLE(float, uint)

//shuffle2 is handled via intrinsifying the OpenCL function

#endif /* VC4CL_VECTOR_H */

