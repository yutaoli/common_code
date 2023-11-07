#ifndef HARE_ATOMIC64_GCC_CPP11_H
#define HARE_ATOMIC64_GCC_CPP11_H

#include <atomic>
#include <thread>
#include <cstdint>

// Implement all operations using C++11 standard library atomics and GCC intrinsics.
// Not as fast as the x64 specializations, but allows Aeron to work on other platforms (e.g. ARM).
// See: https://gcc.gnu.org/onlinedocs/gcc/_005f_005fatomic-Builtins.html

inline void hare_thread_fence()
{
    std::atomic_thread_fence(std::memory_order_acq_rel);
}

inline void hare_fence()
{
    std::atomic_thread_fence(std::memory_order_seq_cst);
}

inline void hare_acquire()
{
    std::atomic_thread_fence(std::memory_order_acquire);
}

inline void hare_release()
{
    std::atomic_thread_fence(std::memory_order_release);
}

// inline void hare_cpu_pause()
// {
//     ;
// }

#define HARE_GET_VOLATILE(dst, src) \
do \
{ \
    dst = src; \
    hare_acquire(); \
} \
while (false)
	
#define HARE_PUT_ORDERED(dst, src) \
do \
{ \
    hare_release(); \
    dst = src; \
} \
while (false)
	
#define HARE_PUT_VOLATILE(dst, src) \
do \
{ \
    hare_release(); \
    dst = src; \
    hare_acquire(); \
} \
while (false)
	
#define HARE_GET_AND_ADD_INT64(original, current, value) \
do \
{ \
	original =  __atomic_fetch_add((volatile int64_t *)&current, (int64_t)value, __ATOMIC_SEQ_CST); \
} \
while (false)

#define HARE_GET_AND_ADD_INT32(original, current, value) \
do \
{ \
    original =  __atomic_fetch_add((volatile int32_t *)&current, (int32_t)value, __ATOMIC_SEQ_CST); \
} \
while (false)
	
#define HARE_CMPXCHG64(original, dst, expected, desired) \
do \
{ \
	if (__atomic_compare_exchange(&dst, &expected, &desired, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)) \
    { \
        original = expected; \
    } \
    else \
    { \
        original =  dst; \
    } \
} \
while (false)
	
inline bool hare_cmpxchg64(volatile int64_t* destination, int64_t expected, int64_t desired)
{
	if (__atomic_compare_exchange(destination, &expected, &desired, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST))
    {
        return true;
    }
	
	return false;
}

inline bool hare_cmpxchgu64(volatile uint64_t* destination, uint64_t expected, uint64_t desired)
{
    if (__atomic_compare_exchange(destination, &expected, &desired, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST))
    {
        return true;
    }
	
	return false;
}

inline bool hare_cmpxchg32(volatile int32_t* destination, int32_t expected, int32_t desired)
{
    if (__atomic_compare_exchange(destination, &expected, &desired, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST))
    {
        return true;
    }
	
	return false;
}

#define HARE_CMPXCHG32(original, dst, expected, desired) \
do \
{ \
	if (__atomic_compare_exchange(&dst, &expected, &desired, false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)) \
    { \
        original = expected; \
    } \
    else \
    { \
        original =  dst; \
    } \
} \
while (false)
	
inline bool hare_cas_int64(volatile int64_t *dst, int64_t expected, int64_t desired)
{
    return hare_cmpxchg64(dst, expected, desired);
}

inline bool hare_cas_uint64(volatile uint64_t *dst, uint64_t expected, uint64_t desired)
{
    return hare_cmpxchgu64(dst, expected, desired);
}

inline bool hare_cas_int32(volatile int32_t *dst, int32_t expected, int32_t desired)
{
    return hare_cmpxchg32(dst, expected, desired);
}

#endif