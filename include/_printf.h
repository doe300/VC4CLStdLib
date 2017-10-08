/*
 * Author: doe300
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef VC4CL_PRINTF
#define VC4CL_PRINTF

#include "_config.h"

//void* vc4cl_get_param(uint);
//void vc4cl_print_char(char);
//
//INLINE int printf(__constant const char * restrict format, ...)
//{
//	__constant const char* formatPtr = format;
//	uint paramIndex = 1;
//	while(*format != '\0')
//	{
//		if(*format == '%')
//		{
//			++formatPtr;
//			switch(*formatPtr)
//			{
//				case '%':
//					vc4cl_print_char('%');
//					break;
//				case 'c':
//					vc4cl_print_char(*vc4cl_get_param(paramIndex));
//				case 's':
//
//			}
//		}
//		else
//			vc4cl_print_char(*formatPtr);
//		++formatPtr;
//	}
//	//TODO
//	return -1;
//}

#endif /* VC4CL_PRINTF */
