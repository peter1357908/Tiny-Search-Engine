/*
 * index.c - for CS50 lab5, and as part of CS50 TSE project
 *
 * see index.h for more information
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
void indexMaker(hashtable_t *index, char *dir) {
	int id = 1;// the id of the current webpage file
	int pos = 0;// stores the current position into words in currPage
	char *currWord;// stores the current word in currPage
	webpage_t *currPage;// the current webpage
	
	// for each file in the pageDirectory, load into 'currPage'
	while ((currPage = loadPage(dir, id)) != NULL) {
		// for each word in 'currPage'...
		while ((pos = webpage_getNextWord(currPage, pos, &currWord)) > 0) {
			// ignore words with fewer than 3 characters, and normalize the word
			if (strlen(currWord) < 3) {
				continue;
			}
			NormalizeWord(currWord);
			// insert the word with a new counter - if it already exists, nothing happens
			// ignore any potential errors
			hashtable_insert(index, currWord, counters_new());
			// increment the count
			counters_add(hashtable_find(index, currWord), id);
			// free the allocated space for the current string
			free(currWord);
		}
		// free currPage, reset position tracker, and increment id
		webpage_delete(currPage);
		pos = 0;
		id++;
	}
}

// save the index data structure into a file
void indexSaver(hashtable_t *index, FILE *indexFile) {
	hashtable_iterate(index, indexFile, indexPrinter);
}


// print the index hashtable. Returns 'false' on error and 'true' otherwise
bool indexPrinter(void *arg, const char *key, void *item) {
	FILE *indexFile = arg;
	counters_t *counter = item;
	// print the word
	if (fputs(key, indexFile) == EOF) {
		return false;
	}
	// print the counters for that word
	counters_iterate(counter, NULL, counterPrinter);
	// go to the next line
	if (fputc('\n', indexFile) == EOF) {
		return false;
	}

	return true;
}

// print the counters for each word. Returns 'false' on error and 'true' otherwise
bool counterPrinter(void *arg, const int key, int count) {
	FILE *indexFile = arg;
	char id[12], tally[12]; // stores the string form of the id integer and tally integer
	
	if (sprintf(id, "%d", key) < 0 || sprintf(tally, "%d", count) < 0) {
		return false;
	}
	
	// won't let one error get away
	if (
			fputc(' ', indexFile) == EOF   ||
			fputs(id, indexFile) == EOF    ||
			fputc(' ', indexFile) == EOF   ||
			fputs(tally, indexFile) == EOF
			) {
		return false;
	}

	return true;
}

// to pass into hashtable_delete - just calls counters_delete
void counterDeleter(void *item) {
	counters_t *counter = item;
	counters_delete(counter);
}

void indexDeleter(void) {
	hashtable_delete(index, counterDeleter);
}


