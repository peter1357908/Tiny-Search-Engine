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
void indexMaker(hashtable_t *index, char *dir);

// save the index data structure into a file
void indexSaver(hashtable_t *index, FILE *indexFile);

// delete the index by deleting the hashtable and its counters sub-structures
void indexDeleter(hashtable_t *index);
