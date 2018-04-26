/* 
 * hashtable.c - CS50 'hashtable' module
 *
 * see hashtable.h for more information.
 *
 * Shengsong Gao, April 2018, modified from bag.c of the lab 3 starter kit..
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "hashtable.h"
#include "set.h"
#include "jhash.h"

/**************** file-local global variables ****************/

/**************** local types ****************/

/**************** global types ****************/
typedef struct hashtable {
	int size;						// the size of the hash table
  struct set **sets;	      // a pointer to an array of pointers to a set
} hashtable_t;

/**************** global functions ****************/
/* that is, visible outside this file */
/* see hashtable.h for comments about exported functions */

/**************** local functions ****************/
/* not visible outside this file */

/**************** hashtable_new() ****************/
hashtable_t *
hashtable_new(const int num_slots)
{
	hashtable_t *ht = malloc(sizeof(hashtable_t));

  if (ht == NULL) {
    return NULL; // error allocating hashtable
  } else {
		ht->size = num_slots;
    // initialize the hashtable as a pointer to an array of pointers to a set
    ht->sets = calloc(num_slots, sizeof(struct set*));
		// and initialize each element of the array to be an empty set
		// another way is to initialize as necessary, in the hashtable_insert function
		for (int i = 0; i < num_slots; i++) {
			ht->sets[i] = set_new();
		}
    return ht;
  }
}

/**************** hashtable_insert() ****************/
bool
hashtable_insert(hashtable_t *ht, const char *key, void *item)
{
  if (ht != NULL && key != NULL && item != NULL) {
		// hash the key, insert at the corresponding set in the array, and return status
		// relies on the convinient set_insert function from the 'set' module
		return set_insert(ht->sets[JenkinsHash(key, ht->size)], key, item);
  }
	// some parameter is NULL, return false
	return false;
}

/**************** hashtable_find() ****************/
void *
hashtable_find(hashtable_t *ht, const char *key)
{
	// set_find actually checks if key is NULL, but we still check it here
  if (ht != NULL && key != NULL) {
		return set_find(ht->sets[JenkinsHash(key, ht->size)], key);
	}
	// hashtable or key is NULL
	return NULL;
}

/**************** hashtable_print() ****************/
void
hashtable_print(hashtable_t *ht, FILE *fp, void (*itemprint)(FILE *fp, const char *key, void *item) )
{
  if (fp != NULL) {
    if (ht != NULL) {
      fputc('[', fp);
			// if itemprint is NULL, only prints "[]"
			if (itemprint != NULL) {
    		for (int i = 0; i < ht->size; i++) {
					// ask the function from the 'set' module to print the current set 
	 				set_print(ht->sets[i], fp, itemprint); 
	  			fputc(',', fp);
				}
      }
      fputc(']', fp);
    } else {
      fputs("(null)", fp);
    }
  }
}

/**************** hashtable_iterate() ****************/
void
hashtable_iterate(hashtable_t *ht, void *arg, void (*itemfunc)(void *arg, const char *key, void *item) )
{
	// again, set checks if itemfunc is NULL, but we still do it on this level
  if (ht != NULL && itemfunc != NULL) {
		for (int i = 0; i < ht->size; i++) {
			set_iterate(ht->sets[i], arg, itemfunc);
		}
	}
}

/**************** hashtable_delete() ****************/
void 
hashtable_delete(hashtable_t *ht, void (*itemdelete)(void *item) )
{
  if (ht != NULL) {
    for (int i = 0; i < ht->size; i++) {
			set_delete(ht->sets[i], itemdelete);	    // delete the set's content...
		}
		free(ht->sets);
    free(ht);
  }
}
