#ifndef HARE_ATOMIC64_MSVC_H
#define HARE_ATOMIC64_MSVC_H

#include <stdbool.h>
#include <stdint.h>
#include <winsock2.h>
#include <windows.h>
#include <intrin.h>

#define HARE_GET_VOLATILE(dst, src) \
do \
{ \
    dst = src; \
    _ReadWriteBarrier(); \
} \
while (false) \

#define HARE_PUT_ORDERED(dst, src) \
do \
{ \
    _ReadWriteBarrier(); \
    dst = src; \
} \
while (false) \

#define HARE_PUT_VOLATILE(dst, src) \
do \
{ \
    _ReadWriteBarrier(); \
    dst = src; \
    MemoryBarrier(); \
} \
while (false) \

#define HARE_GET_AND_ADD_INT64(original, current, value) \
do \
{ \
    original = _InlineInterlockedAdd64((long long volatile *)&current, (long long)value) - value; \
} \
while (false) \

#define HARE_GET_AND_ADD_INT32(original, current, value) \
do \
{ \
    original = _InlineInterlockedAdd((long volatile *)&current, (long)value) - value; \
} \
while (false) \

inline bool hare_cas_int64(volatile int64_t *dst, int64_t expected, int64_t desired)
{
    int64_t original = _InterlockedCompareExchange64(
        (long long volatile *)dst, (long long)desired, (long long)expected);

    return original == expected;
}

inline bool hare_cas_uint64(volatile uint64_t *dst, uint64_t expected, uint64_t desired)
{
    uint64_t original = _InterlockedCompareExchange64(
        (long long volatile *)dst, (long long)desired, (long long)expected);

    return original == expected;
}

inline bool hare_cas_int32(volatile int32_t *dst, int32_t expected, int32_t desired)
{
    int32_t original = _InterlockedCompareExchange((long volatile *)dst, (long)desired, (long)expected);

    return original == expected;
}

inline void hare_acquire()
{
    _ReadWriteBarrier();
}

inline void hare_release()
{
    _ReadWriteBarrier();
}

#define HARE_DECL_ALIGNED(declaration, amt) __declspec(align(amt))  declaration

#endif //HARE_ATOMIC64_MSVC_H
