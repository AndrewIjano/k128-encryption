#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include "k128.h"
#include "io_handling.h"

#define MAX_PASSWORD_SIZE 16

void raise_mode_error()
{
    printf("Two execution modes given!\n");
    exit(1);
}

void raise_argument_error(char *option)
{
    printf("No %s given.", option);
    exit(1);
}

int main(int argc, char **argv) 
{
    int mode = -1;
    char filename_in[MAX_FILE_NAME_SIZE + 1];
    char filename_out[MAX_FILE_NAME_SIZE + 1];
    char password[MAX_PASSWORD_SIZE + 1];
    bool erase_file = false;

    printf("oi\n");
    if (argc < 7)
    {
        printf("Wrong arguments given");
        exit(1);
    }

    mode = argv[1][1];
    strcpy(filename_in, argv[3]);

    if (mode == 'c' || mode == 'd')
    {
        if (argc < 8)
        {
            printf("Wrong arguments given");
            exit(1);
        }
        strcpy(filename_out, argv[5]);
        strcpy(password, argv[7]);
        erase_file = argc == 9 && strcmp(argv[8], "-a") == 0;
    }
    else
        strcpy(password, argv[5]);
   
    uint64_t file_size; 
    uint64_t file_size_out;
    byte_t *plaintext;
    byte_t *ciphertext;
    switch (mode)
    {
        case 'c':;
            file_size = get_file_size(filename_in);
            plaintext = malloc(sizeof(byte_t) * file_size);
            read_file_to_array(filename_in, plaintext, file_size);

            ciphertext = encrypt(plaintext, password, file_size, &file_size_out);

            write_array_to_file(filename_out, ciphertext, file_size_out-1);

            free(plaintext);
            free(ciphertext);
            break;
        case 'd':;
            file_size = get_file_size(filename_in);
            ciphertext = malloc(sizeof(byte_t) * file_size);
            read_file_to_array(filename_in, ciphertext, file_size);

            plaintext = decrypt(ciphertext, password, file_size, &file_size_out);

            write_array_to_file(filename_out, plaintext, file_size_out);

            free(ciphertext);
            free(plaintext);
            break;
    }

    return 0;
}