/*
 * index.h - header file for 'index.c', part of CS50 TSE project
 *
 * exit status:
 * 	2 - error printing something to the index file
 * 	3 - error converting the integers in the counter to string
 *
 * Shengsong Gao, May 2018
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "hashtable.h"

// make the hashtable-counters data structure for index
// dir is the crawler directory - the caller is responsible for validation
hashtable_t *indexMaker(char *dir);

// load index file and return an index data structure
hashtable_t *indexLoader(FILE *old);

// save the index data structure into the specified file
void indexSaver(hashtable_t *index, FILE *indexFile);

// delete the index by deleting the hashtable and its counters sub-structures
void indexDeleter(hashtable_t *index);
