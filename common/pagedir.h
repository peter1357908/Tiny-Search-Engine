/*
 * pagedir.h - header file for pagedir.c 
 *
 * pagedir writes html string into a file, with the provided id and path.
 *
 * returns false for any error and true for success
 *
 * Shengsong Gao, April 2018.
 */

#ifndef __PAGEDIR_H
#define __PAGEDIR_H

#include <stdio.h>
#include <stdbool.h>
#include "webpage.h"

bool pagedir(webpage_t *page, char *path, int id);

#endif // __PAGEDIR_H
