/*
 * Author: doe300
 *
 * See the file "LICENSE" for the full license governing this code.
 */

#ifndef VC4CL_ATOMICS_H
#define VC4CL_ATOMICS_H

#include "_config.h"
#include "_overloads.h"
#include "_intrinsics.h"

    INLINE int atomic_add(volatile __global int * ptr, int val) OVERLOADABLE
    {
        vc4cl_mutex_lock();
        int old = vc4cl_dma_read(ptr);
        vc4cl_dma_write(ptr, old + val);
        vc4cl_mutex_unlock();
        return old;
    }
    
    INLINE unsigned int atomic_add(volatile __global unsigned int * ptr, unsigned int val) OVERLOADABLE
    {
        vc4cl_mutex_lock();
        unsigned int old = vc4cl_dma_read(ptr);
        vc4cl_dma_write(ptr, old + val);
        vc4cl_mutex_unlock();
        return old;
    }
    
    INLINE int atomic_add(volatile __local int * ptr, int val) OVERLOADABLE
    {
        vc4cl_mutex_lock();
        int old = vc4cl_dma_read(ptr);
        vc4cl_dma_write(ptr, old + val);
        vc4cl_mutex_unlock();
        return old;
    }
    
    INLINE unsigned int atomic_add(volatile __local unsigned int * ptr, unsigned int val) OVERLOADABLE
    {
        vc4cl_mutex_lock();
        unsigned int old = vc4cl_dma_read(ptr);
        vc4cl_dma_write(ptr, old + val);
        vc4cl_mutex_unlock();
        return old;
    }
    
    INLINE int atom_add(volatile __global int *ptr, int val) OVERLOADABLE
    {
        return atomic_add(ptr, val);
    }
    
    INLINE unsigned int atom_add(volatile __global unsigned int *ptr, unsigned int val) OVERLOADABLE
    {
        return atomic_add(ptr, val);
    }
    
    INLINE int atom_add(volatile __local int *ptr, int val) OVERLOADABLE
    {
        return atomic_add(ptr, val);
    }
    
    INLINE unsigned int atom_add(volatile __local unsigned int *ptr, unsigned int val) OVERLOADABLE
    {
        return atomic_add(ptr, val);
    }
    
    INLINE int atomic_sub(volatile __global int * ptr, int val) OVERLOADABLE
    {
        vc4cl_mutex_lock();
        int old = vc4cl_dma_read(ptr);
        vc4cl_dma_write(ptr, old - val);
        vc4cl_mutex_unlock();
        return old;
    }
    
    INLINE unsigned int atomic_sub(volatile __global unsigned int * ptr, unsigned int val) OVERLOADABLE
    {
        vc4cl_mutex_lock();
        unsigned int old = vc4cl_dma_read(ptr);
        vc4cl_dma_write(ptr, old - val);
        vc4cl_mutex_unlock();
        return old;
    }
    
    INLINE int atomic_sub(volatile __local int * ptr, int val) OVERLOADABLE
    {
        vc4cl_mutex_lock();
        int old = vc4cl_dma_read(ptr);
        vc4cl_dma_write(ptr, old - val);
        vc4cl_mutex_unlock();
        return old;
    }
    
    INLINE unsigned int atomic_sub(volatile __local unsigned int * ptr, unsigned int val) OVERLOADABLE
    {
        vc4cl_mutex_lock();
        unsigned int old = vc4cl_dma_read(ptr);
        vc4cl_dma_write(ptr, old - val);
        vc4cl_mutex_unlock();
        return old;
    }
    
    INLINE int atom_sub(volatile __global int *ptr, int val) OVERLOADABLE
    {
        return atomic_sub(ptr, val);
    }
    
    INLINE unsigned int atom_sub(volatile __global unsigned int *ptr, unsigned int val) OVERLOADABLE
    {
        return atomic_sub(ptr, val);
    }
    
    INLINE int atom_sub(volatile __local int *ptr, int val) OVERLOADABLE
    {
        return atomic_sub(ptr, val);
    }
    
    INLINE unsigned int atom_sub(volatile __local unsigned int *ptr, unsigned int val) OVERLOADABLE
    {
        return atomic_sub(ptr, val);
    }
    
    INLINE int atomic_xchg(volatile __global int * ptr, int val) OVERLOADABLE
    {
        vc4cl_mutex_lock();
        int old = vc4cl_dma_read(ptr);
        vc4cl_dma_write(ptr, val);
        vc4cl_mutex_unlock();
        return old;
    }
    
    INLINE unsigned int atomic_xchg(volatile __global unsigned int * ptr, unsigned int val) OVERLOADABLE
    {
        vc4cl_mutex_lock();
        unsigned int old = vc4cl_dma_read(ptr);
        vc4cl_dma_write(ptr, val);
        vc4cl_mutex_unlock();
        return old;
    }
    
    INLINE float atomic_xchg(volatile __global float * ptr, float val) OVERLOADABLE
	{
		vc4cl_mutex_lock();
		float old = vc4cl_dma_read(ptr);
		vc4cl_dma_write(ptr, val);
		vc4cl_mutex_unlock();
		return old;
	}

    INLINE int atomic_xchg(volatile __local int * ptr, int val) OVERLOADABLE
    {
        vc4cl_mutex_lock();
        int old = vc4cl_dma_read(ptr);
        vc4cl_dma_write(ptr, val);
        vc4cl_mutex_unlock();
        return old;
    }
    
    INLINE unsigned int atomic_xchg(volatile __local unsigned int * ptr, unsigned int val) OVERLOADABLE
    {
        vc4cl_mutex_lock();
        unsigned int old = vc4cl_dma_read(ptr);
        vc4cl_dma_write(ptr, val);
        vc4cl_mutex_unlock();
        return old;
    }
    
    INLINE float atomic_xchg(volatile __local float * ptr, float val) OVERLOADABLE
	{
		vc4cl_mutex_lock();
		float old = vc4cl_dma_read(ptr);
		vc4cl_dma_write(ptr, val);
		vc4cl_mutex_unlock();
		return old;
	}

    INLINE int atom_xchg(volatile __global int * ptr, int val) OVERLOADABLE
    {
        return atomic_xchg(ptr, val);
    }
    
    INLINE unsigned int atom_xchg(volatile __global unsigned int * ptr, unsigned int val) OVERLOADABLE
    {
        return atomic_xchg(ptr, val);
    }
    
    INLINE float atom_xchg(volatile __global float * ptr, float val) OVERLOADABLE
	{
		return atomic_xchg(ptr, val);
	}

    INLINE int atom_xchg(volatile __local int * ptr, int val) OVERLOADABLE
    {
        return atomic_xchg(ptr, val);
    }
    
    INLINE unsigned int atom_xchg(volatile __local unsigned int * ptr, unsigned int val) OVERLOADABLE
    {
        return atomic_xchg(ptr, val);
    }
    
    INLINE float atom_xchg(volatile __local float * ptr, float val) OVERLOADABLE
	{
		return atomic_xchg(ptr, val);
	}

    INLINE int atomic_inc(volatile __global int * ptr) OVERLOADABLE
    {
        vc4cl_mutex_lock();
        int old = vc4cl_dma_read(ptr);
        vc4cl_dma_write(ptr, old + 1);
        vc4cl_mutex_unlock();
        return old;
    }
    
    INLINE unsigned int atomic_inc(volatile __global unsigned int * ptr) OVERLOADABLE
    {
        vc4cl_mutex_lock();
        unsigned int old = vc4cl_dma_read(ptr);
        vc4cl_dma_write(ptr, old + 1);
        vc4cl_mutex_unlock();
        return old;
    }
    
    INLINE int atomic_inc(volatile __local int * ptr) OVERLOADABLE
    {
        vc4cl_mutex_lock();
        int old = vc4cl_dma_read(ptr);
        vc4cl_dma_write(ptr, old + 1);
        vc4cl_mutex_unlock();
        return old;
    }
    
    INLINE unsigned int atomic_inc(volatile __local unsigned int * ptr) OVERLOADABLE
    {
        vc4cl_mutex_lock();
        unsigned int old = vc4cl_dma_read(ptr);
        vc4cl_dma_write(ptr, old + 1);
        vc4cl_mutex_unlock();
        return old;
    }
    
    INLINE int atom_inc(volatile __global int * ptr) OVERLOADABLE
    {
        return atomic_inc(ptr);
    }
    
    INLINE unsigned int atom_inc(volatile __global unsigned int * ptr) OVERLOADABLE
    {
        return atomic_inc(ptr);
    }
    
    INLINE int atom_inc(volatile __local int * ptr) OVERLOADABLE
    {
        return atomic_inc(ptr);
    }
    
    INLINE unsigned int atom_inc(volatile __local unsigned int * ptr) OVERLOADABLE
    {
        return atomic_inc(ptr);
    }
    
    INLINE int atomic_dec(volatile __global int * ptr) OVERLOADABLE
    {
        vc4cl_mutex_lock();
        int old = vc4cl_dma_read(ptr);
        vc4cl_dma_write(ptr, old - 1);
        vc4cl_mutex_unlock();
        return old;
    }
    
    INLINE unsigned int atomic_dec(volatile __global unsigned int * ptr) OVERLOADABLE
    {
        vc4cl_mutex_lock();
        unsigned int old = vc4cl_dma_read(ptr);
        vc4cl_dma_write(ptr, old - 1);
        vc4cl_mutex_unlock();
        return old;
    }
    
    INLINE int atomic_dec(volatile __local int * ptr) OVERLOADABLE
    {
        vc4cl_mutex_lock();
        int old = vc4cl_dma_read(ptr);
        vc4cl_dma_write(ptr, old - 1);
        vc4cl_mutex_unlock();
        return old;
    }
    
    INLINE unsigned int atomic_dec(volatile __local unsigned int * ptr) OVERLOADABLE
    {
        vc4cl_mutex_lock();
        unsigned int old = vc4cl_dma_read(ptr);
        vc4cl_dma_write(ptr, old - 1);
        vc4cl_mutex_unlock();
        return old;
    }
    
    INLINE int atom_dec(volatile __global int * ptr) OVERLOADABLE
    {
        return atomic_dec(ptr);
    }
    
    INLINE unsigned int atom_dec(volatile __global unsigned int * ptr) OVERLOADABLE
    {
        return atomic_dec(ptr);
    }
    
    INLINE int atom_dec(volatile __local int * ptr) OVERLOADABLE
    {
        return atomic_dec(ptr);
    }
    
    INLINE unsigned int atom_dec(volatile __local unsigned int * ptr) OVERLOADABLE
    {
        return atomic_dec(ptr);
    }
    
    INLINE int atomic_cmpxchg(volatile __global int * ptr, int compare, int val) OVERLOADABLE
    {
        vc4cl_mutex_lock();
        int old = vc4cl_dma_read(ptr);
        vc4cl_dma_write(ptr, (old == compare) ? val : old);
        vc4cl_mutex_unlock();
        return old;
    }
    
    INLINE unsigned int atomic_cmpxchg(volatile __global unsigned int * ptr, unsigned int compare, unsigned int val) OVERLOADABLE
    {
        vc4cl_mutex_lock();
        unsigned int old = vc4cl_dma_read(ptr);
        vc4cl_dma_write(ptr, (old == compare) ? val : old);
        vc4cl_mutex_unlock();
        return old;
    }
    
    INLINE int atomic_cmpxchg(volatile __local int * ptr, int compare, int val) OVERLOADABLE
    {
        vc4cl_mutex_lock();
        int old = vc4cl_dma_read(ptr);
        vc4cl_dma_write(ptr, (old == compare) ? val : old);
        vc4cl_mutex_unlock();
        return old;
    }
    
    INLINE unsigned int atomic_cmpxchg(volatile __local unsigned int * ptr, unsigned int compare, unsigned int val) OVERLOADABLE
    {
        vc4cl_mutex_lock();
        unsigned int old = vc4cl_dma_read(ptr);
        vc4cl_dma_write(ptr, (old == compare) ? val : old);
        vc4cl_mutex_unlock();
        return old;
    }
    
    INLINE int atom_cmpxchg(volatile __global int * ptr, int compare, int val) OVERLOADABLE
    {
        return atomic_cmpxchg(ptr, compare, val);
    }
    
    INLINE unsigned int atom_cmpxchg(volatile __global unsigned int * ptr, unsigned int compare, unsigned int val) OVERLOADABLE
    {
        return atomic_cmpxchg(ptr, compare, val);
    }
    
    INLINE int atom_cmpxchg(volatile __local int * ptr, int compare, int val) OVERLOADABLE
    {
        return atomic_cmpxchg(ptr, compare, val);
    }
    
    INLINE unsigned int atom_cmpxchg(volatile __local unsigned int * ptr, unsigned int compare, unsigned int val) OVERLOADABLE
    {
        return atomic_cmpxchg(ptr, compare, val);
    }
    
    INLINE int atomic_min(volatile __global int * ptr, int val) OVERLOADABLE
    {
        vc4cl_mutex_lock();
        int old = vc4cl_dma_read(ptr);
        vc4cl_dma_write(ptr, min(old, val));
        vc4cl_mutex_unlock();
        return old;
    }
    
    INLINE unsigned int atomic_min(volatile __global unsigned int * ptr, unsigned int val) OVERLOADABLE
    {
        vc4cl_mutex_lock();
        unsigned int old = vc4cl_dma_read(ptr);
        vc4cl_dma_write(ptr, min(old, val));
        vc4cl_mutex_unlock();
        return old;
    }
    
    INLINE int atomic_min(volatile __local int * ptr, int val) OVERLOADABLE
    {
        vc4cl_mutex_lock();
        int old = vc4cl_dma_read(ptr);
        vc4cl_dma_write(ptr, min(old, val));
        vc4cl_mutex_unlock();
        return old;
    }
    
    INLINE unsigned int atomic_min(volatile __local unsigned int * ptr, unsigned int val) OVERLOADABLE
    {
        vc4cl_mutex_lock();
        unsigned int old = vc4cl_dma_read(ptr);
        vc4cl_dma_write(ptr, min(old, val));
        vc4cl_mutex_unlock();
        return old;
    }
    
    INLINE int atom_min(volatile __global int * ptr, int val) OVERLOADABLE
    {
        return atomic_min(ptr, val);
    }
    
    INLINE unsigned int atom_min(volatile __global unsigned int * ptr, unsigned int val) OVERLOADABLE
    {
        return atomic_min(ptr, val);
    }
    
    INLINE int atom_min(volatile __local int * ptr, int val) OVERLOADABLE
    {
        return atomic_min(ptr, val);
    }
    
    INLINE unsigned int atom_min(volatile __local unsigned int * ptr, unsigned int val) OVERLOADABLE
    {
        return atomic_min(ptr, val);
    }
    
    INLINE int atomic_max(volatile __global int * ptr, int val) OVERLOADABLE
    {
        vc4cl_mutex_lock();
        int old = vc4cl_dma_read(ptr);
        vc4cl_dma_write(ptr, max(old, val));
        vc4cl_mutex_unlock();
        return old;
    }
    
    INLINE unsigned int atomic_max(volatile __global unsigned int * ptr, unsigned int val) OVERLOADABLE
    {
        vc4cl_mutex_lock();
        unsigned int old = vc4cl_dma_read(ptr);
        vc4cl_dma_write(ptr, max(old, val));
        vc4cl_mutex_unlock();
        return old;
    }
    
    INLINE int atomic_max(volatile __local int * ptr, int val) OVERLOADABLE
    {
        vc4cl_mutex_lock();
        int old = vc4cl_dma_read(ptr);
        vc4cl_dma_write(ptr, max(old, val));
        vc4cl_mutex_unlock();
        return old;
    }
    
    INLINE unsigned int atomic_max(volatile __local unsigned int * ptr, unsigned int val) OVERLOADABLE
    {
        vc4cl_mutex_lock();
        unsigned int old = vc4cl_dma_read(ptr);
        vc4cl_dma_write(ptr, max(old, val));
        vc4cl_mutex_unlock();
        return old;
    }
    
    INLINE int atom_max(volatile __global int * ptr, int val) OVERLOADABLE
    {
        return atomic_max(ptr, val);
    }
    
    INLINE unsigned int atom_max(volatile __global unsigned int * ptr, unsigned int val) OVERLOADABLE
    {
        return atomic_max(ptr, val);
    }
    
    INLINE int atom_max(volatile __local int * ptr, int val) OVERLOADABLE
    {
        return atomic_max(ptr, val);
    }
    
    INLINE unsigned int atom_max(volatile __local unsigned int * ptr, unsigned int val) OVERLOADABLE
    {
        return atomic_max(ptr, val);
    }
    
    INLINE int atomic_and(volatile __global int * ptr, int val) OVERLOADABLE
    {
        vc4cl_mutex_lock();
        int old = vc4cl_dma_read(ptr);
        vc4cl_dma_write(ptr, old & val);
        vc4cl_mutex_unlock();
        return old;
    }
    
    INLINE unsigned int atomic_and(volatile __global unsigned int * ptr, unsigned int val) OVERLOADABLE
    {
        vc4cl_mutex_lock();
        unsigned int old = vc4cl_dma_read(ptr);
        vc4cl_dma_write(ptr, old & val);
        vc4cl_mutex_unlock();
        return old;
    }
    
    INLINE int atomic_and(volatile __local int * ptr, int val) OVERLOADABLE
    {
        vc4cl_mutex_lock();
        int old = vc4cl_dma_read(ptr);
        vc4cl_dma_write(ptr, old & val);
        vc4cl_mutex_unlock();
        return old;
    }
    
    INLINE unsigned int atomic_and(volatile __local unsigned int * ptr, unsigned int val) OVERLOADABLE
    {
        vc4cl_mutex_lock();
        unsigned int old = vc4cl_dma_read(ptr);
        vc4cl_dma_write(ptr, old & val);
        vc4cl_mutex_unlock();
        return old;
    }
    
    INLINE int atom_and(volatile __global int * ptr, int val) OVERLOADABLE
    {
        return atomic_and(ptr, val);
    }
    
    INLINE unsigned int atom_and(volatile __global unsigned int * ptr, unsigned int val) OVERLOADABLE
    {
        return atomic_and(ptr, val);
    }
    
    INLINE int atom_and(volatile __local int * ptr, int val) OVERLOADABLE
    {
        return atomic_and(ptr, val);
    }
    
    INLINE unsigned int atom_and(volatile __local unsigned int * ptr, unsigned int val) OVERLOADABLE
    {
        return atomic_and(ptr, val);
    }
    
    INLINE int atomic_or(volatile __global int * ptr, int val) OVERLOADABLE
    {
        vc4cl_mutex_lock();
        int old = vc4cl_dma_read(ptr);
        vc4cl_dma_write(ptr, old | val);
        vc4cl_mutex_unlock();
        return old;
    }
    
    INLINE unsigned int atomic_or(volatile __global unsigned int * ptr, unsigned int val) OVERLOADABLE
    {
        vc4cl_mutex_lock();
        unsigned int old = vc4cl_dma_read(ptr);
        vc4cl_dma_write(ptr, old | val);
        vc4cl_mutex_unlock();
        return old;
    }
    
    INLINE int atomic_or(volatile __local int * ptr, int val) OVERLOADABLE
    {
        vc4cl_mutex_lock();
        int old = vc4cl_dma_read(ptr);
        vc4cl_dma_write(ptr, old | val);
        vc4cl_mutex_unlock();
        return old;
    }
    
    INLINE unsigned int atomic_or(volatile __local unsigned int * ptr, unsigned int val) OVERLOADABLE
    {
        vc4cl_mutex_lock();
        unsigned int old = vc4cl_dma_read(ptr);
        vc4cl_dma_write(ptr, old | val);
        vc4cl_mutex_unlock();
        return old;
    }
    
    INLINE int atom_or(volatile __global int * ptr, int val) OVERLOADABLE
    {
        return atomic_or(ptr, val);
    }
    
    INLINE unsigned int atom_or(volatile __global unsigned int * ptr, unsigned int val) OVERLOADABLE
    {
        return atomic_or(ptr, val);
    }
    
    INLINE int atom_or(volatile __local int * ptr, int val) OVERLOADABLE
    {
        return atomic_or(ptr, val);
    }
    
    INLINE unsigned int atom_or(volatile __local unsigned int * ptr, unsigned int val) OVERLOADABLE
    {
        return atomic_or(ptr, val);
    }
    
    INLINE int atomic_xor(volatile __global int * ptr, int val) OVERLOADABLE
    {
        vc4cl_mutex_lock();
        int old = vc4cl_dma_read(ptr);
        vc4cl_dma_write(ptr, old ^ val);
        vc4cl_mutex_unlock();
        return old;
    }
    
    INLINE unsigned int atomic_xor(volatile __global unsigned int * ptr, unsigned int val) OVERLOADABLE
    {
        vc4cl_mutex_lock();
        unsigned int old = vc4cl_dma_read(ptr);
        vc4cl_dma_write(ptr, old ^ val);
        vc4cl_mutex_unlock();
        return old;
    }
    
    INLINE int atomic_xor(volatile __local int * ptr, int val) OVERLOADABLE
    {
        vc4cl_mutex_lock();
        int old = vc4cl_dma_read(ptr);
        vc4cl_dma_write(ptr, old ^ val);
        vc4cl_mutex_unlock();
        return old;
    }
    
    INLINE unsigned int atomic_xor(volatile __local unsigned int * ptr, unsigned int val) OVERLOADABLE
    {
        vc4cl_mutex_lock();
        unsigned int old = vc4cl_dma_read(ptr);
        vc4cl_dma_write(ptr, old ^ val);
        vc4cl_mutex_unlock();
        return old;
    }
    
    INLINE int atom_xor(volatile __global int * ptr, int val) OVERLOADABLE
    {
        return atomic_xor(ptr, val);
    }
    
    INLINE unsigned int atom_xor(volatile __global unsigned int * ptr, unsigned int val) OVERLOADABLE
    {
        return atomic_xor(ptr, val);
    }
    
    INLINE int atom_xor(volatile __local int * ptr, int val) OVERLOADABLE
    {
        return atomic_xor(ptr, val);
    }
    
    INLINE unsigned int atom_xor(volatile __local unsigned int * ptr, unsigned int val) OVERLOADABLE
    {
        return atomic_xor(ptr, val);
    }

#endif /* VC4CL_ATOMICS_H */

