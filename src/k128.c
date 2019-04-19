#include "k128.h"

/**
 * @brief Converts a 8 bytes array to a 64 bytes integer
 * 
 * @param buffer     The 8-bit array with 8 elements
 * @return uint64_t  The 64-bit integer converted
 */
static uint64_t to_uint64(uint8_t *buffer)
{
    uint64_t n = 0;
    // Puts the i-th buffer element in the i-th leftmost byte in n
    for (int i = 0; i < 8; i++)
        n |= (uint64_t) buffer[i] << 8*(7 - i);
    return n; 
} 

/**
 * @brief Generates the subskeys from the primary key K
 * 
 * @param R The number of iterations of the algorithm
 * @param K The 128-bit primary key
 * @return uint64_t* The 4*R + 2 64-bit subkeys
 */
uint64_t *generate_subkeys (int R, char *K)
{
    // Allocates the L and k arrays
    uint64_t *L = malloc(sizeof (uint64_t) * (4*R + 4));
    uint64_t *k = malloc(sizeof (uint64_t) * (4*R + 4));

    // Divides the K primary key
    L[0] = to_uint64((uint8_t *)K);
    L[1] = to_uint64((uint8_t *)(K + 8));
    for (int i = 2; i < 4*R + 3; i++)
        L[i] = L[i-1] + 0x9e3779b97f4a7151;
    
    k[0] = 0x8aed2a6bb7e15162;
    for (int i = 1; i < 4*R + 4; i++)
        k[i] = k[i-1] + 0x7c159e3779b97f4a;
    
    uint64_t A = 0, B = 0;
    for (int s = 1; s < 4*R + 4; s++)
    {
        int i = s - 1;
        k[i] = (k[i] + A + B) << 3;
        A = k[i];
        
        L[i] = (L[i] + A + B) << (A + B);
        B = L[i];
    }

    // Creates the final subkeys array
    uint64_t *subkeys = malloc(sizeof(uint64_t) * (4 * R + 2));
    for (int i = 1; i < 4*R + 3; i++)
        subkeys[i-1] = k[i];
        
    return subkeys;
}

/**
 * @brief Generates the 128-bit primary key 
 * 
 * @param password The string password
 * @return char*  The 128-bit primary key
 */
char *generate_primary_key(char *password)
{
    // Exits if the password is invalid
    if (!is_valid_password(password)) 
    {
        fprintf(stderr, "Wrong password style!");
        exit(EXIT_FAILURE);
    }

    // Allocates the primary key and concatenates the remainder
    char *K = malloc(sizeof (char) * 16);
    int pass_length = strlen(password);
    for (int i = 0; i < 16; i++)
        K[i] = password[i%pass_length];
    return K;
}

/**
 * @brief Checks if the password is valid
 * 
 * @param password The string password
 * @return true    If the number of letters >= 2,
 *                 if the number of numbers >= 2,
 *                 and if the password size >= 8
 * @return false   Otherwise
 */
static bool is_valid_password(char *password)
{
    int letters = 0, digits = 0;
    int pass_length = strlen(password);

    // Counts the number of letters and numbers
    for (int i = 0; i < pass_length; i++) {
        char c = password[i];
        if (('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z')) letters++;
        if ('0' <= c && c <= '9') digits++;
    }

    // Returns if the password is valid
    return letters >= 2 && digits >= 2 && pass_length >= 8;
}