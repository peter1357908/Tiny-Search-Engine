/*
 * indexer.c - for CS50 lab5, and as part of CS50 TSE project
 *
 * see the markdown files in the same directory for more information.
 *
 * usage:
 *
 * ./indexer pageDirectory indexFilename
 *
 * exit status:
 * 	
 * 	0 - success
 * 	1 - error during argument parsing (unexpected number of arguments, bad input, etc.)
 * 	2 - error printing something to the index file
 * 	3 - error converting the integers in the counter to string
 * 	4 - error making the index hashtable
 * 	99 - exited from an notNull() statement - see stderr for more information
 *
 *
 * Shengsong Gao, May 2018
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "set.h"
#include "counters.h"
#include "hashtable.h"
#include "word.h"
#include "webpage.h"
#include "pagedir.h"

void cleanup(void); 																				// free all allocated memory
void *notNull(void *pointer, const char *message);					// optimized assertp()
void indexMaker(void); 																			// make the index data structure
void indexPrinter(void *arg, const char *key, void *item);	// print the index hashtable
void counterPrinter(void *arg, const int key, int count);		// print the counters for each word
void counterDeleter(void *item);														// to pass into hashtable_delete

// the variables initialized by main, used by indexMaker, and deleted by cleanup.
hashtable_t *index;				// the ultimate index data structure
char *dir;								// the pageDirectory
FILE *indexFile;					// the index file

// parse the command line, validate parameters, initialize other modules
int main(const int argc, char *argv[]) {
	dir = argv[1];

	if (argc != 3) {
		fprintf(stderr, "usage: ./indexer pageDirectory indexFilename\n");
		exit(1);
	}
	else if (!isCrawlerDirectory(dir)) {
		fprintf(stderr, "pageDirectory '%s' isn't a crawler-produced directory!\n", dir);
		exit(1);
	}
	else if ((indexFile = fopen(argv[2], "w")) == NULL) {
		fprintf(stderr, "writing to indexFile '%s' failed!\n", argv[2]);
		exit(1);
	}

	// make a new hashtable, fill up the hashtable, and print to indexFile.
	index = notNull(hashtable_new(900), "failed when making the index hashtable.\n");
	indexMaker();
	hashtable_iterate(index, NULL, indexPrinter);
	
	fclose(indexFile);
	cleanup();
	exit(0);
}

// make the index data structure
void indexMaker(void) {
	int id = 1;								// the id of the current webpage file
	int pos = 0;							// stores the current position into words in currPage
	char *currWord;						// stores the current word in currPage
	webpage_t *currPage;			// the current webpage
	
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
			hashtable_insert(index, currWord, notNull(counters_new(), "failed when making a 'counters'.\n"));
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


// print the index hashtable
void indexPrinter(void *arg, const char *key, void *item) {
	counters_t *counter = item;
	// print the word
	if (fputs(key, indexFile) == EOF) {
		fprintf(stderr, "failed when printing the string '%s' to index file.\n", key);
		exit(2);
	}
	// print the counters for that word
	counters_iterate(counter, NULL, counterPrinter);
	// go to the next line
	if (fputc('\n', indexFile) == EOF) {
		fprintf(stderr, "failed to print a newline character to index file.\n");
		exit(2);
	}
}

// print the counters for each word
void counterPrinter(void *arg, const int key, int count) {
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

// free the currpage, the bag, and the hashtable
void cleanup(void) {
	hashtable_delete(index, counterDeleter);
}

void *notNull(void *pointer, const char *message) {
	if (pointer == NULL) {
		cleanup();
		fprintf(stderr, "NULL detected: %s\n", message);
		exit(99);
	}
	return pointer;
}
