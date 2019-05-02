#include <stdint.h>
#include <stdbool.h>
#include "io_handling.h"

uint64_t *uint64_to_block(uint64_t a, uint64_t b);
uint64_t to_uint64(uint8_t *buffer);
uint8_t *to_uint8(uint64_t n);
uint64_t **data_to_blocks(byte_t *data, uint64_t data_size,
                          bool is_encrypting);
byte_t *blocks_to_data(uint64_t **blocks, uint64_t blocks_num,
                       uint64_t data_size);