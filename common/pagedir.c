/*
 * pagedir.c - writes html string into a file with the provided webpage info
 *
 * see pagedir.h for more information
 *
 * Shengsong Gao, April 2018.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "webpage.h"
#include "pagedir.h"

// not visible outside of pagedir.c:
bool insertS(char *theS, FILE *file);

bool pagedir(webpage_t *page, char *path, int id) {
	char *pagePath; 			// the local file path of the webpage to be stored
	char idString[12]; 		// stores the string form of the integer id
	char depthString[12];	// stores the string form of the integer depth
	FILE *file;						// the file to write onto
	
	// convert the integers into its string form, return false on failure
	if (sprintf(idString, "%d", id) < 0 || sprintf(depthString, "%d", webpage_getDepth(page)) < 0) {
		return false;
	}
	
	// construct the pagePath
	// length of path plus 13 (11 for idString + 2 for a slash and a null terminator)
	if ((pagePath = calloc(strlen(path) + 13, sizeof(char))) == NULL) {
		return false;
	}
	strcpy(pagePath, path);
	strcat(pagePath, "/");
	strcat(pagePath, idString);
	
	// (create and) open the file
	if ((file = fopen(pagePath, "w")) != NULL) {
		// I can but didn't easily assume no error - the output has to be perfect.
		// first line: URL
		if (insertS(webpage_getURL(page), file) &&
				// second line: depth
				insertS("\n", file) &&
				insertS(depthString, file) &&
				// rest: page content
				insertS("\n", file) &&
				insertS(webpage_getHTML(page), file))
		{
			fclose(file);
			free(pagePath);
			return true;
		}
	}
	free(pagePath);
	return false;	
}

// insert the string into the file: false if something went wrong, true otherwise.
bool insertS(char *theS, FILE *file) {
	if (fputs(theS, file) == EOF) {
		return false;
	}
	return true;
}
