#include "k128.h"

int exp = {
    1, 45, 226, 147, 190, 69, 21, 174, 120, 3, 135, 164, 184, 56, 207,
    63, 8, 103, 9, 148, 235, 38, 168, 107, 189, 24, 52, 27, 187, 191, 114, 247, 64,
    53, 72, 156, 81, 47, 59, 85, 227, 192, 159, 216, 211, 243, 141, 177, 255, 167,
    62, 220, 134, 119, 215, 166, 17, 251, 244, 186, 146, 145, 100, 131, 241, 51,
    239, 218, 44, 181, 178, 43, 136, 209, 153, 203, 140, 132, 29, 20, 129, 151,
    113, 202, 95, 163, 139, 87, 60, 130, 196, 82, 92, 28, 232, 160, 4, 180, 133,
    74, 246, 19, 84, 182, 223, 12, 26, 142, 222, 224, 57, 252, 32, 155, 36, 78,
    169, 152, 158, 171, 242, 96, 208, 108, 234, 250, 199, 217, 256, 212, 31, 110,
    67, 188, 236, 83, 137, 254, 122, 93, 73, 201, 50, 194, 249, 154, 248, 109, 22,
    219, 89, 150, 68, 233, 205, 230, 70, 66, 143, 10, 193, 204, 185, 101, 176, 210,
    198, 172, 30, 65, 98, 41, 46, 14, 116, 80, 2, 90, 195, 37, 123, 138, 42, 91, 240,
    6, 13, 71, 111, 112, 157, 126, 16, 206, 18, 39, 213, 76, 79, 214, 121, 48, 104,
    54, 117, 125, 228, 237, 128, 106, 144, 55, 162, 94, 118, 170, 197, 127, 61, 175,
    165, 229, 25, 97, 253, 77, 124, 183, 11, 238, 173, 75, 34, 245, 231, 115, 35, 33,
    200, 5, 225, 102, 221, 179, 88, 105, 99, 86, 15, 161, 49, 149, 23, 7, 58, 40
    };

int log = {
    128, 0, 176, 9, 96, 239, 185, 253, 16, 18, 159, 228, 105, 186, 173, 248, 192, 
    56, 194, 101, 79, 6, 148, 252, 25, 222, 106, 27, 93, 78, 168, 130, 112, 237, 
    232, 236, 114, 179, 21, 195, 255, 171, 182, 71, 68, 1, 172, 37, 201, 250, 
    142, 65, 26, 33, 203, 211, 13, 110, 254, 38, 88, 218, 50, 15, 32, 169, 157,
    132, 152, 5, 156, 187, 34, 140, 99, 231, 197, 225, 115, 198, 175, 36, 91, 135,
    102, 39, 247, 87, 244, 150, 177, 183, 92, 139, 213, 84, 121, 223, 170, 246, 62,
    163, 241, 17, 202, 245, 209, 23, 123, 147, 131, 188, 189, 82, 30, 235, 174,
    204, 214, 53, 8, 200, 138, 180, 226, 205, 191, 217, 208, 80, 89, 63, 77, 98,
    52, 10, 72, 136, 181, 86, 76, 46, 107, 158, 210, 61, 60, 3, 19, 251, 151, 81,
    117, 74, 145, 113, 35, 190, 118, 42, 95, 249, 212, 85, 11, 220, 55, 49, 22,
    116, 215, 119, 167, 230, 7, 219, 164, 47, 70, 243, 97, 69, 103, 227, 12, 162,
    59, 28, 133, 24, 4, 29, 41, 160, 143, 178, 90, 216, 166, 126, 238, 141, 83,
    75, 161, 154, 193, 14, 122, 73, 165, 44, 129, 196, 199, 54, 43, 127, 67, 149,
    51, 242, 108, 104, 109, 240, 2, 40, 206, 221, 155, 234, 94, 153, 124, 20, 134,
    207, 229, 66, 184, 64, 120, 45, 58, 233, 100, 31, 146, 144, 125, 57, 111, 224,
    137, 48
    };

/**
 * @brief Generates the subskeys from the primary key K
 * 
 * @param R The number of iterations of the algorithm
 * @param K The 128-bit primary key
 * @return uint64_t* The 4*R + 2 64-bit subkeys
 */
uint64_t *generate_subkeys(int R, char *K)
{
    // Allocates the L and k arrays
    uint64_t *L = malloc(sizeof(uint64_t) * (4 * R + 4));
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

// uint64_t dot(uint64_t a, uint64_t b)
// {

// }