/*
 * pagedir.h - header file for pagedir.c 
 *
 * Shengsong Gao, April 2018.
 */

#ifndef __PAGEDIR_H
#define __PAGEDIR_H

#include <stdio.h>
#include <stdbool.h>
#include "webpage.h"

// writes webpage content (url, depth, html) to "dir/id"
bool pagedir(webpage_t *page, char *dir, int id);

// tests if dir is crawler dir by trying to read .crawler file in the root of dir
bool isCrawlerDirectory(char *dir);
	
// concatenate file string, file separator "/", and the directory path string
// the caller is responsible for freeing the returned string
// returns NULL on calloc() error
char *catPath(char *dir, char *file);

// loads the content of a saved webpage file into a webpage_t
// assuming legal file content, "need not have extensive error checking"
webpage_t *loadPage(char *dir, int id); 


#endif // __PAGEDIR_H
