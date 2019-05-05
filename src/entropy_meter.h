#include "utils.h"
#include "k128.h"
#include <math.h>
void entropy_meter(byte_t *input, char *password, uint64_t file_size, int mode);

enum block_sum_type {B_SUM, B_MAX, B_MIN, B_SQD, B_TOT};