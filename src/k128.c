#include "k128.h"

#define R 12

int f_exp[] = {
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

int f_log[] = {
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
        n |= (uint64_t)buffer[i] << 8 * (7 - i);
    return n;
}

/**
 * @brief Converts a 64 bytes integer to a 8 bytes array
 * 
 * @param n          The 64-bit integer
 * @return uint8_t * The 8-bit array with 8 elements
 */
static uint8_t *to_uint8(uint64_t n)
{
    uint8_t *n_array = malloc(sizeof(int8_t) * 8);
    for (int i = 0; i < 8; i++)
        n_array[i] = n >> 8 * (7 - i);
    return n_array;
}

/**
 * @brief Converts an array of bytes to blocks
 * 
 * @param data The array of bytes
 * @param data_size The size of the data
 * @param is_encrypting If true, appends a extra block with the data size
 * @return uint64_t** The array of blocks
 */
static uint64_t **data_to_blocks(byte_t *data, uint64_t data_size,
                                 bool is_encrypting)
{
    // Calculates the data size for a 16-byte multiple plus the data size block
    uint64_t extended_data_size = data_size / 16 + (data_size % 16 > 0) + is_encrypting;
    // Allocates the arrays
    uint64_t **data_blocks = malloc(sizeof(uint64_t *) * extended_data_size);
    uint64_t *data_block;
    for (uint64_t i = 0; i < 16*extended_data_size; i++)
    {
        // Calculates auxiliary values
        uint64_t block_num = i / 16;
        uint64_t block_byte = i % 16;
        uint64_t block_slice = (block_byte < 8) ? 0 : 1;
        
        // Creates a block 
        if (block_byte == 0)
            data_block = malloc(sizeof(uint64_t) * 2);

        // Initializes an slice of the block
        if (block_byte % 8 == 0)
            data_block[block_slice] = 0;

        // Determines the byte to be allocated
        // Fill with 1 bits incomplete blocks
        byte_t data_byte = 0xff;
        if (i < data_size)
            data_byte = data[i];
        
        // Puts the byte in the block
        data_block[block_slice] |= (uint64_t)data_byte << 8 * (7 - block_byte % 8);

        // Puts the block in the array
        if (block_byte == 15)
            data_blocks[block_num] = data_block;
    }
    // Appends the data size block
    if (is_encrypting)
    {
        data_block = malloc(sizeof(uint64_t) * 2); 
        data_block[0] = -1;
        data_block[1] = data_size;
        data_blocks[extended_data_size - 1] = data_block;
    }
    return data_blocks;
}

/**
 * @brief Converts 128-bit blocks to an array of bytes 
 * 
 * @param blocks The array of 128-bit blocks
 * @param blocks_num The number of blocks
 * @param data_size The size of the data (see is_encrypting)
 * @return byte_t* The array of bytes
 */
static byte_t *blocks_to_data(uint64_t **blocks, uint64_t blocks_num,
                              uint64_t data_size)
{
    // Allocates the array of bytes
    byte_t *data = malloc(sizeof(byte_t) * 16 * (blocks_num)); //+ is_encrypting));
    uint64_t i = 0;
    // Puts each byte of the block in the array
    for (uint64_t block_num = 0; block_num < blocks_num; block_num++)
        for (int slice = 0; slice < 2; slice++)
            for (int byte = 0; byte < 8; byte++, i++)
                data[i] = blocks[block_num][slice] >> 8 * (7 - byte);
             
    return data;
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
    for (int i = 0; i < pass_length; i++)
    {
        char c = password[i];
        if (('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z'))
            letters++;
        if ('0' <= c && c <= '9')
            digits++;
    }

    // Returns if the password is valid
    return letters >= 2 && digits >= 2 && pass_length >= 8;
}

/**
 * @brief Returns the dot operation between two numbers
 * 
 * @param b The first 64-bit operand
 * @param c The second 64-bit operand
 * @return uint64_t The operation result
 */
static uint64_t dot(uint64_t b, uint64_t c)
{
    // printf("oix\n");
    uint8_t *A = to_uint8(0);
    uint8_t *B = to_uint8(b);
    uint8_t *C = to_uint8(c);
    for (int i = 0; i < 8; i++)
        A[i] = f_exp[B[i]] ^ f_exp[C[i]];

    return to_uint64(A);
}

/**
 * @brief Returns the inverse dot operation between two numbers
 * such that a = dot(b, c)
 * 
 * @param a The 64-bit operand, result from dot(b, c)
 * @param c The second 64-bit operand from dot(b, c)
 * @return uint64_t The operand b from dot(b, c)
 */
static uint64_t inv_dot(uint64_t a, uint64_t c)
{
    uint8_t *A = to_uint8(a);
    uint8_t *B = to_uint8(0);
    uint8_t *C = to_uint8(c);

    for (int i = 0; i < 8; i++)
        B[i] = f_log[A[i] ^ f_exp[C[i]]];

    return to_uint64(B);
}

/**
 * @brief Returns the complementary of a 64-bit ineteger
 * 
 * @param a The 64-bit integer
 * @return uint64_t The 64-bit integer b such that a + b = 0 mod 2^64
 */
static uint64_t complement(uint64_t a)
{
    return UINT64_MAX - a + 1;
}

/**
 * @brief Generates the subskeys from the primary key K
 * 
 * @param K The 128-bit primary key
 * @return uint64_t* The 4*R + 2 64-bit subkeys
 */
uint64_t *generate_subkeys(char *K)
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
    
    free(L);
    free(k);
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
 * @brief Executes one encryption iteration
 * 
 * @param X The partial encrypted 128-bit plaintext
 * @param subkeys The array of the four 64-bit subkeys 
 */
void encryption_iteration(uint64_t *X, uint64_t *subkeys) 
{
    // The first part of the iteration
    X[0] = dot(X[0], subkeys[0]);
    X[1] += subkeys[1];
    // The second part of the iteration
    uint64_t Y1 = X[0] ^ X[1];
    uint64_t Y2 = dot(dot(subkeys[2], Y1) + Y1, subkeys[3]);
    uint64_t Z  = dot(subkeys[2], Y1) + Y2;

    X[0] ^= Z;
    X[1] ^= Z;
}

/**
 * @brief Executes a block encryption
 * 
 * @param plaintext_block A 128-bit block of the plaintext
 * @param subkeys The 64-bit subkeys
 */
void block_encryption(uint64_t *plaintext_block, uint64_t *subkeys)
{
    // Executes the encryption iterations
    for (int i = 0; i < R; i++)
        encryption_iteration(plaintext_block, subkeys + 4*i);
    // Executes the final iteration
    plaintext_block[0] = dot(plaintext_block[0], subkeys[4*R]);
    plaintext_block[1] += subkeys[4*R + 1];
}

byte_t *encrypt(byte_t *plaintext_data, char *password, uint64_t file_size,
                uint64_t *file_size_out)
{
    // Generates the keys
    char *primary_key = generate_primary_key(password);
    uint64_t *subkeys = generate_subkeys(primary_key);
    // Particionates the data in blocks
    uint64_t **blocks = data_to_blocks(plaintext_data, file_size, true);
    // Calculates the number of blocks
    uint64_t blocks_num = file_size / 16 + (file_size % 16 > 0) + 1;
    // Encrypts each block
    for (uint64_t i = 0; i < blocks_num; i++)
        block_encryption(blocks[i], subkeys);
    // Converts the block to an array fo bytes
    byte_t *data = blocks_to_data(blocks, blocks_num, file_size);
    // Calculates the encrypted file size
    *file_size_out = 16*blocks_num + 1;
    // Frees the memory allocated
    free(primary_key);
    free(subkeys);
    free(blocks);
    return data;
}

/**
 * @brief Executes the one decryption iteration
 * 
 * @param Y The partial decrypted ciphertext
 * @param subkeys The array of the four 64-bit subkeys
 */
void decryption_iteration(uint64_t *Y, uint64_t *subkeys)
{
    // The inverse of the second part of the encryption
    // Calculates Y1 from Y[0] = Xe' and Y[1] = Xf'
    // Xe' ^ Xf' = Xe^Z ^ Xf^Z = Xe ^ Xf = Y1 
    uint64_t Y1 = Y[0] ^ Y[1];
    uint64_t Y2 = dot(dot(subkeys[2], Y1) + Y1, subkeys[3]);
    uint64_t Z = dot(subkeys[2], Y1) + Y2;
    // Recovers the original Xe and Xf
    Y[0] ^= Z;
    Y[1] ^= Z;
    
    // The inverse of the first part of the encryption 
    Y[0] = inv_dot(Y[0], subkeys[0]);
    Y[1] += complement(subkeys[1]);
}

/**
 * @brief Executes a block decryption
 * 
 * @param ciphertext_block A 128-bit block of the ciphertext 
 * @param subkeys The 64-bit subkeys
 */
void block_decryption(uint64_t *ciphertext_block, uint64_t *subkeys)
{
    // Executes the final iteration inverse 
    ciphertext_block[0] = inv_dot(ciphertext_block[0], subkeys[4*R]);
    ciphertext_block[1] += complement(subkeys[4*R + 1]);
    // Executes the decryption iterations
    for (int i = R-1; i >= 0; i--)
        decryption_iteration(ciphertext_block, subkeys + 4*i);
}

byte_t *decrypt(byte_t *ciphertext_data, char *password, uint64_t file_size,
                uint64_t *file_size_out)
{
    // Generates the keys
    char * primary_key = generate_primary_key(password);
    uint64_t *subkeys = generate_subkeys(primary_key);
    // Partionates the data in blocks
    uint64_t **blocks = data_to_blocks(ciphertext_data, file_size, false);
    // Calculates the number of blocks
    uint64_t blocks_num = file_size / 16 + (file_size % 16 > 0);
    // Decrypts each block
    for (uint64_t i = 0; i < blocks_num; i++)
        block_decryption(blocks[i], subkeys);
    // Converts the blocks to an array of bytes
    byte_t *data = blocks_to_data(blocks, blocks_num, file_size);
    // Recovers the original file size
    *file_size_out = to_uint64(data + file_size - 8);
    // Free the memory allocated
    free(primary_key);
    free(subkeys);
    free(blocks);

    return data;
}
