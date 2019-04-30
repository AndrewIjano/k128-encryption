#include <stdio.h>
#include <stdint.h>
#include <math.h>

int main()
{
    int e[256];
    int l[256];

    e[0] = 1;
    printf("int exp = {%d", e[0]);
    for (int i = 1; i < 256; i++)
    {
        e[i] = (e[i-1] * 45) % 257;
        printf(", %d", e[i]);
    }
    printf("}\n");

    l[0] = 128;
    for (int i = 0; i < 256; i++) 
        l[e[i]] = i;

    printf("int log = {%d", l[0]);
    for (int i = 1; i < 256; i++)
        printf(", %d", l[i]);
    printf("}\n");
    return 0;
}
