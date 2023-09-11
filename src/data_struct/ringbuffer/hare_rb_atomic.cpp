#include "hare_rb_atomic.h"

extern bool hare_cas_int64(volatile int64_t *dst, int64_t expected, int64_t desired);

extern bool hare_cas_uint64(volatile uint64_t *dst, uint64_t expected, uint64_t desired);

extern bool hare_cas_int32(volatile int32_t *dst, int32_t expected, int32_t desired);

extern void hare_acquire();

extern void hare_release();
