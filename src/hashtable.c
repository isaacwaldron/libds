/*
 * libds hash table implementation
 * Copyright (c) 2013 Isaac Waldron
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
