/*
 * Author: doe300
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef VC4CL_EXTENSIONS_H
#define VC4CL_EXTENSIONS_H

#include "_config.h"


	/*
	 * Loop unroll pragma extension
	 *
	 * Defines "#pragma unroll <factor>"
	 *
	 * CLang supports this natively, so we do not need to do anything
	 *
	 * See https://www.khronos.org/registry/OpenCL/extensions/nv/cl_nv_pragma_unroll.txt
	 * See https://clang.llvm.org/docs/AttributeReference.html#pragma-unroll-pragma-nounroll
	 */
#ifndef cl_nv_pragma_unroll
#define cl_nv_pragma_unroll 1
#endif

	/*
	 * ARM core-ID extension
	 *
	 * Adds function
	 * 	uint arm_get_core_id( void )
	 * which returns the ID of the OpenCL Computation Unit, which is always zero
	 *
	 * See https://www.khronos.org/registry/OpenCL/extensions/arm/cl_arm_get_core_id.txt
	 */
#ifndef cl_arm_core_id
#define cl_arm_core_id 1
#endif
	uint arm_get_core_id(void);	//prototype, prevents warning
	uint arm_get_core_id(void)
	{
		return 0;
	}

#endif /* VC4CL_EXTENSIONS_H */

