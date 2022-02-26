/*
 * Author: doe300
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef VC4CL_OVERLOADS_H
#define VC4CL_OVERLOADS_H

#include "_config.h"

#ifndef OVERLOADABLE
#define OVERLOADABLE  __attribute__((overloadable))
#endif
/*
 * "__attribute__((const)) function attribute
 *  Many functions examine only the arguments passed to them, and have no effects except for the return value.
 *  This is a much stricter class than __attribute__((pure)), because a function is not permitted to read global memory.
 *  If a function is known to operate only on its arguments then it can be subject to common sub-expression elimination and loop optimizations."
 *
 * http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0491c/Cacgigch.html
 */
#ifndef CONST
#define CONST __attribute__((const))    //tells the compiler, that result won't change
#endif
/*
 * "__attribute__((pure)) function attribute
 *  Many functions have no effects except to return a value, and their return value depends only on the parameters and global variables.
 *  Functions of this kind can be subject to data flow analysis and might be eliminated."
 *
 *  http://infocenter.arm.com/help/topic/com.arm.doc.dui0491c/Cacigdac.html
 */
#define PURE __attribute__((pure))
#define INLINE __attribute__((always_inline)) __attribute__((flatten)) inline   //flatten inlines all call within this function
#define FUNC_1(ret, func, argType, argName) ret func(argType argName) OVERLOADABLE
#ifndef OVERLOAD_1
#define OVERLOAD_1(ret, func, argType, argName) \
	FUNC_1(ret##16, func, argType##16, argName); \
	FUNC_1(ret##8, func, argType##8, argName); \
	FUNC_1(ret##4, func, argType##4, argName); \
	FUNC_1(ret##3, func, argType##3, argName); \
	FUNC_1(ret##2, func, argType##2, argName); \
	FUNC_1(ret, func, argType, argName);
#endif

#ifndef OVERLOAD_1_RETURN_SCALAR
#define OVERLOAD_1_RETURN_SCALAR(ret, func, argType, argName) \
	FUNC_1(ret, func, argType##16, argName); \
	FUNC_1(ret, func, argType##8, argName); \
	FUNC_1(ret, func, argType##4, argName); \
	FUNC_1(ret, func, argType##3, argName); \
	FUNC_1(ret, func, argType##2, argName); \
	FUNC_1(ret, func, argType, argName);
#endif

#define FUNC_2(ret, func, argType0, argName0, argType1, argName1) ret func(argType0 argName0, argType1 argName1) OVERLOADABLE
#ifndef OVERLOAD_2
#define OVERLOAD_2(ret, func, argType0, argName0, argType1, argName1) \
	FUNC_2(ret##16, func, argType0##16, argName0, argType1##16, argName1); \
	FUNC_2(ret##8, func, argType0##8, argName0, argType1##8, argName1); \
	FUNC_2(ret##4, func, argType0##4, argName0, argType1##4, argName1); \
	FUNC_2(ret##3, func, argType0##3, argName0, argType1##3, argName1); \
	FUNC_2(ret##2, func, argType0##2, argName0, argType1##2, argName1); \
	FUNC_2(ret, func, argType0, argName0, argType1, argName1);
#endif

#ifndef OVERLOAD_2_SCALAR
#define OVERLOAD_2_SCALAR(ret, func, argType0, argName0, argType1, argName1) \
	FUNC_2(ret##16, func, argType0##16, argName0, argType1, argName1); \
	FUNC_2(ret##8, func, argType0##8, argName0, argType1, argName1); \
	FUNC_2(ret##4, func, argType0##4, argName0, argType1, argName1); \
	FUNC_2(ret##3, func, argType0##3, argName0, argType1, argName1); \
	FUNC_2(ret##2, func, argType0##2, argName0, argType1, argName1); \
	FUNC_2(ret, func, argType0, argName0, argType1, argName1);
#endif

#ifndef OVERLOAD_2_RETURN_SCALAR
#define OVERLOAD_2_RETURN_SCALAR(ret, func, argType0, argName0, argType1, argName1) \
	FUNC_2(ret, func, argType0##16, argName0, argType1##16, argName1); \
	FUNC_2(ret, func, argType0##8, argName0, argType1##8, argName1); \
	FUNC_2(ret, func, argType0##4, argName0, argType1##4, argName1); \
	FUNC_2(ret, func, argType0##3, argName0, argType1##3, argName1); \
	FUNC_2(ret, func, argType0##2, argName0, argType1##2, argName1); \
	FUNC_2(ret, func, argType0, argName0, argType1, argName1);
#endif

#ifndef OVERLOAD_2_SCALAR_RETURN_SCALAR
#define OVERLOAD_2_SCALAR_RETURN_SCALAR(ret, func, argType0, argName0, argType1, argName1) \
	FUNC_2(ret, func, argType0##16, argName0, argType1, argName1); \
	FUNC_2(ret, func, argType0##8, argName0, argType1, argName1); \
	FUNC_2(ret, func, argType0##4, argName0, argType1, argName1); \
	FUNC_2(ret, func, argType0##3, argName0, argType1, argName1); \
	FUNC_2(ret, func, argType0##2, argName0, argType1, argName1); \
	FUNC_2(ret, func, argType0, argName0, argType1, argName1);
#endif

#define FUNC_3(ret, func, argType0, argName0, argType1, argName1, argType2, argName2) ret func(argType0 argName0, argType1 argName1, argType2 argName2) OVERLOADABLE
#ifndef OVERLOAD_3
#define OVERLOAD_3(ret, func, argType0, argName0, argType1, argName1, argType2, argName2) \
	FUNC_3(ret##16, func, argType0##16, argName0, argType1##16, argName1, argType2##16, argName2); \
	FUNC_3(ret##8, func, argType0##8, argName0, argType1##8, argName1, argType2##8, argName2); \
	FUNC_3(ret##4, func, argType0##4, argName0, argType1##4, argName1, argType2##4, argName2); \
	FUNC_3(ret##3, func, argType0##3, argName0, argType1##3, argName1, argType2##3, argName2); \
	FUNC_3(ret##2, func, argType0##2, argName0, argType1##2, argName1, argType2##2, argName2); \
	inline FUNC_3(ret, func, argType0, argName0, argType1, argName1, argType2, argName2);
#endif

#ifndef OVERLOAD_3_SCALAR
#define OVERLOAD_3_SCALAR(ret, func, argType0, argName0, argType1, argName1, argType2, argName2) \
	FUNC_3(ret##16, func, argType0##16, argName0, argType1##16, argName1, argType2, argName2); \
	FUNC_3(ret##8, func, argType0##8, argName0, argType1##8, argName1, argType2, argName2); \
	FUNC_3(ret##4, func, argType0##4, argName0, argType1##4, argName1, argType2, argName2); \
	FUNC_3(ret##3, func, argType0##3, argName0, argType1##3, argName1, argType2, argName2); \
	FUNC_3(ret##2, func, argType0##2, argName0, argType1##2, argName1, argType2, argName2); \
	inline FUNC_3(ret, func, argType0, argName0, argType1, argName1, argType2, argName2);
#endif

#define FUNC_4(ret, func, argType0, argName0, argType1, argName1, argType2, argName2, argType3, argName3) ret func(argType0 argName0, argType1 argName1, argType2 argName2, argType3 argName3) OVERLOADABLE

#define FUNC_5(ret, func, argType0, argName0, argType1, argName1, argType2, argName2, argType3, argName3, arg4Type, arg4Name) ret func(argType0 argName0, argType1 argName1, argType2 argName2, argType3 argName3, arg4Type arg4Name) OVERLOADABLE

#ifndef SIMPLE_1
#define SIMPLE_1(ret, func, argType, argName, content) \
	INLINE FUNC_1(ret##16, func, argType##16, argName) \
	{ \
		typedef argType##16 arg_t;\
		typedef ret##16 result_t;\
		return content; \
	} \
	INLINE FUNC_1(ret##8, func, argType##8, argName) \
	{ \
		typedef argType##8 arg_t;\
		typedef ret##8 result_t;\
		return content; \
	} \
	INLINE FUNC_1(ret##4, func, argType##4, argName) \
	{ \
		typedef argType##4 arg_t;\
		typedef ret##4 result_t;\
		return content; \
	} \
	INLINE FUNC_1(ret##3, func, argType##3, argName) \
	{ \
		typedef argType##3 arg_t;\
		typedef ret##3 result_t;\
		return content; \
	} \
	INLINE FUNC_1(ret##2, func, argType##2, argName) \
	{ \
		typedef argType##2 arg_t;\
		typedef ret##2 result_t;\
		return content; \
	} \
	INLINE FUNC_1(ret, func, argType, argName) \
	{ \
		typedef argType arg_t;\
		typedef ret result_t;\
		return content; \
	}
#endif

#ifndef SIMPLE_1_RETURN_SCALAR
#define SIMPLE_1_RETURN_SCALAR(ret, func, argType, argName, content) \
	INLINE FUNC_1(ret, func, argType##16, argName) \
	{ \
		typedef argType##16 arg_t;\
		typedef ret result_t;\
		return content; \
	} \
	INLINE FUNC_1(ret, func, argType##8, argName) \
	{ \
		typedef argType##8 arg_t;\
		typedef ret result_t;\
		return content; \
	} \
	INLINE FUNC_1(ret, func, argType##4, argName) \
	{ \
		typedef argType##4 arg_t;\
		typedef ret result_t;\
		return content; \
	} \
	INLINE FUNC_1(ret, func, argType##3, argName) \
	{ \
		typedef argType##3 arg_t;\
		typedef ret result_t;\
		return content; \
	} \
	INLINE FUNC_1(ret, func, argType##2, argName) \
	{ \
		typedef argType##2 arg_t;\
		typedef ret result_t;\
		return content; \
	} \
	INLINE FUNC_1(ret, func, argType, argName) \
	{ \
		typedef argType arg_t;\
		typedef ret result_t;\
		return content; \
	}
#endif

#ifndef SIMPLE_2
#define SIMPLE_2(ret, func, argType0, argName0, argType1, argName1, content) \
	INLINE FUNC_2(ret##16, func, argType0##16, argName0, argType1##16, argName1) \
	{ \
		typedef argType0##16 arg0_t;\
		typedef argType1##16 arg1_t;\
		typedef ret##16 result_t;\
		return content; \
	} \
	INLINE FUNC_2(ret##8, func, argType0##8, argName0, argType1##8, argName1) \
	{ \
		typedef argType0##8 arg0_t;\
		typedef argType1##8 arg1_t;\
		typedef ret##8 result_t;\
		return content; \
	} \
	INLINE FUNC_2(ret##4, func, argType0##4, argName0, argType1##4, argName1) \
	{ \
		typedef argType0##4 arg0_t;\
		typedef argType1##4 arg1_t;\
		typedef ret##4 result_t;\
		return content; \
	} \
	INLINE FUNC_2(ret##3, func, argType0##3, argName0, argType1##3, argName1) \
	{ \
		typedef argType0##3 arg0_t;\
		typedef argType1##3 arg1_t;\
		typedef ret##3 result_t;\
		return content; \
	} \
	INLINE FUNC_2(ret##2, func, argType0##2, argName0, argType1##2, argName1) \
	{ \
		typedef argType0##2 arg0_t;\
		typedef argType1##2 arg1_t;\
		typedef ret##2 result_t;\
		return content; \
	} \
	INLINE FUNC_2(ret, func, argType0, argName0, argType1, argName1) \
	{ \
		typedef argType0 arg0_t;\
		typedef argType1 arg1_t;\
		typedef ret result_t;\
		return content; \
	}
#endif

#ifndef SIMPLE_2_RETURN_SCALAR
#define SIMPLE_2_RETURN_SCALAR(ret, func, argType0, argName0, argType1, argName1, content) \
	INLINE FUNC_2(ret, func, argType0##16, argName0, argType1##16, argName1) \
	{ \
		typedef argType0##16 arg0_t;\
		typedef argType1##16 arg1_t;\
		typedef ret result_t;\
		return content; \
	} \
	INLINE FUNC_2(ret, func, argType0##8, argName0, argType1##8, argName1) \
	{ \
		typedef argType0##8 arg0_t;\
		typedef argType1##8 arg1_t;\
		typedef ret result_t;\
		return content; \
	} \
	INLINE FUNC_2(ret, func, argType0##4, argName0, argType1##4, argName1) \
	{ \
		typedef argType0##4 arg0_t;\
		typedef argType1##4 arg1_t;\
		typedef ret result_t;\
		return content; \
	} \
	INLINE FUNC_2(ret, func, argType0##3, argName0, argType1##3, argName1) \
	{ \
		typedef argType0##3 arg0_t;\
		typedef argType1##3 arg1_t;\
		typedef ret result_t;\
		return content; \
	} \
	INLINE FUNC_2(ret, func, argType0##2, argName0, argType1##2, argName1) \
	{ \
		typedef argType0##2 arg0_t;\
		typedef argType1##2 arg1_t;\
		typedef ret result_t;\
		return content; \
	} \
	INLINE FUNC_2(ret, func, argType0, argName0, argType1, argName1) \
	{ \
		typedef argType0 arg0_t;\
		typedef argType1 arg1_t;\
		typedef ret result_t;\
		return content; \
	}
#endif

#ifndef SIMPLE_2_SCALAR
#define SIMPLE_2_SCALAR(ret, func, argType0, argName0, argType1, argName1, content) \
	INLINE FUNC_2(ret##16, func, argType0##16, argName0, argType1, argName1) \
	{ \
		typedef argType0##16 arg0_t;\
		typedef argType1 arg1_t;\
		typedef ret##16 result_t;\
		return content; \
	} \
	INLINE FUNC_2(ret##8, func, argType0##8, argName0, argType1, argName1) \
	{ \
		typedef argType0##8 arg0_t;\
		typedef argType1 arg1_t;\
		typedef ret##8 result_t;\
		return content; \
	} \
	INLINE FUNC_2(ret##4, func, argType0##4, argName0, argType1, argName1) \
	{ \
		typedef argType0##4 arg0_t;\
		typedef argType1 arg1_t;\
		typedef ret##4 result_t;\
		return content; \
	} \
	INLINE FUNC_2(ret##3, func, argType0##3, argName0, argType1, argName1) \
	{ \
		typedef argType0##3 arg0_t;\
		typedef argType1 arg1_t;\
		typedef ret##3 result_t;\
		return content; \
	} \
	INLINE FUNC_2(ret##2, func, argType0##2, argName0, argType1, argName1) \
	{ \
		typedef argType0##2 arg0_t;\
		typedef argType1 arg1_t;\
		typedef ret##2 result_t;\
		return content; \
	} \
	//scalar part is skipped, since it is too often already defined for e.g. a version taking two vectors
#endif

#ifndef SIMPLE_3
#define SIMPLE_3(ret, func, argType0, argName0, argType1, argName1, argType2, argName2, content) \
	INLINE FUNC_3(ret##16, func, argType0##16, argName0, argType1##16, argName1, argType2##16, argName2) \
	{ \
		typedef argType0##16 arg0_t;\
		typedef argType1##16 arg1_t;\
		typedef argType2##16 arg2_t;\
		typedef ret##16 result_t;\
		return content; \
	} \
	INLINE FUNC_3(ret##8, func, argType0##8, argName0, argType1##8, argName1, argType2##8, argName2) \
	{ \
		typedef argType0##8 arg0_t;\
		typedef argType1##8 arg1_t;\
		typedef argType2##8 arg2_t;\
		typedef ret##8 result_t;\
		return content; \
	} \
	INLINE FUNC_3(ret##4, func, argType0##4, argName0, argType1##4, argName1, argType2##4, argName2) \
	{ \
		typedef argType0##4 arg0_t;\
		typedef argType1##4 arg1_t;\
		typedef argType2##4 arg2_t;\
		typedef ret##4 result_t;\
		return content; \
	} \
	INLINE FUNC_3(ret##3, func, argType0##3, argName0, argType1##3, argName1, argType2##3, argName2) \
	{ \
		typedef argType0##3 arg0_t;\
		typedef argType1##3 arg1_t;\
		typedef argType2##3 arg2_t;\
		typedef ret##3 result_t;\
		return content; \
	} \
	INLINE FUNC_3(ret##2, func, argType0##2, argName0, argType1##2, argName1, argType2##2, argName2) \
	{ \
		typedef argType0##2 arg0_t;\
		typedef argType1##2 arg1_t;\
		typedef argType2##2 arg2_t;\
		typedef ret##2 result_t;\
		return content; \
	} \
	INLINE FUNC_3(ret, func, argType0, argName0, argType1, argName1, argType2, argName2) \
	{ \
		typedef argType0 arg0_t;\
		typedef argType1 arg1_t;\
		typedef argType2 arg2_t;\
		typedef ret result_t;\
		return content; \
	}
#endif

#ifndef SIMPLE_3_SCALAR
#define SIMPLE_3_SCALAR(ret, func, argType0, argName0, argType1, argName1, argType2, argName2, content) \
	INLINE FUNC_3(ret##16, func, argType0##16, argName0, argType1##16, argName1, argType2, argName2) \
	{ \
		typedef argType0##16 arg0_t;\
		typedef argType1##16 arg1_t;\
		typedef argType2 arg2_t;\
		typedef ret##16 result_t;\
		return content; \
	} \
	INLINE FUNC_3(ret##8, func, argType0##8, argName0, argType1##8, argName1, argType2, argName2) \
	{ \
		typedef argType0##8 arg0_t;\
		typedef argType1##8 arg1_t;\
		typedef argType2 arg2_t;\
		typedef ret##8 result_t;\
		return content; \
	} \
	INLINE FUNC_3(ret##4, func, argType0##4, argName0, argType1##4, argName1, argType2, argName2) \
	{ \
		typedef argType0##4 arg0_t;\
		typedef argType1##4 arg1_t;\
		typedef argType2 arg2_t;\
		typedef ret##4 result_t;\
		return content; \
	} \
	INLINE FUNC_3(ret##3, func, argType0##3, argName0, argType1##3, argName1, argType2, argName2) \
	{ \
		typedef argType0##3 arg0_t;\
		typedef argType1##3 arg1_t;\
		typedef argType2 arg2_t;\
		typedef ret##3 result_t;\
		return content; \
	} \
	INLINE FUNC_3(ret##2, func, argType0##2, argName0, argType1##2, argName1, argType2, argName2) \
	{ \
		typedef argType0##2 arg0_t;\
		typedef argType1##2 arg1_t;\
		typedef argType2 arg2_t;\
		typedef ret##2 result_t;\
		return content; \
	} \
	//scalar version is skipped, since it is already defined by the vector-vector-vector version with "vector" of 1 element
#endif

#ifndef SIMPLE_3_TWO_SCALAR
#define SIMPLE_3_TWO_SCALAR(ret, func, argType0, argName0, argType1, argName1, argType2, argName2, content) \
	INLINE FUNC_3(ret##16, func, argType0##16, argName0, argType1, argName1, argType2, argName2) \
	{ \
		typedef argType0##16 arg0_t;\
		typedef argType1 arg1_t;\
		typedef argType2 arg2_t;\
		typedef ret##16 result_t;\
		return content; \
	} \
	INLINE FUNC_3(ret##8, func, argType0##8, argName0, argType1, argName1, argType2, argName2) \
	{ \
		typedef argType0##8 arg0_t;\
		typedef argType1 arg1_t;\
		typedef argType2 arg2_t;\
		typedef ret##8 result_t;\
		return content; \
	} \
	INLINE FUNC_3(ret##4, func, argType0##4, argName0, argType1, argName1, argType2, argName2) \
	{ \
		typedef argType0##4 arg0_t;\
		typedef argType1 arg1_t;\
		typedef argType2 arg2_t;\
		typedef ret##4 result_t;\
		return content; \
	} \
	INLINE FUNC_3(ret##3, func, argType0##3, argName0, argType1, argName1, argType2, argName2) \
	{ \
		typedef argType0##3 arg0_t;\
		typedef argType1 arg1_t;\
		typedef argType2 arg2_t;\
		typedef ret##3 result_t;\
		return content; \
	} \
	INLINE FUNC_3(ret##2, func, argType0##2, argName0, argType1, argName1, argType2, argName2) \
	{ \
		typedef argType0##2 arg0_t;\
		typedef argType1 arg1_t;\
		typedef argType2 arg2_t;\
		typedef ret##2 result_t;\
		return content; \
	} \
	//scalar version is skipped, since it is already defined by the vector-vector-vector version with "vector" of 1 element
#endif

#ifndef COMPLEX_1
#define COMPLEX_1(ret, func, argType, argName, content) \
	INLINE FUNC_1(ret##16, func, argType##16, argName) \
	{ \
		typedef argType##16 arg_t;\
		typedef ret##16 result_t;\
		typedef int##16 int_t; \
		typedef float##16 float_t; \
		content \
	} \
	INLINE FUNC_1(ret##8, func, argType##8, argName) \
	{ \
		typedef argType##8 arg_t;\
		typedef ret##8 result_t;\
		typedef int##8 int_t; \
		typedef float##8 float_t; \
		content \
	} \
	INLINE FUNC_1(ret##4, func, argType##4, argName) \
	{ \
		typedef argType##4 arg_t;\
		typedef ret##4 result_t;\
		typedef int##4 int_t; \
		typedef float##4 float_t; \
		content \
	} \
	INLINE FUNC_1(ret##3, func, argType##3, argName) \
	{ \
		typedef argType##3 arg_t;\
		typedef ret##3 result_t;\
		typedef int##3 int_t; \
		typedef float##3 float_t; \
		content \
	} \
	INLINE FUNC_1(ret##2, func, argType##2, argName) \
	{ \
		typedef argType##2 arg_t;\
		typedef ret##2 result_t;\
		typedef int##2 int_t; \
		typedef float##2 float_t; \
		content \
	} \
	INLINE FUNC_1(ret, func, argType, argName) \
	{ \
		typedef argType arg_t;\
		typedef ret result_t;\
		typedef int int_t; \
		typedef float float_t; \
		content \
	}
#endif

#ifndef COMPLEX_1_RETURN_SCALAR
#define COMPLEX_1_RETURN_SCALAR(ret, func, argType, argName, content) \
	INLINE FUNC_1(ret, func, argType##16, argName) \
	{ \
		typedef argType##16 arg_t;\
		typedef int##16 int_t; \
		content \
	} \
	INLINE FUNC_1(ret, func, argType##8, argName) \
	{ \
		typedef argType##8 arg_t;\
		typedef int##8 int_t; \
		content \
	} \
	INLINE FUNC_1(ret, func, argType##4, argName) \
	{ \
		typedef argType##4 arg_t;\
		typedef int##4 int_t; \
		content \
	} \
	INLINE FUNC_1(ret, func, argType##3, argName) \
	{ \
		typedef argType##3 arg_t;\
		typedef int##3 int_t; \
		content \
	} \
	INLINE FUNC_1(ret, func, argType##2, argName) \
	{ \
		typedef argType##2 arg_t;\
		typedef int##2 int_t; \
		content \
	} \
	INLINE FUNC_1(ret, func, argType, argName) \
	{ \
		typedef argType arg_t;\
		typedef int int_t; \
		content \
	}
#endif

#ifndef COMPLEX_2
#define COMPLEX_2(ret, func, argType0, argName0, argType1, argName1, content) \
	INLINE FUNC_2(ret##16, func, argType0##16, argName0, argType1##16, argName1) \
	{ \
		typedef argType0##16 arg0_t;\
		typedef argType1##16 arg1_t;\
		typedef ret##16 result_t;\
		typedef int##16 int_t; \
		typedef uint##16 uint_t; \
		typedef float##16 float_t; \
		content \
	} \
	INLINE FUNC_2(ret##8, func, argType0##8, argName0, argType1##8, argName1) \
	{ \
		typedef argType0##8 arg0_t;\
		typedef argType1##8 arg1_t;\
		typedef ret##8 result_t;\
		typedef int##8 int_t; \
		typedef uint##8 uint_t; \
		typedef float##8 float_t; \
		content \
	} \
	INLINE FUNC_2(ret##4, func, argType0##4, argName0, argType1##4, argName1) \
	{ \
		typedef argType0##4 arg0_t;\
		typedef argType1##4 arg1_t;\
		typedef ret##4 result_t;\
		typedef int##4 int_t; \
		typedef uint##4 uint_t; \
		typedef float##4 float_t; \
		content \
	} \
	INLINE FUNC_2(ret##3, func, argType0##3, argName0, argType1##3, argName1) \
	{ \
		typedef argType0##3 arg0_t;\
		typedef argType1##3 arg1_t;\
		typedef ret##3 result_t;\
		typedef int##3 int_t; \
		typedef uint##3 uint_t; \
		typedef float##3 float_t; \
		content \
	} \
	INLINE FUNC_2(ret##2, func, argType0##2, argName0, argType1##2, argName1) \
	{ \
		typedef argType0##2 arg0_t;\
		typedef argType1##2 arg1_t;\
		typedef ret##2 result_t;\
		typedef int##2 int_t; \
		typedef uint##2 uint_t; \
		typedef float##2 float_t; \
		content \
	} \
	INLINE FUNC_2(ret, func, argType0, argName0, argType1, argName1) \
	{ \
		typedef argType0 arg0_t;\
		typedef argType1 arg1_t;\
		typedef ret result_t;\
		typedef int int_t; \
		typedef uint uint_t; \
		typedef float float_t; \
		content \
	}
#endif

#ifndef COMPLEX_3
#define COMPLEX_3(ret, func, argType0, argName0, argType1, argName1, argType2, argName2, content) \
	INLINE FUNC_3(ret##16, func, argType0##16, argName0, argType1##16, argName1, argType2##16, argName2) \
	{ \
		typedef argType0##16 arg0_t;\
		typedef argType1##16 arg1_t;\
		typedef argType2##16 arg2_t;\
		typedef ret##16 result_t;\
		typedef int##16 int_t; \
		content \
	} \
	INLINE FUNC_3(ret##8, func, argType0##8, argName0, argType1##8, argName1, argType2##8, argName2) \
	{ \
		typedef argType0##8 arg0_t;\
		typedef argType1##8 arg1_t;\
		typedef argType2##8 arg2_t;\
		typedef ret##8 result_t;\
		typedef int##8 int_t; \
		content \
	} \
	INLINE FUNC_3(ret##4, func, argType0##4, argName0, argType1##4, argName1, argType2##4, argName2) \
	{ \
		typedef argType0##4 arg0_t;\
		typedef argType1##4 arg1_t;\
		typedef argType2##4 arg2_t;\
		typedef ret##4 result_t;\
		typedef int##4 int_t; \
		content \
	} \
	INLINE FUNC_3(ret##3, func, argType0##3, argName0, argType1##3, argName1, argType2##3, argName2) \
	{ \
		typedef argType0##3 arg0_t;\
		typedef argType1##3 arg1_t;\
		typedef argType2##3 arg2_t;\
		typedef ret##3 result_t;\
		typedef int##3 int_t; \
		content \
	} \
	INLINE FUNC_3(ret##2, func, argType0##2, argName0, argType1##2, argName1, argType2##2, argName2) \
	{ \
		typedef argType0##2 arg0_t;\
		typedef argType1##2 arg1_t;\
		typedef argType2##2 arg2_t;\
		typedef ret##2 result_t;\
		typedef int##2 int_t; \
		content \
	} \
	INLINE FUNC_3(ret, func, argType0, argName0, argType1, argName1, argType2, argName2) \
	{ \
		typedef argType0 arg0_t;\
		typedef argType1 arg1_t;\
		typedef argType2 arg2_t;\
		typedef ret result_t;\
		typedef int int_t; \
		content \
	}
#endif

#ifndef COMPLEX_3_SCALAR
#define COMPLEX_3_SCALAR(ret, func, argType0, argName0, argType1, argName1, argType2, argName2, content) \
	INLINE FUNC_3(ret##16, func, argType0##16, argName0, argType1##16, argName1, argType2, argName2) \
	{ \
		typedef argType0##16 arg0_t;\
		typedef argType1##16 arg1_t;\
		typedef argType2 arg2_t;\
		typedef ret##16 result_t;\
		typedef int##16 int_t; \
		content \
	} \
	INLINE FUNC_3(ret##8, func, argType0##8, argName0, argType1##8, argName1, argType2, argName2) \
	{ \
		typedef argType0##8 arg0_t;\
		typedef argType1##8 arg1_t;\
		typedef argType2 arg2_t;\
		typedef ret##8 result_t;\
		typedef int##8 int_t; \
		content \
	} \
	INLINE FUNC_3(ret##4, func, argType0##4, argName0, argType1##4, argName1, argType2, argName2) \
	{ \
		typedef argType0##4 arg0_t;\
		typedef argType1##4 arg1_t;\
		typedef argType2 arg2_t;\
		typedef ret##4 result_t;\
		typedef int##4 int_t; \
		content \
	} \
	INLINE FUNC_3(ret##3, func, argType0##3, argName0, argType1##3, argName1, argType2, argName2) \
	{ \
		typedef argType0##3 arg0_t;\
		typedef argType1##3 arg1_t;\
		typedef argType2 arg2_t;\
		typedef ret##3 result_t;\
		typedef int##3 int_t; \
		content \
	} \
	INLINE FUNC_3(ret##2, func, argType0##2, argName0, argType1##2, argName1, argType2, argName2) \
	{ \
		typedef argType0##2 arg0_t;\
		typedef argType1##2 arg1_t;\
		typedef argType2 arg2_t;\
		typedef ret##2 result_t;\
		typedef int##2 int_t; \
		content \
	} \
	INLINE FUNC_3(ret, func, argType0, argName0, argType1, argName1, argType2, argName2) \
	{ \
		typedef argType0 arg0_t;\
		typedef argType1 arg1_t;\
		typedef argType2 arg2_t;\
		typedef ret result_t;\
		typedef int int_t; \
		content \
	}
#endif

#define OVERLOAD_ALL_IMAGE_TYPES(ret, func) \
	CONST FUNC_1(ret, func, read_only image1d_t, image); \
	CONST FUNC_1(ret, func, write_only image1d_t, image); \
	CONST FUNC_1(ret, func, read_only image2d_t, image); \
	CONST FUNC_1(ret, func, write_only image2d_t, image); \
	CONST FUNC_1(ret, func, read_only image3d_t, image); \
	/* XXX CONST FUNC_1(ret, func, write_only image3d_t, image); */ \
	CONST FUNC_1(ret, func, read_only image1d_buffer_t, image); \
	CONST FUNC_1(ret, func, write_only image1d_buffer_t, image); \
	CONST FUNC_1(ret, func, read_only image1d_array_t, image); \
	CONST FUNC_1(ret, func, write_only image1d_array_t, image); \
	CONST FUNC_1(ret, func, read_only image2d_array_t, image); \
	CONST FUNC_1(ret, func, write_only image2d_array_t, image);

#define OVERLOAD_ALL_IMAGE_TYPES_1(ret, func, argType, argName) \
	FUNC_2(ret, func, read_only image1d_t, image, argType, argName); \
	FUNC_2(ret, func, write_only image1d_t, image, argType, argName); \
	FUNC_2(ret, func, read_only image2d_t, image, argType, argName); \
	FUNC_2(ret, func, write_only image2d_t, image, argType, argName); \
	FUNC_2(ret, func, read_only image3d_t, image, argType, argName); \
	/* XXX FUNC_2(ret, func, write_only image3d_t, image, argType, argName); */ \
	FUNC_2(ret, func, read_only image1d_buffer_t, image, argType, argName); \
	FUNC_2(ret, func, write_only image1d_buffer_t, image, argType, argName); \
	FUNC_2(ret, func, read_only image1d_array_t, image, argType, argName); \
	FUNC_2(ret, func, write_only image1d_array_t, image, argType, argName); \
	FUNC_2(ret, func, read_only image2d_array_t, image, argType, argName); \
	FUNC_2(ret, func, write_only image2d_array_t, image, argType, argName);

#define OVERLOAD_ALL_IMAGE_TYPES_2(ret, func, arg0Type, arg0Name, arg1Type, arg1Name) \
	FUNC_3(ret, func, read_only image1d_t, image, arg0Type, arg0Name, arg1Type, arg1Name); \
	FUNC_3(ret, func, write_only image1d_t, image, arg0Type, arg0Name, arg1Type, arg1Name); \
	FUNC_3(ret, func, read_only image2d_t, image, arg0Type, arg0Name, arg1Type, arg1Name); \
	FUNC_3(ret, func, write_only image2d_t, image, arg0Type, arg0Name, arg1Type, arg1Name); \
	FUNC_3(ret, func, read_only image3d_t, image, arg0Type, arg0Name, arg1Type, arg1Name); \
	/* XXX FUNC_3(ret, func, write_only image3d_t, image, arg0Type, arg0Name, arg1Type, arg1Name); */ \
	FUNC_3(ret, func, read_only image1d_buffer_t, image, arg0Type, arg0Name, arg1Type, arg1Name); \
	FUNC_3(ret, func, write_only image1d_buffer_t, image, arg0Type, arg0Name, arg1Type, arg1Name); \
	FUNC_3(ret, func, read_only image1d_array_t, image, arg0Type, arg0Name, arg1Type, arg1Name); \
	FUNC_3(ret, func, write_only image1d_array_t, image, arg0Type, arg0Name, arg1Type, arg1Name); \
	FUNC_3(ret, func, read_only image2d_array_t, image, arg0Type, arg0Name, arg1Type, arg1Name); \
	FUNC_3(ret, func, write_only image2d_array_t, image, arg0Type, arg0Name, arg1Type, arg1Name);

#define OVERLOAD_ALL_IMAGE_TYPES_3(ret, func, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name) \
	FUNC_4(ret, func, read_only image1d_t, image, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name); \
	FUNC_4(ret, func, write_only image1d_t, image, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name); \
	FUNC_4(ret, func, read_only image2d_t, image, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name); \
	FUNC_4(ret, func, write_only image2d_t, image, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name); \
	FUNC_4(ret, func, read_only image3d_t, image, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name); \
	/* XXX FUNC_4(ret, func, write_only image3d_t, image, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name); */ \
	FUNC_4(ret, func, read_only image1d_buffer_t, image, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name); \
	FUNC_4(ret, func, write_only image1d_buffer_t, image, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name); \
	FUNC_4(ret, func, read_only image1d_array_t, image, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name); \
	FUNC_4(ret, func, write_only image1d_array_t, image, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name); \
	FUNC_4(ret, func, read_only image2d_array_t, image, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name); \
	FUNC_4(ret, func, write_only image2d_array_t, image, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name);

#define OVERLOAD_ALL_IMAGE_TYPES_4(ret, func, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, arg3Type, arg3Name) \
	FUNC_5(ret, func, read_only image1d_t, image, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, arg3Type, arg3Name); \
	FUNC_5(ret, func, write_only image1d_t, image, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, arg3Type, arg3Name); \
	FUNC_5(ret, func, read_only image2d_t, image, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, arg3Type, arg3Name); \
	FUNC_5(ret, func, write_only image2d_t, image, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, arg3Type, arg3Name); \
	FUNC_5(ret, func, read_only image3d_t, image, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, arg3Type, arg3Name); \
	/* XXX FUNC_5(ret, func, write_only image3d_t, image, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, arg3Type, arg3Name); */ \
	FUNC_5(ret, func, read_only image1d_buffer_t, image, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, arg3Type, arg3Name); \
	FUNC_5(ret, func, write_only image1d_buffer_t, image, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, arg3Type, arg3Name); \
	FUNC_5(ret, func, read_only image1d_array_t, image, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, arg3Type, arg3Name); \
	FUNC_5(ret, func, write_only image1d_array_t, image, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, arg3Type, arg3Name); \
	FUNC_5(ret, func, read_only image2d_array_t, image, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, arg3Type, arg3Name); \
	FUNC_5(ret, func, write_only image2d_array_t, image, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, arg3Type, arg3Name);

#endif /* VC4CL_OVERLOADS_H */

