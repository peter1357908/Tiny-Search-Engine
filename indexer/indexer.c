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
 * 	
 * 	99 - exited from an notNull() statement - see stderr for more information.
 *
 *
 * Shengsong Gao, May 2018
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "set.h"
#include "counter.h"
#include "hashtable.h"
#include "word.h"
#include "webpage.h"
#include "pagedir.h"

void cleanup(void); 																// free all allocated memory
void *notNull(void *pointer, const char *message);	// optimized assertp()
void indexer(void); 																// make the index data structure
void indexSaver(void);															// save the index to a file

// the variables initialized by main, used by indexer, and deleted by cleanup.
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

	index = hashtable_new(900);
	indexer();
	indexSaver();
	
	fclose(indexFile);
	cleanup();
	exit(0);
}

// make the index data structure
void indexer(void) {
	int id = 1;								// the id of the current webpage file
	int pos = 0;							// stores the current position into words in currPage
	char *currWord;						// stores the current word in currPage
	webpage_t *currPage;			// the current webpage
	counters_t *currCounter; 	// stores the current counter

	
	// for each file in the pageDirectory, load into 'currPage'
	while ((currPage = loadPage(dir, id++)) != NULL) {
		// for each word in 'currPage'...
		while ((pos = webpage_getNextWord(currPage, pos, &currWord)) > 0) {
			// insert the word with a new counter, if it exists, nothing happens
			hashtable_insert(index, currWord, counters_new());
			// increment the count
			counters_add(hashtable_find(currWord), id);
			// free the allocated space for the current string
			free(currWord);
		}
		// reset position tracker
		pos = 0;
	}


	webpage_delete(currPage);
}

// free the currpage, the bag, and the hashtable
void cleanup(void) {
	hashtable_delete(index, counter_delete);
}

void *notNull(void *pointer, const char *message) {
	if (pointer == NULL) {
		cleanup();
		fprintf(stderr, "NULL detected: %s\n", message);
		exit(99);
	}
	return pointer;
}
