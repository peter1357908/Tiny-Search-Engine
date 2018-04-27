/*
 * crawler.c - for CS50 lab4, and as part of CS50 TSE project
 *
 * see the markdown files in the same directory for more information.
 *
 * BEWARE: hashtable size is set to be 30... under the current requirements
 * it's not easy to and I didn't make it adaptive.
 *
 * made my own notNull() function similar to notNull(), but actually frees
 * everything before exiting.
 *
 * usage:
 *
 * 	crawler seedURL pageDirectory maxDepth 
 *
 * exit status:
 * 	
 * 	0 - success
 * 	1 - error during argument parsing (unexpected number of arguments, bad input, etc.)
 * 	2 - error fetching webpage
 *	3 - error writing webpage to file
 *	4 - error normalizing the seedURL
 *	99 - exited from an notNull() statement - see stderr for more information.
 *
 *
 * Shengsong Gao, April 2018
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "bag.h"
#include "hashtable.h"
#include "webpage.h"
#include "memory.h"
#include "pagedir.h"

void cleanup(void); 																// free all allocated memory
void *notNull(void *pointer, const char *message);	// optimized assertp()
void crawler(char *seedURL, char *path, int md); 		// actual crawler

inline static void logr(const char *word, const int depth, const char *url)
{
	printf("%2d %*s%9s: %s\n", depth, depth, "", word, url);
}

bag_t *toVisit;					// stores webpages we have yet to explore
hashtable_t *visited;		// stores URLs we have visited
webpage_t *currpage;		// current page being explored

// parses arguments, initializes other modules, and call crawler()
int main(const int argc, char *argv[]) {
	int md; // maxDepth

	// parse the command line, validate parameters, initialize other modules
	if (argc != 4) {
		fprintf(stderr, "usage: crawler seedURL pageDirectory maxDepth\n");
		exit(1);
	}

	// easiest check first: maxDepth is an int and >= 0
	if (sscanf(argv[3], "%d", &md) == 1) {
		if (md < 0) {
			fprintf(stderr, "maxDepth should be >= 0.\n");
			exit(1);
		}
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

	// test writing to pageDirectory by creating .crawler there
	// (10 = /.crawler(9) + null-terminator(1))
	char *dotCrawlerPath = notNull(calloc(10 + strlen(argv[2]), sizeof(char)), "error making .crawler path\n"); 
	strcpy(dotCrawlerPath, argv[2]);
	strcat(dotCrawlerPath, "/.crawler");
	// create it with fopen(w), and immediately close it
	fclose(notNull(fopen(dotCrawlerPath, "w"), "writing to pageDirectory failed\n"));
	free(dotCrawlerPath);

	// initialize the bag of webpages we have yet to explore
	toVisit = notNull(bag_new(), "failed to initialize bag toVisit\n");

	// initialize the hashtable of URLs we've seen so far - each URL as key, and
	// a dummy character pointer as value (because NULL not allowed as value)
	visited = notNull(hashtable_new(30), "failed to initialize hashtable visited\n");
	
	crawler(argv[1], argv[2], md);

	exit(0);
}

// do the actual crawlin'! 
void crawler(char *seedURL, char *path, int md) {
	int id = 1;						// id of the next webpage to be explored.
	int pos = 0;					// current position in the html buffer
	char *dummy = " ";		// dummy string to be the value of URLs in the hashtable
	char *resultURL; 			// URL from webpage_getNextURL
	webpage_t *URLpage;		// the page created for a URL scanned from currpage
	
	// add it to the bag of webpages to crawl, assuming no error
	bag_insert(toVisit, currpage);

	// mark the normalized URL as visited, assuming no error
	hashtable_insert(visited, seedURL, dummy);
	
	// while there are more webpages to crawl, extract one from the bag.
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
		
		logr("Fetched", webpage_getDepth(currpage), webpage_getURL(currpage));

		// fetch success, write it to the specified path with current id.
		// Increment id and exit on failure
		if (!pagedir(currpage, path, id++)) {
			fprintf(stderr, "error writing html to file\n");
			cleanup();
			exit(3);
		}	

		logr("Saved", webpage_getDepth(currpage), webpage_getURL(currpage));

		// explore the webpage if its depth is < maxDepth
		if (webpage_getDepth(currpage) < md) {
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
	bag_delete(toVisit, webpage_delete);
	// dummy string doesn't need to be freed
	hashtable_delete(visited, NULL);
}

void *notNull(void *pointer, const char *message) {
	if (pointer == NULL) {
		cleanup();
		fprintf(stderr, "NULL detected! %s\n", message);
		exit(99);
	}
	return pointer;
}
