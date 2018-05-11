/* 
 * querier.c - for CS50 lab6, and as part of CS50 TSE project
 *
 * Shengsong Gao, May 2018
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "counters.h"
#include "hashtable.h"
#include "word.h"
#include "pagedir.h"
#include "file.h"

// function declarations
char **tokenize(char *str);



/*
 * build an array of pointers to the words in the string.
 * the caller is responsible for freeing the returned array
 * assume the input string is not EOF and mutable (to null-terminate words in place)
 * side effect for the input string:
 * 	1. the first whitespace after each word is replaced with '\0'
 * 	2. each word is normalized
 * fprintf to stderr and return NULL if 
 * 	1. any char in the string is neither an alpha or a whitespace
 * 	2. no alphabetical char is in the str
 * 	3. calloc() error
 */
char **tokenize(char *str) {
	int numWords = 0;							// the number of words in the string
	int numChars = strlen(str);		// the number of characters in the string
	char currChar;								// the current character
	char *currWordP;							// the pointer to the current word
	char **array;									// the array to be returned
	bool wasAlpha = false;  			// is the last char examined alphabetical?
	bool wasSpace = true; 				// is the last char examined a whitespace?

	/* 
	 * get the number of words in the string (to calloc() the array)
	 * another approach is to reallocate space for array as necessary, like in readlinep()
	 * null-terminate each word, and return NULL upon encountering invalid chars
	 * make use of the null-terminator from the string, thus "i <= numChars"
	 */
	for (int i = 0; i <= numChars; i++) {
		currChar = str[i];
		if (isalpha(currChar)) {
			wasAlpha = true;
		}
		else if (isspace(currChar)) {
			if (wasAlpha) {
				numWords++;
				str[i] = '\0';
			}
			wasAlpha = false;
		}
		else {
			fprintf(stderr, "Error: bad character '%c' in query.\n", currChar);
			return NULL;
		}
	}
	
	// return NULL for trivial string (empty or all-whitespace)
	if (numWords = 0) {
		fprintf(stderr, "Error: trivial query - empty or all-whitespace.\n");
		return NULL;
	}
	
	// allocate space for the array and return NULL upon calloc failure
	if ((array = (char **)calloc(numWords, sizeof(char *))) == NULL) {
		fprintf(stderr, "Error: calloc() failed.\n");
		return NULL;
	}

	// reuse numWords as a counter for how many words are already stored in the array
	numWords = 0;

	// normalize each word and add its pointer to the array
	for (i = 0; i < numChars; i++) {
		if (isalpha(str[i])) {
			if (wasSpace) {
				currWordP = &(str[i]);
				NormalizeWord(currWordP);
				array[numWords] = currWordP;
				numWords++;'
			}
			wasSpace = false;
		}
		// no need to check for invalid chars anymore - previous loop did the check.
		else {
			wasSpace = true;
		}
	}

	return array;
}

