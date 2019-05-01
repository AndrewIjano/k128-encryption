#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char *generate_primary_key(char *password);
uint64_t *generate_subkeys(char *K);
uint64_t *encryption_iteration(uint64_t *X, uint64_t *subkeys);
uint64_t *block_encryption(uint64_t *plaintext_block, uint64_t *subkeys);
uint64_t *decryption_iteration(uint64_t *Y, uint64_t *subkeys);
uint64_t *block_decryption(uint64_t *ciphertext_block, uint64_t *subkeys);

static uint64_t to_uint64(uint8_t *buffer);
static uint8_t *to_uint8(uint64_t n);
static bool is_valid_password(char *password);
static uint64_t dot(uint64_t b, uint64_t c);
static uint64_t inv_dot(uint64_t a, uint64_t c);
static uint64_t complement(uint64_t a);
