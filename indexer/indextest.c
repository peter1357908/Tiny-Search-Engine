/*
 * indextest.c - for CS50 lab5, and as part of CS50 TSE project
 *
 * see the markdown files in the same directory for more information.
 *
 * usage:
 *
 * ./indextest oldIndexFilename newIndexFilename
 *
 * exit status (2~4 are from 'index' module):
 * 	
 * 	0 - success
 * 	1 - error during argument parsing (unexpected number of arguments, bad input, etc.)
 * 	2 - error printing something to the index file
 * 	3 - error converting the integers in the counter to string
 * 	4 - error initializing data structures (hashtable or counters)
 *
 * Shengsong Gao, May 2018
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "hashtable.h"
#include "index.h"

// parse the command line, validate parameters, initialize other modules
int main(const int argc, char *argv[]) {
	hashtable_t *index;				// the ultimate index data structure
	char *dir = argv[1];			// the pageDirectory
	FILE *oldIndexFile;				// the OLD index file
	FILE *newIndexFile;				// the NEW index file
	
	if (argc != 3) {
		fprintf(stderr, "usage: ./indextest oldIndexFilename newIndexFilename\n");
		exit(1);
	}
	else if ((oldIndexFile = fopen(argv[1], "r")) == NULL) {
		fprintf(stderr, "unable to open oldIndexFilename '%s' for read failed!\n", argv[1]);
		exit(1);
	}
	else if ((newIndexFile = fopen(argv[2], "w")) == NULL) {
		fprintf(stderr, "writing to newIndexFilename '%s' failed!\n", argv[2]);
		exit(1);
	}

	// make a new hashtable, fill up the hashtable, and print to indexFile.	
	index = indexLoader(oldIndexFile);
	// close oldIndexFile asap
	fclose(oldIndexFile);
	
	indexSaver(index, newIndexFile);
	// close newIndexFile asap
	fclose(newIndexFile);

	// clean up
	indexDeleter(index);
	exit(0);
}
