#include "entropy_meter.h"

static void change_data(byte_t *dest, byte_t *src, uint64_t bit_index,
                        uint64_t data_size, int mode)
{
    for (uint64_t i = 0; i < data_size; i++)
        dest[i] = src[i];
     
    dest[bit_index / 8] ^= (1 << (7 - bit_index % 8));

    if (mode == 2 && bit_index + 8 < 8 * data_size)
        dest[bit_index / 8 + 1] ^= 1 << (7 - bit_index % 8);
}

static int hamming_distance(byte_t *data, byte_t *changed_data)
{
    int distance = 0;
    for (int i = 0; i < 16; i++)
        for (int j = 0; j < 8; j++)
            distance += ((data[i] >> j) & 1) ^ ((changed_data[i] >> j) & 1);
    return distance;
}

void entropy_meter(byte_t *input, char *password, uint64_t file_size, int mode)
{
    uint64_t file_size_out;
    byte_t *input_changed = malloc(sizeof(byte_t) * file_size);
    
    byte_t *enc_input = encrypt(input, password, file_size, &file_size_out);

    uint64_t num_blocks = file_size_out/16 - 1;
    uint64_t *blocks_distance_sum = malloc(sizeof(uint64_t) * num_blocks);
    uint64_t *blocks_distance_max = malloc(sizeof(uint64_t) * num_blocks);
    uint64_t *blocks_distance_min = malloc(sizeof(uint64_t) * num_blocks);
    uint64_t *blocks_distance_sqd = malloc(sizeof(uint64_t) * num_blocks);
    uint64_t *blocks_distance_tot = malloc(sizeof(uint64_t) * num_blocks);

    for (uint64_t i = 0; i < num_blocks; i++)
    {
        blocks_distance_sum[i] = 0;
        blocks_distance_max[i] = 0;
        blocks_distance_min[i] = 128;
        blocks_distance_sqd[i] = 0;
        blocks_distance_tot[i] = 0;
    }
    
    for (uint64_t i = 0; i < 8*file_size_out - 128; i++)
    {
        change_data(input_changed, input, i, file_size, mode);
        byte_t *enc_changed = encrypt(input_changed, password, file_size, 
                                      &file_size_out);
        for (uint64_t block = 0; block < num_blocks; block++)
            if (i/128 <= block)
            {
                int distance = hamming_distance(enc_input + 16 * block,
                                                enc_changed + 16 * block);
                if (distance > blocks_distance_max[block])
                    blocks_distance_max[block] = distance;
 
                if (distance < blocks_distance_min[block])
                    blocks_distance_min[block] = distance;

                blocks_distance_sum[block] += distance;
                blocks_distance_sqd[block] += distance * distance;
                blocks_distance_tot[block] += 1;
            }
        free(enc_changed);                                                           
    }
    printf("Mode %d\n", mode);
    printf("          \tMean distance\tMax distance\tMin distance\tStd Deviation\n");
    for (uint64_t i = 0; i < num_blocks; i++)
    {
        uint64_t total = blocks_distance_tot[i];
        uint64_t sum = blocks_distance_sum[i]; 
        float mean = sum / ((i + 1) * 128.0);
        uint64_t sum_sqd = blocks_distance_sqd[i]; 
        double std_dev = sqrt((sum_sqd - 2 * mean * sum + total * mean * mean)/(total - 1.0));
        printf("Block %5lu\t%11.2f\t%12lu\t%12lu\t%12.2f\n", i + 1,
                mean,
                blocks_distance_max[i],
                blocks_distance_min[i],
                std_dev);
    }

    free(blocks_distance_sum);
    free(blocks_distance_max);
    free(blocks_distance_min);
    free(blocks_distance_sqd);
    free(blocks_distance_tot);
}