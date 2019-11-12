//
// Created by Raman Chakhouski on 11/12/19.
//

#ifndef CPPDEMO_MUMBO_JUMBO_H
#define CPPDEMO_MUMBO_JUMBO_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

#include "sha256.h"


#define CACHE_LINE_LENGTH 8
typedef unsigned char byte;

#define LE2BE_32U(num) ((num>>24)&0xff) | ((num<<8)&0xff0000) | ((num>>8)&0xff00) | ((num<<24)&0xff000000)


void print_cache_line(const byte* data);

uint32_t hash_block(const byte* data);

#endif //CPPDEMO_MUMBO_JUMBO_H
