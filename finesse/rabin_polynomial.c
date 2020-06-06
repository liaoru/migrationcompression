/*
 * rabin_polynomial.c
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "rabin_polynomial.h"
#include "rabin_polynomial_constants.h"

uint64_t rabin_polynomial_prime=RAB_POLYNOMIAL_REM;

unsigned int rabin_sliding_window_size=RAB_POLYNOMIAL_WIN_SIZE;

unsigned int rabin_polynomial_max_block_size=RAB_MAX_BLOCK_SIZE;
unsigned int rabin_polynomial_min_block_size=RAB_MIN_BLOCK_SIZE;

unsigned int rabin_polynomial_average_block_size=RAB_POLYNOMIAL_AVG_BLOCK_SIZE;

int rabin_poly_init_completed=0;

int testsetnum;

int testmaxblocksize;

uint64_t *polynomial_lookup_buf;

/**
 * Prints the list of rabin polynomials to the given file
 */
void print_rabin_poly_list_to_file(FILE *out_file, struct rabin_polynomial *poly) {

    struct rabin_polynomial *cur_poly=poly;

    while(cur_poly != NULL) {
        print_rabin_poly_to_file(out_file,cur_poly,1);
        cur_poly=cur_poly->next_polynomial;
    }

}

/**
 * Prints a given rabin polynomial to file in the format:
 * start,length hash
 */
void print_rabin_poly_to_file(FILE *out_file, struct rabin_polynomial *poly,int new_line) {

    if(poly == NULL)
        return;

    fprintf(out_file, "%llu,%u %llu",poly->start,poly->length,poly->polynomial);

    if(new_line)
        fprintf(out_file, "\n");
}

/*
 * Initialize the algorithm with the default params.
 */
int initialize_rabin_polynomial_defaults() {



    if(rabin_poly_init_completed != 0)
        return 1; //Nothing to do

    polynomial_lookup_buf=malloc(sizeof(uint64_t)*RAB_POLYNOMIAL_MAX_WIN_SIZE);

    if(polynomial_lookup_buf == NULL) {
        fprintf(stderr, "Could not initialize rabin polynomial lookaside buffer, out of memory\n");
        return 0;
    }

    int index=0;
    uint64_t curPower=1;
    //Initialize the lookup values we will need later
    for(index=0;index<RAB_POLYNOMIAL_MAX_WIN_SIZE;index++) {
        //TODO check if max window size is a power of 2
        //and if so use shifters instead of multiplication
        polynomial_lookup_buf[index]=curPower;
        curPower*=rabin_polynomial_prime;
    }


    rabin_poly_init_completed=1;

    return 1;

}


/**
 * Modifies the average block size, checking to make sure it doesn't
 * go above the max or below the min
 *//*
void change_average_rabin_block_size(int increment_mode) {

    if(increment_mode && rabin_polynomial_average_block_size < rabin_polynomial_max_block_size) {
        rabin_polynomial_average_block_size++;
    } else if(!increment_mode && rabin_polynomial_average_block_size > rabin_polynomial_min_block_size) {
        rabin_polynomial_average_block_size--;
    }
}
*/
/**
 * Initalizes the algorithm with the provided paramters
 */
int initialize_rabin_polynomial(uint64_t prime, unsigned max_size, unsigned int min_size, unsigned int average_block_size) {

    rabin_polynomial_prime=prime;
    rabin_polynomial_max_block_size=max_size;
    rabin_polynomial_min_block_size=min_size;
    rabin_polynomial_average_block_size=average_block_size;

    return initialize_rabin_polynomial_defaults();


}


/*
 * Generate a new fingerprint with the given info and add it to the tail
 */
struct rabin_polynomial *gen_new_polynomial(struct rabin_polynomial *tail, uint64_t total_len, uint16_t length, uint64_t rab_sum) {

    struct rabin_polynomial *next=malloc(sizeof(struct rabin_polynomial));

    if(next == NULL) {
        fprintf(stderr, "Could not allocate memory for rabin fingerprint record!");
        return NULL;
    }

    if(tail != NULL)
        tail->next_polynomial=next;

    next->next_polynomial=NULL;
    next->start=total_len-length;
    next->length=length;
    next->polynomial=rab_sum;

    return next;

}

/*
 * Writes out the fingerprint list in binary form
 */
int write_rabin_fingerprints_to_binary_file(FILE *file,struct rabin_polynomial *head) {

    struct rabin_polynomial *poly=head;

    while(poly != NULL) {
        //size_t ret_val=fwrite(poly, sizeof(struct rabin_polynomial), 1, file);
        size_t ret_val=fprintf(file, "%llu %u %llu\n", poly->start, poly->length, poly->polynomial);

        if(ret_val == 0) {
            fprintf(stderr, "Could not write rabin polynomials to file.");
            return -1;
        }

        poly=poly->next_polynomial;
    }

    return 0;
}

/**
 * Reads a list of rabin fingerprints in binary form
 */
struct rabin_polynomial *read_rabin_polys_from_file_binary(FILE *file) {
    struct rabin_polynomial *head=gen_new_polynomial(NULL,0,0,0);
    struct rabin_polynomial *tail=head;

    if(head == NULL)
        return NULL;

    size_t polys_read=fread(head, sizeof(struct rabin_polynomial), 1, file);

    while(polys_read != 0 && tail != NULL) {
        struct rabin_polynomial *cur_poly=gen_new_polynomial(tail,0,0,0);
        fread(cur_poly, sizeof(struct rabin_polynomial), 1, file);
        tail=cur_poly;
    }

    return head;
}


/*
 * Deallocates the entire fingerprint list
 */
void free_rabin_fingerprint_list(struct rabin_polynomial *head) {

    struct rabin_polynomial *cur_poly,*next_poly;

    cur_poly=head;

    while(cur_poly != NULL) {
        next_poly=cur_poly->next_polynomial;
        free(cur_poly);
        cur_poly=next_poly;
    }

}
void cleanedge()
{
	ssize_t i;
	for(i = 0; i < RAB_MAX_BLOCK_NUM_SIZE; i++)
	{
		edgelist[i][0] = 1;
	}
}
void addedge(int a1, int b1)
{
	edgelist[a1][edgelist[a1][0]] = b1;
	edgelist[a1][0]++;
	edgelist[b1][edgelist[b1][0]] = a1;
	edgelist[b1][0]++;
	return;
}

int cmp(const void *p1, const void *p2)
{
	if(edgelist[*(int*)p1][0] > edgelist[*(int*)p2][0])
	{
		return -1;
	}
	else
	{
		return 1;
	}
}

void recombination()
{
	ssize_t i, j, k;
	int total = 0;
	int tmp = 0;
	cleanedge();
	printf("clean");
	for(i = 0; i < RAB_MAX_BLOCK_NUM_SIZE; i++)
	{
		//if(i % (RAB_MAX_BLOCK_NUM_SIZE / 100) == 0) printf("r: %d, total: %d\n", i, total);
		for(j = i + 1; j < RAB_MAX_BLOCK_NUM_SIZE; j++)
		{
			/*if(feature.feature[i][0] == feature.feature[j][0] || feature.feature[i][1] == feature.feature[j][1] ||feature.feature[i][2] == feature.feature[j][2])
			{
				total++;
				addedge(i,j);
			}*/
			tmp = 0;
			for(k = 0; k < 3; k++)
			{
				if(feature.feature[i][k] == feature.feature[j][k]) tmp++;
			}
			if(tmp >= 3)
			{
				total++;
				addedge(i,j);
			}
		}
	}


}
void output_block(int* total, int ed, char *file_data, FILE *file_to_write)
{
	ifre[ed] = 1;
	*total += 1;
	fwrite(&(ed), sizeof(int), 1, file_to_write);
	fwrite((file_data + ed * RAB_MAX_BLOCK_SIZE), sizeof(char), RAB_MAX_BLOCK_SIZE, file_to_write);
	return;
}

/*
 * Gets the list of fingerprints from the given file
 */
void output_rec_to_file(char *file_data, FILE *file_to_write)
{
	int total = 0;
	testmaxblocksize = 0;
	testsetnum = 0;
	ssize_t i,j;
	for(i = 0; i < RAB_MAX_BLOCK_NUM_SIZE; i++)
	{
		resort[i] = i;
		ifre[i] = 0;
	}
	qsort(resort, RAB_MAX_BLOCK_NUM_SIZE, sizeof(int), cmp);
	printf("%d %d\n", resort[0], edgelist[resort[0]][0]);
	int tmp1;
	int p1 = 0;
	testmaxblocksize = edgelist[resort[0]][0];
	for(i = 0; i < RAB_MAX_BLOCK_NUM_SIZE; i++)
	{
		if(ifre[resort[i]] == 1)
		{
			continue;
		}
		testsetnum++;
		ifre[resort[i]] = 1;
		total += 1;
		fwrite(&(resort[i]), sizeof(int), 1, file_to_write);
		fwrite((file_data + resort[i] * RAB_MAX_BLOCK_SIZE), sizeof(char), RAB_MAX_BLOCK_SIZE, file_to_write);
		tmp1 = 1;
		while(tmp1<edgelist[resort[i]][0])
		{
			if(ifre[edgelist[resort[i]][tmp1]] != 1)
			output_block(&total, edgelist[resort[i]][tmp1], file_data, file_to_write);
			tmp1++;
		}
	}
	printf("outputblock: %d\n", total);
	return;
}
void freeedge()
{
	struct edge *tmp1, *tmp2;
	for(int i = 0; i < RAB_MAX_BLOCK_NUM_SIZE; i++)
	{
		tmp1 = &block_sim[i];
		while(tmp1->next != NULL)
		{
			tmp2 = tmp1->next;
			tmp1->next = tmp2->next;
			free(tmp2);
		}
	}
	return;
}
struct rabin_polynomial *get_file_rabin_polys(FILE *file_to_read, FILE *file_to_write, FILE *test_info) {
	clock_t start, finish;
	printf("1");
    initialize_rabin_polynomial_defaults();
	printf("2");
    struct rab_block_info *block=NULL;
	char *file_data;
    file_data=malloc(RAB_FILE_READ_BUF_SIZE);


    if(file_data == NULL) {
        fprintf(stderr,"Could not allocate buffer for reading input file to rabin polynomial.\n");
        return NULL;
    }
    ssize_t bytes_read=fread(file_data,1,RAB_FILE_READ_BUF_SIZE,file_to_read);
	printf("3");
    while(bytes_read != 0) {
		if(bytes_read != RAB_FILE_READ_BUF_SIZE)
		{
			printf("!");
			fwrite(file_data, 1, bytes_read, file_to_write);
			break;
		}
		start = clock();
        block=read_rabin_block(file_data,bytes_read,block);
		finish = clock();
		fprintf(test_info, "%f\t", (float)(finish - start) / CLOCKS_PER_SEC);
		start = clock();
		recombination();
		finish = clock();
		fprintf(test_info, "%f\t", (float)(finish - start) / CLOCKS_PER_SEC);
		start = clock();
		output_rec_to_file(file_data, file_to_write);
		finish = clock();
		fprintf(test_info, "%f\t", (float)(finish - start) / CLOCKS_PER_SEC);
		fprintf(test_info, "%d\t%d\n", testsetnum, testmaxblocksize);
		printf("time: %f\n", (float)(finish - start) / CLOCKS_PER_SEC);
        bytes_read=fread(file_data,1,RAB_FILE_READ_BUF_SIZE,file_to_read);
    }
	printf("7");
	//freeedge();
    free(file_data);
    struct rabin_polynomial *head=block->head;
    free(block);
    return head;
}

/**
 * Allocates an empty block
 */
struct rab_block_info *init_empty_block() {

    initialize_rabin_polynomial_defaults();
	struct rab_block_info *block=malloc(sizeof(struct rab_block_info));
    if(block == NULL) {
        fprintf(stderr,"Could not allocate rabin polynomial block, no memory left!\n");
        return NULL;
    }

	block->head=gen_new_polynomial(NULL,0,0,0);

	if(block->head == NULL)
        return NULL; //Couldn't allocate memory

	block->tail=block->head;
	block->cur_roll_checksum=0;
	block->total_bytes_read=0;
	block->window_pos=0;
	block->current_poly_finished=0;

    block->current_window_data=malloc(sizeof(char)*rabin_sliding_window_size);

	if(block->current_window_data == NULL) {
	    fprintf(stderr,"Could not allocate buffer for sliding window data!\n");
	    free(block);
	    return NULL;
	}
    int i;
	for(i=0;i<rabin_sliding_window_size;i++) {
	    block->current_window_data[i]=0;
	}

    return block;
}

/**
 * Reads a block of memory and generates a rabin fingerprint list from it.
 * Since most of the time we will not end on a border, the function returns
 * a block struct, which keeps track of the current blocksum and rolling checksum
 */
 void do_transform(struct rabin_check *transform, uint64_t checksum)
 {
	 ssize_t i;
	 uint64_t tmp;
	 for(i = 0; i < 12; i++)
	 {
		 tmp = checksum * transform_a[i] + transform_b[i];
		 if(tmp > transform->check[i]) transform->check[i] = tmp;
	 }
	 return;
 }
void do_finesse(struct rabin_check *transform, uint64_t checksum, ssize_t now, ssize_t size)
{
	ssize_t i;
	int tmp = now/(size/12);
	if(checksum > transform->check[tmp]) transform->check[tmp] = checksum;
	return;

}
 void do_feature(struct rabin_check transform, ssize_t num)
 {
	 ssize_t i,j;
	 for(i = 0; i < 3; i++)
	 {
		 feature.feature[num][i] = 0;
		 for(j = 0; j < 4; j++)
		 {
			 feature.feature[num][i] += transform.check[i * 4 + j] * rabin_prime_feature[j];
		 }
	 }
	 return;
 };

struct rab_block_info *read_rabin_block(void *buf, ssize_t size, struct rab_block_info *cur_block) {
    struct rab_block_info *block;
    if(cur_block == NULL) {
        block=init_empty_block();
        if(block == NULL)
            return NULL;
    }
    else {
     	block=cur_block;
    }
    //We ended on a border, gen a new tail
    if(block->current_poly_finished) {
        struct rabin_polynomial *new_poly=gen_new_polynomial(NULL,0,0,0);
        block->tail->next_polynomial=new_poly;
        block->tail=new_poly;
        block->current_poly_finished=0;
    }

	struct rabin_check n_transform;

    ssize_t i, j;
	ssize_t num = 0;
	for(j=0;j<12;j++)
	{
		n_transform.check[j] = 0;
	}
    for(i=0;i<size;i++) {
		if(i % 10000000 == 0) printf("i: %d\n", i);
    	char cur_byte=*((char *)(buf+i));
        char pushed_out=block->current_window_data[block->window_pos];
        block->current_window_data[block->window_pos]=cur_byte;
        block->cur_roll_checksum=(block->cur_roll_checksum*rabin_polynomial_prime)+cur_byte;
        block->tail->polynomial=(block->tail->polynomial*rabin_polynomial_prime)+cur_byte;
        block->cur_roll_checksum-=(pushed_out*polynomial_lookup_buf[rabin_sliding_window_size]);
		do_finesse(&n_transform, block->cur_roll_checksum, i, size);
		//do_transform(&n_transform, block->cur_roll_checksum);
        block->window_pos++;
        block->total_bytes_read++;
        block->tail->length++;

        if(block->window_pos == rabin_sliding_window_size) //Loop back around
            block->window_pos=0;
        //printf("!");
        //If we hit our special value or reached the max win size create a new block
        //if((block->tail->length >= rabin_polynomial_min_block_size && (block->cur_roll_checksum % rabin_polynomial_average_block_size) == rabin_polynomial_prime)|| block->tail->length == rabin_polynomial_max_block_size) {
         if(block->tail->length == rabin_polynomial_max_block_size) {
			//printf("?");
			block->tail->start=block->total_bytes_read-block->tail->length;
            struct rabin_polynomial *new_poly=gen_new_polynomial(NULL,0,0,0);
            block->tail->next_polynomial=new_poly;
            block->tail=new_poly;

            do_feature(n_transform, num);
			num++;
			for(j=0;j<12;j++)
			{
				n_transform.check[j] = 0;
			}
            if(i==size-1)
                block->current_poly_finished=1;
        }
    }

    return block;

}

