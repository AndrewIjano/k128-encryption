#include <stdio.h>
#include <stdlib.h>
#include "k128.h"

int main() 
{
    char *K = generate_primary_key("p2assword1");
    printf("Primary Key: %s\n", K);

    uint64_t *subkeys = generate_subkeys(K);
    free(K);
    uint64_t b = 0x0123456789abcdef;
    uint64_t c = 0x0123456789abcdef;

    uint64_t *X = malloc(sizeof(uint64_t) * 2);
    X[0] = b;
    X[1] = c;
    printf("X: %016lx %016lx\n", X[0], X[1]);

    uint64_t *Y = block_encryption(X, subkeys);
    free(X);
    printf("Y: %016lx %016lx\n", Y[0], Y[1]);

    uint64_t *Z = block_decryption(Y, subkeys);
    free(Y);
    
    printf("X: %016lx %016lx\n", Z[0], Z[1]);
    free(Z);
    return 0;
}