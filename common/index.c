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

// functions intended for use inside index.c only
void indexPrinter(void *arg, const char *key, void *item);
void counterPrinter(void *arg, const int key, int count);
void counterDeleter(void *item);


void indexMaker(hashtable_t *index, char *dir) {
	int id = 1;								// the id of the current webpage file
	int pos = 0;							// stores the current position into words in currPage
	char *currWord;						// stores the current word in currPage
	webpage_t *currPage;			// the current webpage
	counters_t *currCounter;	// the current counter
	
	// for each file in the pageDirectory, load into 'currPage'
	while ((currPage = loadPage(dir, id)) != NULL) {
		// for each word in 'currPage'...
		while ((pos = webpage_getNextWord(currPage, pos, &currWord)) > 0) {
			// ignore words with fewer than 3 characters, and normalize the word
			if (strlen(currWord) < 3) {
				free(currWord);
				continue;
			}
			NormalizeWord(currWord);
			// try to insert the word with a new counter
			currCounter = counters_new();
			if (!hashtable_insert(index, currWord, currCounter)) {
				// if failed, the word exists already, free currCounter
				counters_delete(currCounter);
			}

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
void indexPrinter(void *arg, const char *key, void *item) {
	if (arg == NULL) {
		fprintf(stderr, "indexFile arg passed to indexPrinter is NULL!\n");
		exit(2);
	}
	FILE *indexFile = arg;
	counters_t *counter = item;
	// print the word
	if (fputs(key, indexFile) == EOF) {
		fprintf(stderr, "failed when printing the string '%s' to index file.\n", key);
		exit(2);
	}
	// print the counters for that word
	counters_iterate(counter, indexFile, counterPrinter);
	// go to the next line
	if (fputc('\n', indexFile) == EOF) {
		fprintf(stderr, "failed to print a newline character to index file.\n");
		exit(2);
	}
}

// print the counters for each word. Returns 'false' on error and 'true' otherwise
void counterPrinter(void *arg, const int key, int count) {
	if (arg == NULL) {
		fprintf(stderr, "indexFile arg passed to counterPrinter is NULL!\n");
		exit(2);
	}
	FILE *indexFile = arg;
	char id[12], tally[12]; // stores the string form of the id integer and tally integer
	
	if (sprintf(id, "%d", key) < 0 || sprintf(tally, "%d", count) < 0) {
		fprintf(stderr, "failed when parsing the doc id or tally to string.\n");
		exit(3);
	}
	
	// won't let one error get away
	if (
			fputc(' ', indexFile) == EOF   ||
			fputs(id, indexFile) == EOF    ||
			fputc(' ', indexFile) == EOF   ||
			fputs(tally, indexFile) == EOF
			) {
		fprintf(stderr, "failed when writing docID or tally to the indexFile.\n");
		exit(2);
	}
}

// to pass into hashtable_delete - just calls counters_delete
void counterDeleter(void *item) {
	counters_t *counter = item;
	counters_delete(counter);
}

void indexDeleter(hashtable_t *index) {
	hashtable_delete(index, counterDeleter);
}


