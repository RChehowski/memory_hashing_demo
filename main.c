#include "include/mumbo_jumbo.h"


// Sensitive data in RAM
byte sensitive_data_in_ram[] = {
        0xe8, 0x13, 0x9e, 0x26, 0xaf, 0xc5, 0x72, 0x44, // mapped on the 1st cache line
        0xbc, 0x6d, 0x78, 0x50, 0x66, 0x2f, 0x66, 0x8f, // -//- 2nd cache line
        0x8e, 0x4f, 0xa0, 0x34, 0x03, 0x7c, 0x72, 0x20, // -//- 3rd cache line
        0x46, 0x12, 0xbd, 0x7b, 0x74, 0xbe, 0xf7, 0x38, // -//- 2rd cache line
};



void secure_fetch(int offset, byte cache_line[CACHE_LINE_LENGTH])
{
    // This values are secured inside the CPU EPROM, they never go into the RAM
    static const uint32_t hashes_in_eprom[] = {
            0x5f608e42,
            0x81edb73a,
            0xc920cf75,
            0x691200be,
    };

    byte* fetch_address = sensitive_data_in_ram + offset;

    const uint32_t block_hash = hash_block(fetch_address);
    const uint32_t eprom_hash = hashes_in_eprom[offset / 8];

    printf("block_hash: 0x%08x, eprom_hash: 0x%08x\n", block_hash, eprom_hash);
    if (block_hash != eprom_hash)
    {
        fprintf(stderr, "Nice try, hacker!\n");
        fflush(stderr);

        raise(SIGBUS);
    }

    memcpy(cache_line, fetch_address, CACHE_LINE_LENGTH);
}

void fetch_demo()
{
    byte fetched_data[CACHE_LINE_LENGTH];

    secure_fetch(0 * CACHE_LINE_LENGTH, fetched_data);
    secure_fetch(1 * CACHE_LINE_LENGTH, fetched_data);
    secure_fetch(2 * CACHE_LINE_LENGTH, fetched_data);
    secure_fetch(3 * CACHE_LINE_LENGTH, fetched_data);
}

int main (void)
{
    printf("=== Under normal conditions everything works fine\n");
    fetch_demo();

    printf("\n\n=== ... until we've 'spoiled' the data in RAM\n");
    *((uint32_t*)sensitive_data_in_ram) = LE2BE_32U(0x0BADC0DE);

    fetch_demo();
}