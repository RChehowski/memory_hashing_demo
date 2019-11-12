//
// Created by Raman Chakhouski on 11/12/19.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

#include "../include/sha256.h"


#define CACHE_LINE_LENGTH 8
typedef unsigned char byte;

#define LE2BE_32U(num) ((num>>24)&0xff) | ((num<<8)&0xff0000) | ((num>>8)&0xff00) | ((num<<24)&0xff000000)


void print_cache_line(const byte* data)
{
    printf("CACHE LINE: ");
    for (const byte* data_p = data; data_p != data + CACHE_LINE_LENGTH; ++data_p)
        printf("0x%02x, ", *data_p);
    printf("[%d bytes]\n", CACHE_LINE_LENGTH);
}

uint32_t hash_block(const byte* data)
{
    print_cache_line(data);

    // Acquire hash
    static SHA256_CTX ctx;
    static byte sha_bytes[SHA256_BLOCK_SIZE];

    sha256_init(&ctx);
    sha256_update(&ctx, data, CACHE_LINE_LENGTH);
    sha256_final(&ctx, sha_bytes);


    // Collate bytes
    uint32_t accumulator = 0;
    const uint32_t* wide_hash = (const uint32_t*)sha_bytes;
    const size_t num_iterations = SHA256_BLOCK_SIZE / sizeof(uint32_t);

    for (size_t i = 0; i < num_iterations; ++i)
        accumulator ^= wide_hash[i];


    // Discard data
    memset(&ctx, 0, sizeof(ctx));
    memset(sha_bytes, 0, sizeof(sha_bytes));

    return accumulator;
}
