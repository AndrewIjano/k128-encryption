#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "io_handling.h"

char *generate_primary_key(char *password);
uint64_t *generate_subkeys(char *K);
void encryption_iteration(uint64_t *X, uint64_t *subkeys);
void block_encryption(uint64_t *plaintext_block, uint64_t *subkeys);
byte_t *encrypt(byte_t *plaintext_data, char *password, uint64_t file_size,
                uint64_t *file_size_out);
void decryption_iteration(uint64_t *Y, uint64_t *subkeys);
void block_decryption(uint64_t *ciphertext_block, uint64_t *subkeys);
byte_t *decrypt(byte_t *ciphertext_data, char *password, uint64_t file_size,
                uint64_t *file_size_out);