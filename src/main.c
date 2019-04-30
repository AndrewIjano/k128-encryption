#include <stdio.h>
#include <stdlib.h>
#include "k128.h"

#define R 12

int main() 
{
    char *K = generate_primary_key("p2assword1");
    printf("Primary Key:\n");
    printf("%s\n", K);

    uint64_t *subkeys = generate_subkeys(R, K);
    printf("Subkeys:\n");
    for (int i = 0; i < 4*R + 2; i++)
        printf("%016lx\n", subkeys[i]);

    // uint64_t a = 0x0000000000000000;

    uint64_t b = 0x0123456789abcdef;
    // uint8_t c = b >> 16;

    return 0;
}