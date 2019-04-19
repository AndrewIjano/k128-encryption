#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static uint64_t to_uint64(uint8_t *buffer);
char *generate_primary_key(char *password);
uint64_t *generate_subkeys(int R, char *K);
static bool is_valid_password(char *password);