#include "mumbo_jumbo.h"

/**
 * This is the cache line.
 */
typedef struct{
    byte _content[8];
} CacheLine;

/**
 * This is the sensitive data that is stored in the RAM, it can be altered in any time
 * by any thread. However, memory protection system should prevent it.
 */
byte sensitive_data_in_ram[] = {
    0xe8, 0x13, 0x9e, 0x26, 0xaf, 0xc5, 0x72, 0x44, // mapped on the 1st cache line
    0xbc, 0x6d, 0x78, 0x50, 0x66, 0x2f, 0x66, 0x8f, //      -//-     2nd cache line
    0x8e, 0x4f, 0xa0, 0x34, 0x03, 0x7c, 0x72, 0x20, //      -//-     3rd cache line
    0x46, 0x12, 0xbd, 0x7b, 0x74, 0xbe, 0xf7, 0x38, //      -//-     4th cache line
};


/**
 * Fetches CACHE_LINE_LENGTH from the RAM, checking its hash sum.
 *
 * @param offset An offset.
 * @param cache_line Destination cache line buffer.
 */
void fetch(int offset, CacheLine* cache_line)
{
    // This values are secured inside the CPU EPROM, they never go into the RAM
    static const uint32_t hashes_in_eprom[] = {
        0x5f608e42,
        0x81edb73a,
        0xc920cf75,
        0x691200be,
    };

    byte* fetch_address = sensitive_data_in_ram + offset;

    const uint32_t block_hash = hash_block(fetch_address, sizeof(CacheLine));
    const uint32_t eprom_hash = hashes_in_eprom[offset / 8];

    const bool hash_not_valid = block_hash != eprom_hash;

    printf(" > block_hash: 0x%08x, eprom_hash: 0x%08x, %s\n", block_hash, eprom_hash,
            hash_not_valid ? "Error" : "OK");

    if (hash_not_valid)
    {
        fprintf(stderr, "Nice try, hacker!\n");
        fflush(stderr);

        raise(SIGBUS);
    }

    memcpy(cache_line, fetch_address, sizeof(CacheLine));
}

void fetch_demo()
{
    CacheLine cache_line;

    fetch(0 * sizeof(CacheLine), &cache_line);
    fetch(1 * sizeof(CacheLine), &cache_line);
    fetch(2 * sizeof(CacheLine), &cache_line);
    fetch(3 * sizeof(CacheLine), &cache_line);
}

int main (void)
{
    printf("=== Under normal conditions everything works fine\n");
    fetch_demo();

    printf("\n\n");
    const uint32_t spoiler = 0x0badc0de;
    printf("=== ... until we've 'spoiled' the data in RAM with 0x%08x\n", spoiler);
    *((uint32_t*)(sensitive_data_in_ram + sizeof(CacheLine))) = htonl(spoiler);

    fetch_demo();
}