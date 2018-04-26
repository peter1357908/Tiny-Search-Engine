/* 
 * counters.c - CS50 'counters' module
 *
 * see counters.h for more information.
 *
 * Shengsong Gao, April 2018, modified from bag.c of the lab 3 starter kit.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "counters.h"

/**************** file-local global variables ****************/
/* none */

/**************** local types ****************/
typedef struct countersnode {
	int key;					// integer key
  int item;		      // integer item
  struct countersnode *next;	      // link to next node
} countersnode_t;

/**************** global types ****************/
typedef struct counters {
  struct countersnode *head;	      // head of the list of items in counters
} counters_t;

/**************** global functions ****************/
/* that is, visible outside this file */
/* see counters.h for comments about exported functions */

/**************** local functions ****************/
/* not visible outside this file */

/**************** counters_new() ****************/
counters_t *
counters_new(void)
{
  counters_t *counters = malloc(sizeof(counters_t));

  if (counters == NULL) {
    return NULL; // error allocating counters
  } else {
    // initialize contents of counters structure
    counters->head = NULL;
    return counters;
  }
}

/**************** counters_add() ****************/
void
counters_add(counters_t *ctrs, const int key)
{
  if (ctrs != NULL && key >= 0) {
  	// iterate through the list to see if the key already exists, if so, its counter++
		for (countersnode_t *node = ctrs->head; node != NULL; node = node->next) {
			if (node->key == key) {
				node->item++;
				return;
			}
		}
		
		// allocate memory for a new node to be added to the list
  	countersnode_t *node = malloc(sizeof(countersnode_t));

  	if (node != NULL) {
			// the key doesn't exist, and node is allocated - fill it up.
			node->key = key;
  	 	node->item = 1;

  	 	node->next = ctrs->head;
			ctrs->head = node;
		}
  }
}

/**************** counters_set() ****************/
void
counters_set(counters_t *ctrs, const int key, int count)
{
  if (ctrs != NULL && key >= 0 && count >= 0) {
  	// iterate through the list to see if the key already exists, if so, its counter = count
		for (countersnode_t *node = ctrs->head; node != NULL; node = node->next) {
			if (node->key == key) {
				node->item = count;
				return;
			}
		}
		
		// allocate memory for a new node to be added to the list
  	countersnode_t *node = malloc(sizeof(countersnode_t));

  	if (node != NULL) {
			// the key doesn't exist, and node is allocated - fill it up.
			node->key = key;
  	 	node->item = count;

  	 	node->next = ctrs->head;
			ctrs->head = node;
		}
  }
}

/**************** counters_get() ****************/
int
counters_get(counters_t *ctrs, const int key)
{
  if (ctrs == NULL) {
    return 0; // bad counters
  } else if (ctrs->head == NULL) {
    return 0; // counters is empty
  } else {
		// else, iterate through the counters
		for (countersnode_t *node = ctrs->head; node != NULL; node = node->next) {
    	// if key matched, return the item
			if (node->key == key) {
				return node->item;
    	}
		}
	}
	// key never matched, return 0
	return 0;
}

/**************** counters_print() ****************/
void
counters_print(counters_t *ctrs, FILE *fp)
{
  if (fp != NULL) {
    if (ctrs != NULL) {
   	 	fputc('{', fp);
   	  for (countersnode_t *node = ctrs->head; node != NULL; node = node->next) {
				// print this node 
			  fprintf(fp, "(key: %d, counter: %d)", node->key, node->item); 
			  fputc(',', fp);
      }
      fputc('}', fp);
			fputc('\n', fp);
    } else {
      fputs("(null)\n", fp);
    }
  }
}

/**************** counters_iterate() ****************/
void
counters_iterate(counters_t *ctrs, void *arg, void (*itemfunc)(void *arg, const int key, int count) )
{
  if (ctrs != NULL && itemfunc != NULL) {
    // call itemfunc with arg, on each item
    for (countersnode_t *node = ctrs->head; node != NULL; node = node->next) {
      (*itemfunc)(arg, node->key, node->item); 
    }
  }
}

/**************** counters_delete() ****************/
void 
counters_delete(counters_t *ctrs)
{
  if (ctrs != NULL) {
    for (countersnode_t *node = ctrs->head; node != NULL; ) {
      countersnode_t *next = node->next;	    // remember what comes next
      free(node);			    // free the node
      node = next;			    // and move on to next
    }
    free(ctrs);
  }
}
