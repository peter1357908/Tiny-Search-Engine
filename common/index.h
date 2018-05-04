/*
 * index.h - header file for 'index.c', part of CS50 TSE project
 *
 * Shengsong Gao, May 2018
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "counters.h"
#include "hashtable.h"
#include "word.h"
#include "webpage.h"
#include "pagedir.h"

// make the hashtable-counters data structure for index
void indexMaker(hashtable_t *index, char *dir);

// save the index data structure into a file
void indexSaver(hashtable_t *index, FILE *indexFile);

// print the index hashtable. Returns 'false' on error and 'true' otherwise
bool indexPrinter(void *arg, const char *key, void *item);

// print the counters for each word. Returns 'false' on error and 'true' otherwise
bool counterPrinter(void *arg, const int key, int count);

// to pass into hashtable_delete in indexDeleter - just calls counters_delete
void counterDeleter(void *item);

// delete the index by deleting the hashtable and its counters sub-structures
void indexDeleter(void);
