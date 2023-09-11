#include "hare_bitutil.h"

extern uint8_t *hare_cache_line_align_buffer(uint8_t *buffer);
extern int hare_number_of_trailing_zeroes(int32_t value);
extern int hare_number_of_trailing_zeroes_u64(uint64_t value);
extern int hare_number_of_leading_zeroes(int32_t value);
extern int32_t hare_find_next_power_of_two(int32_t value);
