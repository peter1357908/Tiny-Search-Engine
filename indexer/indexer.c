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
 * Shengsong Gao, April 2018
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "counter.h"
#include "hashtable.h"
#include "words.h"
#include "webpage.h"
#include "pagedir.h"

void cleanup(void); 																// free all allocated memory
void *notNull(void *pointer, const char *message);	// optimized assertp()
void indexer(char *seedURL, char *path, int md); 		// actual indexer

hashtable_t *index;				// the ultimate index data structure

// parse the command line, validate parameters, initialize other modules
int main(const int argc, char *argv[]) {
	
	if (argc != 3) {
		fprintf(stderr, "usage: ./indexer pageDirectory indexFilename\n");
		exit(1);
	}

	else {
		fprintf(stderr, "bad maxDepth input. Expected an integer >= 0.\n");
		exit(1);
	}

	// normalize the seedURL first and exit on error
	if (!NormalizeURL(argv[1])) {
		fprintf(stderr, "error normalizing the seedURL\n");
		exit(4);
	}
	
	// try to initialize the normalized seedURL as a webpage_t based on input
	currpage = notNull(webpage_new(argv[1], 0, NULL), "error initializing seedURL as a webpage_t\n");

	// test writing to pageDirectory by creating .indexer there
	// (10 = /.indexer(9) + null-terminator(1))
	char *dotCrawlerPath = notNull(calloc(10 + strlen(argv[2]), sizeof(char)), "error making .indexer path\n"); 
	strcpy(dotCrawlerPath, argv[2]);
	strcat(dotCrawlerPath, "/.indexer");
	// create it with fopen(w), and immediately close it
	fclose(notNull(fopen(dotCrawlerPath, "w"), "writing to pageDirectory failed\n"));
	free(dotCrawlerPath);

	// initialize the bag of webpages we have yet to explore
	toVisit = notNull(bag_new(), "failed to initialize bag toVisit\n");

	// initialize the hashtable of URLs we've seen so far - each URL as key, and
	// a dummy character pointer as value (because NULL not allowed as value)
	visited = notNull(hashtable_new(30), "failed to initialize hashtable visited\n");
	
	indexer(argv[1], argv[2], md);

	exit(0);
}

// do the actual indexin'! 
void indexer(char *seedURL, char *path, int md) {
	int id = 1;						// id of the next webpage to be explored.
	int pos = 0;					// current position in the html buffer
	char *dummy = " ";		// dummy string to be the value of URLs in the hashtable
	char *resultURL; 			// URL from webpage_getNextURL
	webpage_t *URLpage;		// the page created for a URL scanned from currpage
	
	// add it to the bag of webpages to index, assuming no error
	bag_insert(toVisit, currpage);

	// mark the normalized URL as visited, assuming no error
	hashtable_insert(visited, seedURL, dummy);
	
	// while there are more webpages to index, extract one from the bag.
	while((currpage = bag_extract(toVisit)) != NULL) {
		// retrieve the webpage, exit upon failure.
		// webpage_fetch() already implements a 1-sec delay after each attempt
		if (!webpage_fetch(currpage)) {
			fprintf(stderr, "error fetching webpage of URL: %s\n", webpage_getURL(currpage));
			cleanup();
			exit(2);
		}
		
		// if for some reason its html is null, ignore this URL and continue
		if (webpage_getHTML(currpage) == NULL) {
			continue;
		}
		
		// fetch success, write it to the specified path with current id.
		// Increment id and exit on failure
		if (!pagedir(currpage, path, id++)) {
			fprintf(stderr, "error writing html to file\n");
			cleanup();
			exit(3);
		}	

		logr("Fetched", webpage_getDepth(currpage), webpage_getURL(currpage));

		// explore the webpage if its depth is < maxDepth
		if (webpage_getDepth(currpage) < md) {
			logr("Scanning", webpage_getDepth(currpage), webpage_getURL(currpage));
			// get the nextURL if possible
			// currpage's html is compressed as a side effect, but doesn't matter.
			while ((pos = webpage_getNextURL(currpage, pos, &resultURL)) > 0) {
				// only proceed with URL that's valid and internal
				// (IsInternalURL() normalizes the URL as a side effect)
				logr("Found", webpage_getDepth(currpage), resultURL);

				if (IsInternalURL(resultURL)) {
					// if it's never visited before, insert it into the bag
					// (it's marked as visited in the process)
					if (hashtable_insert(visited, resultURL, dummy)) {
						URLpage = webpage_new(resultURL, webpage_getDepth(currpage) + 1, NULL);
						bag_insert(toVisit, URLpage);
						logr("Added", webpage_getDepth(currpage), resultURL);
					}
					else {
						logr("IgnDupl", webpage_getDepth(currpage), resultURL);
					}
				}
				else {
					logr("IgnExtrn", webpage_getDepth(currpage), resultURL);
				}

				// free the memory allocated by webpage_getNextURL()
				free(resultURL);
			}
			// reset pos
			pos = 0;
		}

		// free the current webpage - it's no longer useful
		webpage_delete(currpage);
	}
	
	cleanup();
}

// free the currpage, the bag, and the hashtable
void cleanup(void) {
	webpage_delete(currpage);
	hashtable_delete(visited, NULL);
}

void *notNull(void *pointer, const char *message) {
	if (pointer == NULL) {
		cleanup();
		fprintf(stderr, "NULL detected: %s\n", message);
		exit(99);
	}
	return pointer;
}
