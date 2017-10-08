
## Status of the support of the OpenCL Standard Library

Source: [OpenCL 1.2 Reference Pages](https://www.khronos.org/registry/cl/sdk/1.2/docs/man/xhtml/), section OpenCL Compiler -> Built-in Functions

### Async Copy and Prefetch Functions
| Function | Implementation | Remarks |
|-------------------------------|---------------------------|-------------|
| async_work_group_copy			| read/write of DMA block	| *only executed by the first work-item* |
| async_work_group_strided_copy	| per-item copy				| *only executed by the first work-item* |
| wait_group_events				| *barrier*					| *other work-items wait for the first one to finish* |
| prefetch						| *no-op*					||

### Common Functions
| Function | Implementation | Remarks |
|-----------|-----------------------------------|-------------|
| clamp		| fmin(fmax(a,min), max)			||
| degrees	| fmul(a, 180/PI)					||
| max		| fmax(a, b)						||
| min		| fmin(a, b)						||
| mix		| fadd(x, fmul(fsub(y, x), a))		||
| radians	| fmul(a, PI/180)					||
| sign		| a > 0 ? 1.0 : a <= 0 ? -1.0 : 0.0 ||
| smoothstep| *see reference*					| *check for improvement* |
| step		| a < edge ? 0.0 : 1.0				||

### Explicit Memory Fence Functions
| Function | Implementation | Remarks |
|-------------------|-------------------|-------------|
| mem_fence			| *no-op*			| *memory-access will already be committed immediately* |
| read_mem_fence	| *no-op*			| *memory-access will already be committed immediately* |
| write_mem_fence	| *no-op*			| *memory-access will already be committed immediately* |

### Geometric Functions
| Function | Implementation | Remarks |
|---------------|-----------------------------------|-------------|
| cross			| *a x b*							||
| dot			| *a &middot; b*					||
| distance		| length(fsub(a,b))					||
| length		| sqrt(dot(a,a))					||
| normalize		| fdiv(a, sqrt(dot(a,a)))			||
| fast_distance	| fast_length(fsub(a,b))			||
| fast_length	| half_sqrt(dot(a,a))				||
| fast_normalize| fmul(a, half_rsqrt(dot(a,a)))		||

### Image Functions
| Function | Implementation | Remarks |
|-------------------------------|-------------------|-------------|
| read_imagef					| *not supported*	| *all versions, int and float coordinates, 1D, 2D and 3D, single and array, with and without sampler* |
| read_imagei					| *not supported*	| *all versions, int and float coordinates, 1D, 2D and 3D, single and array, with and without sampler* |
| read_imageui					| *not supported*	| *all versions, int and float coordinates, 1D, 2D and 3D, single and array, with and without sampler* |
| write_imagef					| *not supported*	| *all versions, int and float coordinates, 1D, 2D and 3D, single and array* |
| write_imagei					| *not supported*	| *all versions, int and float coordinates, 1D, 2D and 3D, single and array* |
| write_imageui					| *not supported*	| *all versions, int and float coordinates, 1D, 2D and 3D, single and array* |
| get_image_width				| *not supported*	| *all versions, int and float coordinates, 1D, 2D and 3D, single and array* |
| get_image_height				| *not supported*	| *all versions, int and float coordinates, 1D, 2D and 3D, single and array* |
| get_image_depth				| *not supported*	||
| get_image_channel_data_type	| *not supported*	| *all versions* |
| get_image_channel_order		| *not supported*	| *all versions* |
| get_image_dim					| *not supported*	| *all versions* |
| get_image_array_size			| *not supported*	| *all versions* |

### Integer Functions
| Function | Implementation | Remarks |
|-----------|---------------------------------------|-------------|
| abs		| max(a, -a)							||
| abs_diff	| *see pocl*							||
| add_sat	| clamp(add(a, b), type_min, type_max)	||
| clamp		| min(max(a, min), max)					||
| clz		| clz(a)								||
| hadd		| add(shr(add(a,b),1), carry-bit)		||
| mad24		| add(mul24(a, b), c)					||
| mad_hi	| add(mul_hi(a, b), c)					||
| mad_sat	| clamp(add(mul(a, b), c), t_min, t_max)||
| max		| max(a, b)								| *only signed* |
| min		| min(a, b)								| *only signed* |
| mul24		| mul24(a, b)							||
| mul_hi	| *via mul24*							||
| popcount	| *Brian Kernighan's way*				| *see [source](https://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetKernighan)* |
| rhadd		| add(shr(add(a,b,1),1), carry-bit)		||
| rotate	| ror(a, sub(32, b))					||
| sub_sat	| clamp(sub(a, b), type_min, type_max)	||
| upsample	| or(shl(hi, 8|16), lo)					||

### Math Functions
| Function | Implementation | Remarks |
|---------------|-----------------------------------|-------------|
| acos			| fsub(M_PI/2.0f, asin(a))			||
| acosh			| *via logarithm*					||
| acospi		| acos(fmul(a, M_PI))				||
| asin			| *Taylor-series*					||
| asinh			| *Taylor-series*					||
| asinpi		| asin(fmul(a, M_PI))				||
| atan			| *Taylor-series*					||
| atan2			| atan(fdiv(a,b))					||
| atanh			| *Taylor-series*					||
| atanpi		| atan(fmul(a, M_PI))				||
| atan2pi		| atanpi(fdiv(a,b))					||
| cbrt			| pow(a, 1/3f)						||
| ceil			|									||
| copysign		| and(a, or(b, NAN))				||
| cos			| *Taylor-series*					| *only for -PI to PI* |
| cosh			| *Taylor-series*					||
| cospi			| cos(fmul(a, M_PI))				||
| half_cos		| cos(a)							||
| native_cos	| cos(a)							||
| erf			| *numeric approximation*			| *see [souce](https://en.wikipedia.org/wiki/Error_function#Numerical_approximations)* |
| erfc			| fsub(1, erf(a))					||
| exp			| exp2(fmul(a, M_LOG2E))			||
| half_exp		| native_exp(a)						| *SFU seems to be exact enough* |
| native_exp	| native_exp2(fmul(a, M_LOG2E))		||
| exp2			| exp10(fmul(a, M_LN2F))			||
| half_exp2		| native_exp2(a)					| *SFU seems to be exact enough* |
| native_exp2	| SFU_EXP2(a)						||
| exp10			| exp2(fmul(a, M_LOG210))			||
| half_exp10	| native_exp10(a)					| *SFU seems to be exact enough* |
| native_exp10	| native_exp2(fmul(a, M_LOG210))	||
| expm1			| fsub(exp(a), 1)					||
| fabs			| fmaxabs(a, a)						||
| fdim			| a>b ? fsub(a,b) : 0				||
| floor			|									||
| fma			| fadd(fmul(a, b), c)				| **does not heed rounding** |
| fmax			| fmax(a, b)						||
| fmin			| fmin(a, b)						||
| fmod			| *see reference*					| *fsub(x, fmul(y, trunc(fdiv(x,y))))* |
| fract			| *see reference*					| *fmin(a - floor(a), 0x1.fffffep - 1f ), \*b = floor(a)* |
| frexp			| **TBD**							||
| hypot			| sqrt(fadd(fmul(a, a), fmul(b, b)))| **no special over-/underflow handling** |
| ilogb			| and(shr(a, 23), 0xFF)				||
| ldexp			| fmul(a, i2f(shl(1, and(b, 31))))	| *a \* 2 ^ b* |
| lgamma		| 		  							| *see [Numerical Recipes in C](http://www.nrbook.com/a/bookcpdf.html), chapter 6.1* |
| lgamma_r		| **TBD**							||
| log			| fmul(log2(a), 1.0/M_LOG2E)		||
| half_log		| native_log(a)						| *SFU seems to be exact enough* |
| native_log	| fmul(native_log2(a), 1/M_LOG2E)	||
| log2			| *iterative approximation*			||
| half_log2		| native_log2(a)					| *SFU seems to be exact enough* |
| native_log2	| SFU_LOG2(a)						||
| log10			| fmul(log2(a), 1/M_LOG210)			||
| half_log10	| native_log10(a)					| *SFU seems to be exact enough* |
| native_log10	| fmul(native_log2(a), 1/M_LOG210)	||
| log1p			| log(fadd(1,a))					||
| logb			| **TBD**							||
| mad			| fadd(fmul(a, b), c)				||
| maxmag		| *see reference*					||
| minmag		| *see reference*					||
| modf			| **TBD**							||
| nan			| and(NaN, a)						||
| nextafter		| **TBD**							||
| pow			| **TBD**							||
| pown			| **TBD**							||
| powr			| exp(fmul(b, log(a)))				||
| half_powr		| powr(a,b)							||
| native_powr	| native_exp2(fmul(b, native_log2(a)))||
| half_recip	| native_recip(a)					| *SFU seems to be exact enough* |
| native_recip	| SFU_RECIP(a)						||
| remainder		| **TBD**							||
| remquo		| **TBD**							||
| rint			|									||
| round			|									||
| rootn			| pow(a, fdiv(1, itof(b)))			| *Newton-Verfahren* |
| rsqrt			| fdiv(1, sqrt(a))					||
| half_rsqrt	| native_rsqrt(a)					| *SFU seems to be exact enough* |
| native_rsqrt	| SFU_RSQRT(a)						||
| sin			| *Taylor-series*					| *only for -PI to PI* |
| half_sin		| sin(a)							||
| native_sin	| sin(a)							||
| sincos		| (*b = cos(a), sin(a))				||
| sinh			| *Taylor-series*					||
| sinpi			| sin(fmul(a, M_PI))				||
| sqrt			| *Taylor-series*					||
| half_sqrt		| sqrt(a)							||
| native_sqrt	| SFU_RECIP(SFU_RSQRT(a))			||
| tan			| *Taylor-series*					||
| half_tan		| tan(a)							||
| native_tan	| tan(a)							||
| tanh			| *Taylor-series*					||
| tanpi			| tan(fmul(a, M_PI))				||
| tgamma		| **TBD**							| **exp(lgamma(a) ??** |
| trunc			|									||

### Misc. Vector Functions
| Function | Implementation | Remarks |
|-----------|-------------------------------|-------------|
| shuffle	| shuffle2(a, a, b)				||
| shuffle2	| *via intrinsics*				| *uses vector-rotations* |
| vec_step	| *compiler-intrinsic*			||

### Relational Functions
| Function | Implementation | Remarks |
|---------------|-------------------------------|-------------|
| isequal		| xor(a, b) == 0				| *true = 1 for scalar and -1 for vectors* |
| isnotequal	| xor(a, b) != 0				| *true = 1 for scalar and -1 for vectors* |
| isgreater		| xor(fmin(a, b), a) != 0		| *true = 1 for scalar and -1 for vectors* |
| isgreaterequal| xor(fmax(a, b), a) == 0		| *true = 1 for scalar and -1 for vectors* |
| isless		| xor(fmax(a, b), a) != 0		| *true = 1 for scalar and -1 for vectors* |
| islessequal	| xor(fmin(a, b), a) == 0		| *true = 1 for scalar and -1 for vectors* |
| islessgreater	| or((x < y), (x > y))			| *true = 1 for scalar and -1 for vectors* |
| isfinite		| xor(and(a, INF), INF) != 0	| *true = 1 for scalar and -1 for vectors* |
| isinf			| xor(and(a, INF), INF) == 0	| *true = 1 for scalar and -1 for vectors* |
| isnan			| xor(and(a, NAN), NAN) == 0	| *true = 1 for scalar and -1 for vectors* |
| isnormal		| *check for denormal value*	||
| isordered		| isequal(a, a) && isequal(b, b)||
| isunordered	| isnan(a) || isnan(b)			||
| signbit		| shr(a, 31) / asr(a, 31)		| *arithmetic shift for vector-type* |
| any			| *see reference*				||
| all			| *see reference*				||
| bitselect		| or(and(not(c), a), and(c, b))	||
| select		| msb(c) ? b : a				||

### Synchronization Function
| Function | Implementation | Remarks |
|---------------|-----------|-------------|
| barrier		| 			| ** Via semaphores per work-item, increase all other, then wait for own (#work-items times) ** |

### Vector Data Load and Store Functions
| Function | Implementation | Remarks |
|---------------|---------------------------------------|-------------|
| vloadn		| load(add(b, mul(a, vector-size))		| (a * vector-count + b)* |
| vload_half	| *not supported*						||
| vload_halfn	| *not supported*						||
| vloada_halfn	| *not supported*						||
| vstoren		| store(add(c, mul(b, vector-size), a)	| *\*(b * vector-count + c) = a* |
| vstore_half	| *not supported*						||
| vstore_halfn	| *not supported*						||
| vstorea_halfn	| *not supported*						||


### Work-Item Functions
| Function | Implementation | Remarks |
|-------------------|-------------------|-------------|
| get_global_id		| *register-read*	| *passed via UNIFORM* |
| get_global_size	| *register-read*	| *passed via UNIFORM* |
| get_global_offset	| *register-read*	| *passed via UNIFORM* |
| get_local_id		| *register-read*	| *passed via UNIFORM* |
| get_local_size	| *register-read*	| *passed via UNIFORM* |
| get_num_groups	| *register-read*	| *passed via UNIFORM* |
| get_group_id		| *register-read*	| *passed via UNIFORM* |
| get_work_dim		| *register-read*	| *passed via UNIFORM* |

### Atomic Functions
| Function | Implementation | Remarks |
|-------------------|---------------------------------------------------|--------------------------|
| atomic_add		| old =	\*p, \*p = add(old, val)					| *enclosed in global mutex-lock* |
| atomic_sub		| old = \*p, \*p = sub(old, val)					| *enclosed in global mutex-lock* |
| atomic_xchg		| old = \*p, \*p = val								| *enclosed in global mutex-lock* |
| atomic_inc		| old = \*p, \*p = add(old, 1)						| *enclosed in global mutex-lock* |
| atomic_dec		| old = \*p, \*p = sub(old, 1)						| *enclosed in global mutex-lock* |
| atomic_cmpxchg	| old = \*p, tmp = xor(old,cmp), \*p = val (if 0)	| *enclosed in global mutex-lock* |
| atomic_min		| old = \*p, \*p = min(old, val)					| *enclosed in global mutex-lock* |
| atomic_max		| old = \*p, \*p = max(old, val)					| *enclosed in global mutex-lock* |
| atomic_and		| old = \*p, \*p = and(old, val)					| *enclosed in global mutex-lock* |
| atomic_or			| old = \*p, \*p = or(old, val)						| *enclosed in global mutex-lock* |
| atomic_xor		| old = \*p, \*p = xor(old, val)					| *enclosed in global mutex-lock* |

### printF
**TODO** OpenCL 1.2 pages 286+