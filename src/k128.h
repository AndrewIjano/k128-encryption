#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char *generate_primary_key(char *password);
uint64_t *generate_subkeys(int R, char *K);

static uint64_t to_uint64(uint8_t *buffer);
static uint8_t *to_uint8(uint64_t n);
static bool is_valid_password(char *password);
static uint64_t dot(uint64_t b, uint64_t c);
