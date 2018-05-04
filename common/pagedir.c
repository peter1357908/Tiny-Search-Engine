/*
 * pagedir.c - writes webpage content into a file (url, depth, html)
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
#include "file.h"

// visible and usable functions outside of pagedir.c
bool pagedir(webpage_t *page, char *dir, int id);
bool isCrawlerDirectory(char *dir);
char *catPath(char *dir, char *file);
webpage_t *loadPage(char *dir, int id);

// writes webpage content to a file with id as name, in dir
bool pagedir(webpage_t *page, char *dir, int id) {
	char *pagePath; 			// the local file path of the webpage to be stored
	char idString[12]; 		// stores the string form of the id integer
	char depthString[12];	// stores the string form of the depth integer
	FILE *file;						// the file to write onto
	
	// convert the integers into its string form, return false on failure
	if (sprintf(idString, "%d", id) < 0 || sprintf(depthString, "%d", webpage_getDepth(page)) < 0) {
		return false;
	}
	
	pagePath = catPath(dir, idString);
	
	// (create and) open the file
	if ((file = fopen(pagePath, "w")) != NULL) {
		// I can but didn't easily assume no error - the output has to be perfect.
		// first line: URL
		if (fputs(webpage_getURL(page), file) != EOF &&
				// second line: depth
				fputs("\n", file) != EOF &&
				fputs(depthString, file) != EOF &&
				// rest: page content
				fputs("\n", file) != EOF &&
				fputs(webpage_getHTML(page), file) != EOF)
		{
			fclose(file);
			free(pagePath);
			return true;
		}
	}
	free(pagePath);
	return false;	
}

// tests if dir is crawler dir by trying to read .crawler file in the root of dir
bool isCrawlerDirectory(char *dir) {
	char *path;     // path for the .crawler file
	FILE *file;			// the .crawler file
	
	path = catPath(dir, ".crawler");

	// tries to open the file
	if ((file = fopen(path, "r")) != NULL) {
		fclose(file);
		free(path);
		return true;
	}
	free(path);
	return false;
}
	
// concatenate file string, file separator "/", and the directory path string
// the caller is responsible for freeing the returned string
char *catPath(char *dir, char *file) {
	char *filePath;
	
	// +1 for null terminator, and another +1 for file separator
	if ((filePath = calloc(strlen(dir) + strlen(file) + 2, sizeof(char))) == NULL) {
		return NULL;
	}
	
	strcpy(filePath, dir);
	strcat(filePath, "/");
	strcat(filePath, file);

	return filePath;
}

/* 
 * loads the content of a saved webpage file into a webpage_t
 * assuming legal file content, "need not have extensive error checking"
 * the caller is responsible for freeing the returned webpage.
 */

webpage_t *loadPage(char *dir, int id) {
	char *pagePath; 					// the local file path of the webpage to be stored
	char idString[12]; 				// stores the string form of the id integer
	char *url, *depth, *html;	// stores the url, depth, and html
	int depthInt;							// stores the integer form of the depth string
	FILE *file;								// the file to read from
	webpage_t *result; 				// the webpage_t to be returned

	// convert the integers into its string form, return NULL on failure
	if (sprintf(idString, "%d", id) < 0) {
		return NULL;
	}
	
	pagePath = catPath(dir, idString);
	
	// (create and) open the file
	if ((file = fopen(pagePath, "r")) != NULL) {
		free(pagePath);

		// first line: URL
		url = readlinep(file);
		free(url);
		
		// second line: depth, also convert to an integer (non-negative)
		depth = readlinep(file);
		sscanf(depth, "%d", &depthInt);
		free(depth);
		
		// rest: page content - html not freed because 'result' still refers to it.
		html = readfilep(file);
		fclose(file);
		result = webpage_new(url, depthInt, html);
		
		return result;
	}
	free(pagePath);
	return NULL;
}
	
