#ifndef HARE_BITUTIL_H
#define HARE_BITUTIL_H

#include <assert.h>
#include <stdint.h>
#include <stddef.h>
#include "hare_platform.h"

#if defined(_MSC_VER)
#include <intrin.h>
#pragma intrinsic(_BitScanForward)
#pragma intrinsic(_BitScanReverse)
#if defined(HARE_CPU_X64)
#pragma intrinsic(_BitScanForward64)
#endif
#endif

#define HARE_CACHE_LINE_LENGTH (64u)

#define HARE_ALIGN(value, alignment) (((value) + ((alignment) - 1u)) & ~((alignment) - 1u))

#define HARE_IS_POWER_OF_TWO(value) ((value) > 0 && (((value) & (~(value) + 1u)) == (value)))

#define HARE_MIN(a, b) ((a) < (b) ? (a) : (b))

#if defined(__GNUC__)
#define HARE_C_COND_EXPECT(exp, c) (__builtin_expect((exp), c))
#else
#define HARE_C_COND_EXPECT(exp, c) (exp)
#endif

inline uint8_t *hare_cache_line_align_buffer(uint8_t *buffer)
{
    size_t remainder = ((size_t)buffer) % HARE_CACHE_LINE_LENGTH;
    return 0 == remainder ? buffer : (buffer + (HARE_CACHE_LINE_LENGTH - remainder));
}

/* Taken from Hacker's Delight as ntz10 at http://www.hackersdelight.org/hdcodetxt/ntz.c.txt */
inline int hare_number_of_trailing_zeroes(int32_t value)
{
#if defined(__GNUC__)
    if (0 == value)
    {
        return 32;
    }

    return __builtin_ctz(value);
#elif defined(_MSC_VER)
    unsigned long r;

    if (_BitScanForward(&r, (unsigned long)value))
    {
        return (int)r;
    }

    return 32;
#else
    char table[32] =
    {
        0, 1, 2, 24, 3, 19, 6, 25,
        22, 4, 20, 10, 16, 7, 12, 26,
        31, 23, 18, 5, 21, 9, 15, 11,
        30, 17, 8, 14, 29, 13, 28, 27
    };

    if (value == 0)
    {
        return 32;
    }

    uint32_t index = (uint32_t)((value & -value) * 0x04D7651F);

    return table[index >> 27u];
#endif
}

inline int hare_number_of_trailing_zeroes_u64(uint64_t value)
{
#if defined(__GNUC__)
    if (0 == value)
    {
        return 64;
    }

    return __builtin_ctzll(value);
#elif defined(_MSC_VER) && defined(HARE_CPU_X64)
    unsigned long r;

    if (_BitScanForward64(&r, (__int64)value))
    {
        return (int)r;
    }

    return 64;
#else
    int lower_tzc = hare_number_of_trailing_zeroes((int32_t) (value & UINT64_C(0xFFFFFFFF)));
    int upper_tzc = hare_number_of_trailing_zeroes((int32_t) ((value >> 32u) & UINT64_C(0xFFFFFFFF)));

    return lower_tzc == 32 ? upper_tzc + lower_tzc : lower_tzc;
#endif
}

inline int hare_number_of_leading_zeroes(int32_t value)
{
#if defined(__GNUC__)
    if (0 == value)
    {
        return 32;
    }

    return __builtin_clz(value);
#elif defined(_MSC_VER)
    unsigned long r;

    if (_BitScanReverse(&r, (unsigned long)value))
    {
        return 31 - (int)r;
    }

    return 32;
#else
#error "do not understand how to clz"
#endif
}

inline int32_t hare_find_next_power_of_two(int32_t value)
{
    value--;

    /*
     * Set all bits below the leading one using binary expansion
     * http://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
     */
    for (size_t i = 1; i < sizeof(value) * 8; i = i * 2)
    {
        value |= (value >> i);
    }

    return value + 1;
}

int32_t hare_randomised_int32();

#endif //HARE_BITUTIL_H
