/*
 * libds hash table implementation
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

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include "libds.h"

hashtable_t *hashtable_create(size_t size, \
	size_t (*hashfunc)(const void *), \
	int (*keycmpfunc)(const void *, const void *))
{
	hashtable_t *hashtable;
	
	if (!hashfunc || !keycmpfunc)
		return NULL;
	
	if (!(hashtable = malloc(sizeof(hashtable_t))))
		return NULL;
		
	if (!(hashtable->nodes = calloc(size, sizeof(hashnode_t)))) {
		free(hashtable);
		return NULL;
	}
	
	hashtable->size = size;
	hashtable->hashfunc = hashfunc;
	hashtable->keycmpfunc = keycmpfunc;
	
	return hashtable;
}

void hashtable_destroy(hashtable_t *hashtable)
{
	size_t n;
	hashnode_t *node, *oldNode;
	
	for (n = 0; n < hashtable->size; n++) {
		node = hashtable->nodes[n];
		while (node) {
			oldNode = node;
			node = node->next;
			free(oldNode);
		}
	}
	
	free(hashtable->nodes);
	free(hashtable);
}

int hashtable_insert(hashtable_t *hashtable, void *key, void *data)
{
	hashnode_t *node;
	size_t hash = hashtable->hashfunc(key) % hashtable->size;
	
	node = hashtable->nodes[hash];
	while (node) {
		if (!hashtable->keycmpfunc(node->key, key))
			return -1; // key already exists!
		node = node->next;
	}
	
	if (!(node = malloc(sizeof(hashnode_t))))
		return -1;
		
	node->key = key;
	node->data = data;
	node->next = hashtable->nodes[hash];
	hashtable->nodes[hash] = node;
	
	return 0;
}

int hashtable_remove(hashtable_t *hashtable, const void *key)
{
	hashnode_t *node, *prevNode = NULL;
	size_t hash = hashtable->hashfunc(key) % hashtable->size;
	
	node = hashtable->nodes[hash];
	while (node) {
		if (!hashtable->keycmpfunc(node->key, key)) {
			if (prevNode)
				prevNode->next = node->next;
			else
				hashtable->nodes[hash] = node->next;
			free(node);
			return 0;
		}
		prevNode = node;
		node = node->next;
	}
	
	return -1;	
}

void *hashtable_get(hashtable_t *hashtable, const void *key)
{
	hashnode_t *node;
	size_t hash = hashtable->hashfunc(key) % hashtable->size;
	
	node = hashtable->nodes[hash];
	while (node) {
		if (!hashtable->keycmpfunc(node->key, key))
			return node->data;
		node = node->next;
	}
	
	return NULL;
}

#ifdef __cplusplus
}
#endif
