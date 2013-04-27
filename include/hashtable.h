/*
 * libds hash table header
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

#ifndef __hashtable_h__
#define __hashtable_h__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

typedef struct hashnode_s {
	void *key;
	void *data;
	struct hashnode_s *next;
} hashnode_t;

typedef struct hashtable_s {
	size_t size;
	hashnode_t **nodes;
	size_t (*hashfunc)(const void *);
	int (*keycmpfunc)(const void *, const void *);
} hashtable_t;

hashtable_t *hashtable_create(size_t size, \
	size_t (*hashfunc)(const void *), \
	int (*keycmpfunc)(const void *, const void *));
void hashtable_destroy(hashtable_t *hashtable);
int hashtable_insert(hashtable_t *hashtable, void *key, void *data);
int hashtable_remove(hashtable_t *hashtable, const void *key);
void *hashtable_get(hashtable_t *hashtable, const void *key);

#ifdef __cplusplus
}
#endif

#endif
