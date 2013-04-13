/*
 * libds hash table header
 * Copyright (c) 2013 Isaac Waldron
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
