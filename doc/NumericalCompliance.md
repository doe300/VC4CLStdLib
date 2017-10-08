
## Numerical Compliance

See [What Every Computer Scientist Should Know About Floating-Point Arithmetic](https://docs.oracle.com/cd/E19957-01/806-3568/ncg_goldberg.html).

### Rounding modes
**TODO** how does the VideoCore IV round?? Test for not representable floating-point numbers

### Inf, NaN, Denormals
* Inf is supported
* NaN is not supported

### Relative Error

	If x is a real number that lies between two finite consecutive floating-point numbers a and b, 
	without being equal to one of them, then ulp(x) = |b ? a|, otherwise ulp(x) is the distance between the two non-equal finite floating-point numbers nearest x.
	Moreover, ulp(NaN) is NaN.
[[Source]](https://www.khronos.org/registry/OpenCL/specs/opencl-1.2.pdf#page=319)

The relative ULP is 2^?24 ? 5.96e-08 [[1]](https://en.wikipedia.org/wiki/Machine_epsilon) for single-precision floating-point values.
So e.g. nextafter(1, 2) will return 1 + 2^-23 ? 1.000000119 [[2]](https://en.wikipedia.org/wiki/Single-precision_floating-point_format)

### Built-in Functions
| Function	 | Allowed (in ULP) | Maximal error | Details |
|------------|-----------|------------|
| x + y | correctly rounded |
| x - y | correctly rounded |
| 1.0 / x | 3 |
| x / y | 3 |
| acos | 4 |
| acospi | 5 |
| asin | 4 |
| asinpi | 5 |
| atan | 5 | 
| atan2 | 6 | 
| atanpi | 5 | 
| atan2pi | 6 |
| acosh | 4 |
| asinh | 4 | 
| atanh | 5 | 
| cbrt | 4 |
| ceil | correctly rounded |
| copysign | 0 |
| cos | 4 |
| cosh | 4 |
| cospi | 4 |
| erfc | 16 |
| erf | 16 | 
| exp | 4 |
| exp2 | 4 |
| exp10 | 4 |
| expm1 | 4 |
| fabs | 0 |
| fdim | correctly rounded |
| floor | correctly rounded |
| fma | correctly rounded |
| fmax | 0 |
| fmin | 0 |
| fmod | 0 |
| fract | correctly rounded |
| frexp | 0 |
| hypot | 4 |
| ilogb | 0 |
| ldexp | correctly rounded |
| log | 4 |
| log2 | 4 |
| log10 | 4 |
| log1p | 4 |
| logb | 0 |
| mad | infinite |
| maxmag | 0 |
| minmag | 0 |
| modf | 0 |
| nan | 0 |
| nextafter | 0 |
| pow | 16 |
| pown | 16 |
| powr | 16 |
| remainder | 0 |
| remquo | 0 |
| rint | correclty rounded |
| rootn | 16 |
| round | correclty rounded |
| rsqrt | 4 |
| sin | 4 |
| sincos | 4 (both) |
| sinh | 4 |
| sinpi | 4 |
| sqrt | 4 |
| tan | 5 |
| tanh | 5 |
| tanpi | 6 |
| tgamma | 16 |
| trunc | correctly rounded |
| half_cos | 8192 |
| half_divide | 8192 |
| half_exp | 8192 |
| half_exp2 | 8192 |
| half_exp10 | 8192 |
| half_log | 8192 |
| half_log2 | 8192 |
| half_log10 | 8192 |
| half_powr | 8192 |
| half_recip | 8192 |
| half_rsqrt | 8192 |
| half_sin | 8192 |
| half_sqrt | 8192 |
| half_tan | 8192 |

| native_cos | impl.-defined |
| native_divide | impl.-defined |
| native_exp | impl.-defined |
| native_exp2 | impl.-defined |
| native_exp10 | impl.-defined |
| native_log | impl.-defined |
| native_log2 | impl.-defined |
| native_log10 | impl.-defined |
| native_powr | impl.-defined |
| native_recip | impl.-defined |
| native_rsqrt | impl.-defined |
| native_sin | impl.-defined |
| native_sqrt | impl.-defined |
| native_tan | impl.-defined |

[OpenCL 1.2 FULL PROFILE](https://www.khronos.org/registry/OpenCL/specs/opencl-1.2.pdf#page=320)
[OpenCL 1.2 EMBEDDED PROFILE](https://www.khronos.org/registry/OpenCL/specs/opencl-1.2.pdf#page=346)

Calculations of ULP are done via one of the following methods:
* Plotting the difference between the original function and the approximation with [kmplot](https://edu.kde.org/kmplot/)
* Calculating the result for the functions with the native C implementation and the custom approximation and checking the difference ( **On host only**)

### Edge case behavior
**TODO**
page 325