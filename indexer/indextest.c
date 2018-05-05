/*
 * indextest.c - for CS50 lab5, and as part of CS50 TSE project
 *
 * see the markdown files in the same directory for more information.
 *
 * usage:
 *
 * ./indextest oldIndexFilename newIndexFilename
 *
 * exit status (2 and 3 are from 'index' module):
 * 	
 * 	0 - success
 * 	1 - error during argument parsing (unexpected number of arguments, bad input, etc.)
 *
 * Shengsong Gao, May 2018
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "hashtable.h"
#include "pagedir.h"
#include "webpage.h"
#include "index.h"

// parse the command line, validate parameters, initialize other modules
int main(const int argc, char *argv[]) {
	hashtable_t *index;				// the ultimate index data structure
	char *dir = argv[1];			// the pageDirectory
	FILE *indexFile;					// the index file
	
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
	if ((index = hashtable_new(900)) == NULL) {
		fprintf(stderr, "failed when initializing the index hashtable.\n");
		exit(4);
	}
	
	indexMaker(index, dir);
	indexSaver(index, indexFile);

	// clean up.
	fclose(indexFile);
	indexDeleter(index);
	exit(0);
}
