/*
 * hashtable.c - CS50 'hashtable' module
 *
 * see hashtable.h or more information.
 *
 * Julian Grunauer, January 26, 2019
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "hashtable.h"
#include "set.h"
#include "memory.h"
#include "jhash.h"
#include "jhash.c"

/**************** global types ****************/
typedef struct hash {
    unsigned long num_slots;
    set_t **arr;
} hashtable_t;

/**************** hashtable functions ****************/

/**************** hashtable_new() ****************/
hashtable_t *hashtable_new(const int num_slots)
{
    hashtable_t *hash = malloc(sizeof(hashtable_t)); //allocates memory for hash structure
    hash->arr = calloc(num_slots, sizeof(set_t*)); //allocates memory for array in hash
    hash->num_slots = num_slots; //size of hash array
    if (hash == NULL) {
        return NULL; // error allocating memory to hash
    } else {
        // initialize contents of set structure
        for (int i = 0; i < num_slots; i++) {
            hash->arr[i] = set_new();
        }
        return hash;
    }
    return NULL;
}

/**************** hashtable_insert() ****************/
_Bool hashtable_insert(hashtable_t *ht, const char *key, void *item)
{
    if (ht != NULL && item != NULL && key != NULL) {
        unsigned long place = JenkinsHash(key, ht->num_slots); //send key to hash function to find correct placement
        if (set_insert(ht->arr[place], key, item)) {
            return true;
        } else {
            return false;
        }
    }
    return false;
}

/**************** hashtable_find() ****************/
void *hashtable_find(hashtable_t *ht, const char *key)
{
    if (ht == NULL || key == NULL) {
        return NULL;
    } else {
        int i = 0;
        while (i < ht->num_slots) {
            void* tmp = set_find(ht->arr[i], key); //use set_find at each arr[i]
            if (tmp != NULL) { //found key at arr[i]
                return tmp;
            }
            i++; //did not find key at arr[i], search at arr[i+1]
        }
        return NULL;
    }
    return NULL; //did not find key or error
}

/**************** hashtable_print() ****************/
void hashtable_print(hashtable_t *ht, FILE *fp, void (*itemprint)(FILE *fp, const char *key, void *item))
{
    if (fp != NULL) {
        if (ht != NULL) {
            fputc('{', fp);
            if (itemprint != NULL) {
                for (int i = 0; i < ht->num_slots; i++) {
                    set_t *set = ht->arr[i];
                    set_print(set, stdout, itemprint); //use set_print to print arr[i]
                }
            }
            fputs(" }\n", fp);
        } else {
            fputs("(null)", fp);
        }
    }
}

/**************** hashtable_iterate() ****************/
void hashtable_iterate(hashtable_t *ht, void *arg, void (*itemfunc)(void *arg, const char *key, void *item))
{
    if (ht != NULL && itemfunc != NULL) {
        for (int i = 0; i < ht->num_slots; i++) {
            set_iterate(ht->arr[i], arg, itemfunc); //use set_iterate for arr[i]
        }
    }
}

/**************** hashtable_delete() ****************/
void hashtable_delete(hashtable_t *ht, void (*itemdelete)(void *item))
{
    if (itemdelete == NULL) {
        printf("delete function is NULL. May cause problems");
    }
    if (ht != NULL) {
        for (int i = 0; i < ht->num_slots; i++) {
            set_delete(ht->arr[i], itemdelete); //use set_delete at arr[i]
        }
    }
    free(ht->arr);
    free(ht);
}



