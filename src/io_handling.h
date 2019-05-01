#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILE_NAME_SIZE 100

typedef unsigned char byte_t;

long get_file_size(char file_name[]);
void read_file_to_array(char file_name[], byte_t file_bytes[], long file_size);
void write_array_to_file(char file_name[], byte_t file_bytes[], long file_size);
