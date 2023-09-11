#ifndef HARE_DRIVER_PLATFORM_H
#define HARE_DRIVER_PLATFORM_H

/*
 * Determine platform, compiler, and CPU and set defines to be used later.
 * Also, error out here if on a platform that is not supported.
 */

#if defined(_MSC_VER)
    #define HARE_COMPILER_MSVC 1

    #if defined(_M_X64)
        #define HARE_CPU_X64 1

    #else
        #error Unsupported CPU!
    #endif

#elif defined(__GNUC__)
    #define HARE_COMPILER_GCC 1

    #if defined(__llvm__)
        #define HARE_COMPILER_LLVM 1
    #endif

    #if defined(__x86_64__)
        #define HARE_CPU_X64 1
    #endif

#else
    #error Unsupported compiler!
#endif

#endif
