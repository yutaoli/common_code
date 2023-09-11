#ifndef HARE_ATOMIC64_GCC_X86_64_H
#define HARE_ATOMIC64_GCC_X86_64_H

#include <stdbool.h>
#include <stdint.h>

#define HARE_GET_VOLATILE(dst, src) \
do \
{ \
    dst = src; \
    __asm__ volatile("" ::: "memory"); \
} \
while (false) \

#define HARE_PUT_ORDERED(dst, src) \
do \
{ \
    __asm__ volatile("" ::: "memory"); \
    dst = src; \
} \
while (false) \

#define HARE_PUT_VOLATILE(dst, src) \
do \
{ \
    __asm__ volatile("" ::: "memory"); \
    dst = src; \
    __asm__ volatile("lock; addl $0, 0(%%rsp)" ::: "cc", "memory"); \
} \
while (false) \

#define HARE_GET_AND_ADD_INT64(original, dst, value) \
do \
{ \
    __asm__ volatile( \
        "lock; xaddq %0, %1" \
        : "=r"(original), "+m"(dst) \
        : "0"(value)); \
} \
while (false) \

#define HARE_GET_AND_ADD_INT32(original, dst, value) \
do \
{ \
    __asm__ volatile( \
        "lock; xaddl %0, %1" \
        : "=r"(original), "+m"(dst) \
        : "0"(value)); \
} \
while (false) \

inline bool hare_cas_int64(volatile int64_t *dst, int64_t expected, int64_t desired)
{
    int64_t original;
    __asm__ volatile(
        "lock; cmpxchgq %2, %1"
        : "=a"(original), "+m"(*dst)
        : "q"(desired), "0"(expected));

    return original == expected;
}

inline bool hare_cas_uint64(volatile uint64_t *dst, uint64_t expected, uint64_t desired)
{
    uint64_t original;
    __asm__ volatile(
        "lock; cmpxchgq %2, %1"
        : "=a"(original), "+m"(*dst)
        : "q"(desired), "0"(expected));

    return original == expected;
}

inline bool hare_cas_int32(volatile int32_t *dst, int32_t expected, int32_t desired)
{
    int32_t original;
    __asm__ volatile(
        "lock; cmpxchgl %2, %1"
        : "=a"(original), "+m"(*dst)
        : "q"(desired), "0"(expected));

    return original == expected;
}

inline void hare_acquire()
{
    volatile int64_t *dummy;
    __asm__ volatile("movq 0(%%rsp), %0" : "=r"(dummy) : : "memory");
}

inline void hare_release()
{
    volatile int64_t dummy = 0;
    (void)dummy;
    __asm__ volatile("" ::: "memory");
}


/*-------------------------------------
 *  Alignment
 *-------------------------------------
 * Note: May not work on local variables.
 * http://gcc.gnu.org/bugzilla/show_bug.cgi?id=24691
 */
#define HARE_DECL_ALIGNED(declaration, amt) declaration __attribute__((aligned(amt)))

#endif //HARE_ATOMIC64_GCC_X86_64_H
