/*
 * Author: doe300
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef VC4CL_CONFIG_H
#define VC4CL_CONFIG_H

#include "defines.h"

#include "opencl-c.h"

#ifndef NULL
#define NULL ((void *)0)
#endif

/*
 * Math constants
 */
#define M_LOG210    3.01029995663981195214f /* log_2(10) */
#undef NAN
#define NAN         0x7fffffffU /* same as defined in OpenCL C, but as integer */
#undef INF
#define INF			0x7f800000U

#define ALL_BITS_SET 0xFFFFFFFFU

#endif /* VC4CL_CONFIG_H */

