//
// Created by Raman Chakhouski on 11/12/19.
//

#ifndef CPPDEMO_MUMBO_JUMBO_H
#define CPPDEMO_MUMBO_JUMBO_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>

#include <arpa/inet.h>

#include "sha256.h"


typedef unsigned char byte;

uint32_t hash_block(const byte* data, const size_t num_bytes);

#endif //CPPDEMO_MUMBO_JUMBO_H
