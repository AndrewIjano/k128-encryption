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

    return 0;
}