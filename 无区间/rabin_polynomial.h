 /*
 * rabin_polynomial.h
 *
 * Created by Joel Lawrence Tucci on 09-March-2011.
 *
 * Copyright (c) 2011 Joel Lawrence Tucci
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the project's author nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <stdint.h>
#include <stdio.h>
#include "rabin_polynomial_constants.h"
#ifdef __APPLE__
#include <unistd.h>

#endif

#ifndef JOEL_TUCCI_RABIN_POLY_C
#define JOEL_TUCCI_RABIN_POLY_C

extern uint64_t rabin_polynomial_prime;
extern unsigned int rabin_sliding_window_size;

extern unsigned int rabin_polynomial_max_block_size;
extern unsigned int rabin_polynomial_min_block_size;

extern unsigned int rabin_polynomial_average_block_size;


/**
 * All the info needed for a rabin polynomial list, namely the start position in the file,
 * the length of the block, the checksum, and the next polynomial
 */
struct rabin_polynomial{
    uint64_t start;
    uint16_t length;
    uint64_t polynomial;
    struct rabin_polynomial *next_polynomial;
};
uint64_t transform_a[12] = {18467,
6334,
26500,
19169,
15724,
11478,
29358,
26962,
24464,
5705,
28145,
23281};
uint64_t transform_b[12] = {16827,
9961,
491,
2995,
11942,
4827,
5436,
32391,
14604,
3902,
153,
2487};
struct rabin_check{
	uint64_t check[12];
};
struct rabin_feature{
	uint64_t feature[RAB_MAX_BLOCK_NUM_SIZE][3];
}feature;
/*
 * Struct used to keep track of rabin polynomials for blocks of memory,
 * since the blocks may or may not end on a boundary, we have to save the
 * current rolling checksum, length, and block checksum so that we can
 * pick up were we left off
 */
struct rab_block_info {
    struct rabin_polynomial *head;
    struct rabin_polynomial *tail;
    uint64_t total_bytes_read;
    unsigned int window_pos;
    char current_poly_finished;
    char *current_window_data;
    uint64_t cur_roll_checksum;
    uint64_t current_block_checksum;
    uint64_t curr_roll_offset;
};

struct edge{
	struct edge* next;
	struct edge* last;
	int num;
	int edge;
}block_sim[RAB_MAX_BLOCK_NUM_SIZE];

//int edgelist[RAB_MAX_BLOCK_NUM_SIZE][RAB_MAX_BLOCK_NUM_SIZE];

uint64_t rabin_prime_feature[4] = {1,
283,
80089,
22665187};

uint64_t recom[RAB_MAX_BLOCK_NUM_SIZE];
short ifre[RAB_MAX_BLOCK_NUM_SIZE];
long long int resort[RAB_MAX_BLOCK_NUM_SIZE];
int resortfeature[RAB_MAX_BLOCK_NUM_SIZE];

void change_average_rabin_block_size(int increment_mode);
int write_rabin_fingerprints_to_binary_file(FILE *file,struct rabin_polynomial *head);
struct rabin_polynomial *read_rabin_polys_from_file_binary(FILE *file);
void free_rabin_fingerprint_list(struct rabin_polynomial *head);

struct rabin_polynomial *gen_new_polynomial(struct rabin_polynomial *tail, uint64_t total_len, uint16_t length, uint64_t rab_sum);

int initialize_rabin_polynomial_defaults();
int initialize_rabin_polynomial(uint64_t prime, unsigned max_size, unsigned int min_size, unsigned int average_block_size);

void print_rabin_poly_to_file(FILE *out_file, struct rabin_polynomial *poly,int new_line);
void print_rabin_poly_list_to_file(FILE *out_file, struct rabin_polynomial *poly);

/*
 * Reads the block given, continuing using the info given in rab_block
 */
//struct rab_block_info *read_rabin_block(void *buf, ssize_t size, struct rab_block_info *cur_block);

struct rabin_polynomial *get_file_rabin_polys(FILE *file_to_read, FILE *file_to_write, FILE *test_info);

#endif
