#ifndef HARE_ATOMIC_H
#define HARE_ATOMIC_H

#include "hare_platform.h"

#if defined(HARE_COMPILER_GCC) && defined(HARE_CPU_X64)
    #include "hare_atomic64_gcc_x86_64.h"
#elif defined(HARE_COMPILER_MSVC) && defined(HARE_CPU_X64)
    #include "hare_atomic64_msvc.h"
#else
    #error Unsupported platform!
#endif

#endif //HARE_ATOMIC_H
