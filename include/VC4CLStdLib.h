/*
 * General header for the VC4CLStdlib implementation, contains all required headers
 *
 * Author: doe300
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef VC4CLSTDLIB_H
#define VC4CLSTDLIB_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "_config.h"
#include "_extensions.h"
#include "_conversions.h"
#include "_common.h"
#include "_math.h"
#include "_integer.h"
#include "_geometric.h"
#include "_relational.h"
#include "_work_items.h"
#include "_vector.h"
#include "_synchronization.h"
#include "_async.h"
#include "_atomics.h"
#include "_images.h"
#include "_printf.h"
#include "_spir_mangling.h"
#include "_clcxx_mangling.h"

#undef ALL_BITS_SET
#undef OVERLOADABLE
#undef CONST
#undef PURE
#undef INLINE
#undef FUNC_1
#undef OVERLOAD_1
#undef OVERLOAD_1_RETURN_SCALAR
#undef FUNC_2
#undef OVERLOAD_2
#undef OVERLOAD_2_SCALAR
#undef OVERLOAD_2_RETURN_SCALAR
#undef OVERLOAD_2_SCALAR_RETURN_SCALAR
#undef FUNC_3
#undef OVERLOAD_3
#undef OVERLOAD_3_SCALAR
#undef FUNC_4
#undef FUNC_5
#undef SIMPLE_1
#undef SIMPLE_1_RETURN_SCALAR
#undef SIMPLE_2
#undef SIMPLE_2_RETURN_SCALAR
#undef SIMPLE_2_SCALAR
#undef SIMPLE_3
#undef SIMPLE_3_SCALAR
#undef SIMPLE_3_TWO_SCALAR
#undef COMPLEX_1
#undef COMPLEX_1_RETURN_SCALAR
#undef COMPLEX_2
#undef COMPLEX_3
#undef COMPLEX_3_SCALAR
#undef OVERLOAD_ALL_IMAGE_TYPES
#undef OVERLOAD_ALL_IMAGE_TYPES_1
#undef OVERLOAD_ALL_IMAGE_TYPES_2
#undef OVERLOAD_ALL_IMAGE_TYPES_3
#undef OVERLOAD_ALL_IMAGE_TYPES_4

#ifdef __cplusplus
}
#endif

#endif /* VC4CLSTDLIB_H */

