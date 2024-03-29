/*
 * libds hash table test
 * 
 * Copyright (c) 2013, Isaac Waldron
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions 
 * are met:
 * 
 * 1. Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright 
 *    notice, this list of conditions and the following disclaimer in 
 *    the documentation and/or other materials provided with the 
 *    distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>

#include "libds.h"

#define TEST_HASHTABLE_SIZE 32768
#define TEST_HASHTABLE_ARRAY_SIZE 1000000

size_t inthash(const void *key)
{
	int i, k;
	
	k = *((int *) key);
	
	size_t hash = 0;
	for (i = 0; i < sizeof(int); i++)
		hash = 31*hash + (&k)[i];
		
	return hash;
}

int intcmp(const void *key1, const void *key2)
{
	return (*((int *) key1) - *((int *) key2));
}

int main(int argc, char *argv[])
{
	long int *numbers;
	int i;
	hashtable_t *table;
	
	numbers = (long int *) \
		calloc(2*TEST_HASHTABLE_ARRAY_SIZE, sizeof(long int));
	if (NULL == numbers) {
		fprintf(stderr, "out of memory!\n");
		return -1;
	}
	
	srandom(time(NULL));
	table = hashtable_create(TEST_HASHTABLE_SIZE, inthash, intcmp);
	fprintf(stdout, "Created hash table of size %d.\n", \
		TEST_HASHTABLE_SIZE);
	
	for (i = 0; i < TEST_HASHTABLE_ARRAY_SIZE; i++) {
		numbers[2*i] = random();
		if (!hashtable_get(table, &numbers[2*i])) {
			numbers[2*i + 1] = random();
			hashtable_insert(table, &numbers[2*i], &numbers[2*i + 1]);
			fprintf(stdout, "%ld -> %ld\n", numbers[2*i], \
				numbers[2*i + 1]);
		}
		else {
			numbers[2*i + 1] = -1;
			fprintf(stderr, "%ld already a key in hash table!\n", \
				numbers[2*i]);
		}
	}
	
	for (i = 0; i < TEST_HASHTABLE_ARRAY_SIZE; i++)
		if (-1 != numbers[2*i + 1])
			if (numbers[2*i + 1] != \
				*((int *) hashtable_get(table, &numbers[2*i])))
				fprintf(stderr, "%ld does not point to %ld!\n", \
					numbers[2*i], numbers[2*i + 1]);
			else
				fprintf(stdout, "Confirmed %ld -> %ld\n", \
					numbers[2*i], numbers[2*i + 1]);

	hashtable_destroy(table);
	free(numbers);
	
	return 0;
}
