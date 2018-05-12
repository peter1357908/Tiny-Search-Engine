/* 
 * querier.c - for CS50 lab6, and as part of CS50 TSE project
 *
 * usage:
 * 	./querier pageDirectory indexFilename
 *
 * exit code:
 * 	0 - success (exited with EOF)
 * 	1 - memory allocation error (e.g. counters_new() failure)
 * 	2 - invalid command-line argument
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
counters_t *queryToCounters(char **query);
int wordType(char *word);
void counters_union(void *arg, const int key, int count);
void counters_saveIntersection(void *arg, const int key, int count);
void counters_intersection(counters_t **resP, counters_t *ref);
void printResult(counters_t *result, char *dir);
void counters_put(void *arg, const int key, int count);
void counters_numKey(void *arg, const int key, int count);
int compare_keyCount_t(const void *a, const void *b);

// special type used for intersecting two counters.
typedef struct savePlusRef {
	counters_t *save;
	counters_t *res;
} twoCounters;
// special type used for storing key-value pairs in the sorted array
typedef struct keyCount {
	int key;
	int count;
} keyCount_t;

// parse command-line arguments and maintains command-line user interface
int main(const int argc, char *argv[]) {
	hashtable_t *index;				// the index, to be built with indexLoader()
	char *dir = argv[1];			// pageDirectory
	char *line;								// the current line fed from stdin
	char **query;							// the query, tokenized from 'line'
	counters_t *scoreTable;		// the score table produced by queryToCounters()
	
	if (argc != 3) {
		fprintf(stderr, "usage: ./querier pageDirectory indexFilename\n");
		exit(2);
	}
	
	if (!isCrawlerDirectory(dir)) {
		fprintf(stderr, "pageDirectory '%s' isn't a crawler-produced directory!\n", dir);
		exit(2);
	}

	if ((indexFile = fopen(argv[2], "r")) == NULL) {
		fprintf(stderr, "unable to open indexFile '%s' for read!\n", argv[2]);
		exit(2);
	}

	index = indexLoader(indexFile);
	fclose(indexFile);
	
	// keep reading from the file, until EOF or error (based on readlinep())
	while((line = readlinep(stdin)) != NULL) {
		// tokenize returns NULL on recoverable errors, so 'continue'
		if ((query = tokenize(line)) == NULL) {
			continue;
		}
		// queryToCounters returns NULL on recoverable errors, so 'continue'
		if ((scoreTable = queryToCounters(index, query, line)) == NULL) {
			continue;
		}
		printResult(scoreTable, dir);
	}
}

/*
 * build an array of pointers to the words in the string.
 * the caller is responsible for freeing the returned array
 * assume the input string is not EOF and mutable (to null-terminate words in place)
 * side effect for the input string:
 * 	1. the first whitespace after each word is replaced with '\0'
 * 	2. each word is normalized 
 * fprintf to stderr and return NULL if :
 * 	1. any char in the string is neither an alpha or a whitespace
 * 	2. no alphabetical char is in the str
 */
char **tokenize(char *str) {
	int numWords = 0;							// the number of words in the string
	int numChars = strlen(str);		// the number of characters in the string
	char currChar;								// the current character
	char *currWord;								// the pointer to the current word
	char **array;									// the array to be returned
	bool afterAlpha = false;  		// is the last char examined alphabetical?
	bool afterSpace = true; 			// is the last char examined a whitespace?

	/* 
	 * get the number of words in the string (to calloc() the array)
	 * another approach is to reallocate space for array as necessary, like in readlinep()
	 * null-terminate each word, and return NULL upon encountering invalid chars
	 * make use of the null-terminator from the string, thus "i <= numChars"
	 */
	for (int i = 0; i <= numChars; i++) {
		currChar = str[i];
		if (isalpha(currChar)) {
			afterAlpha = true;
		}
		else if (isspace(currChar)) {
			// if a whitespace is preceeded by an alpha, that alpha is the end of a word
			if (afterAlpha) {
				numWords++;
				str[i] = '\0';
			}
			afterAlpha = false;
		}
		else {
			free(str);
			fprintf(stderr, "Error: bad character '%c' in query.\n", currChar);
			return NULL;
		}
	}
	
	// return NULL for trivial string (empty or all-whitespace)
	if (numWords = 0) {
		free(str);
		fprintf(stderr, "Error: trivial query - empty or all-whitespace.\n");
		return NULL;
	}
	
	// allocate space for the array and return NULL upon calloc failure
	if ((array = (char **)calloc(numWords, sizeof(char *))) == NULL) {
		free(str);
		fprintf(stderr, "Error: calloc() failed.\n");
		exit(1);
	}

	// reuse numWords as a counter for how many words are already stored in the array
	numWords = 0;

	// normalize each word and add its pointer to the array
	for (i = 0; i < numChars; i++) {
		if (isalpha(str[i])) {
			// if an alpha is preceeded by a whitespace, that alpha is the start of a word
			if (afterSpace) {
				currWord = &(str[i]);
				NormalizeWord(currWord);
				array[numWords] = currWord;
				numWords++;'
			}
			afterSpace = false;
		}
		// no need to check for invalid chars anymore - previous loop did the check.
		else {
			afterSpace = true;
		}
	}
	
	return array;
}


/*
 * produce a 'counters' based on a processed query (from tokenize())
 * the caller is responsible for freeing the returned counters.
 * accepts the original query string as parameter just so it's easily freed
 * frees both 'query' and 'str' upon return
 * fprintf to stderr and return NULL if:
 * 	1. syntax is invalid (consecutive operators, beginning / ending with opeartors)
 * 	2. the query matches no documents
 */
counters_t *queryToCounters(hashtable_t *index, char **query, char *str) {
	char *currWord = NULL;						// the pointer to the current word and last word
	int currType;											// the type of the current token (see wordType())
	bool afterOperator = false;				// is the last token an operator
	bool firstTime;										// is this the first time into an and-sequence
	counters_t *resultCounters = NULL;// result counters, modified along parsing
	counters_t *andCounters = NULL; 	// temporary counters for the current and-sequence
	counters_t *currCounters = NULL;	// counters in the index for the current word

	
	// get the number of words in the query
	// another approach is sharing numWords with tokenize();
	int numWords = sizeof(query) / sizeof(char *);

	// validate the input before actually querying
	
	// first word cannot be an operator
	currWord = query[0];
	if (wordType(currWord) != 0) {
		fprintf(stderr, "Error: '%d' cannot be first\n", currWord);
		goto syntaxError;
	}

	// last word cannot be an operator
	currWord = query[numWords - 1];
	if (wordType(currWord) != 0) {
		fprintf(stderr, "Error: '%d' cannot be last\n", currWord);
		goto syntaxError;
	}

	// cannot have consecutive operators
	for (int i = 1; i < numWords - 1; i++) {
		currWord = query[i];
		if (wordType(currWord) != 0) {
			if (afterOperator) {
				fprintf(stderr, "Error: '%d' and '%d' cannot be adjacent\n", query[i-1], currWord);
				goto syntaxError;
			}
			afterOperator = true;
		}
		else {
			afterOperator = false;
		}
	}

	// parse and query! Go through all tokens...
	for (i = 0; i < numWords; i++) {
		// initialize a counters for a new and-sequence of words
		if ((andCounters = counters_new()) == NULL) {
			fprintf(stderr, "Error: counters_new() failed.\n");
			free(str);
			free(query);
			indexDeleter(index);
			counters_delete(resultCounters);
			exit(1);
		}
		firstTime = true;

		// keep building this and-sequence until "or" or end of query
		while (i < numWords) {
			// get the current token
			currWord = query[i];
			// break the loop when encountering an "or" operator
			if ((currType = wordType(currWord)) == 2) break;
			// continue if and-sequence is NULL (impossible to have intersection)
			// continue also when encountering "and" operators (meaningless token)
			if (currType == 1 || andCounters == NULL) continue;
			if ((currCounters = hashtable_find(index, currWord)) != NULL) {
				// if it's the first time into an and-sequence
				// copy the content of currCounters into andCounters 
				if (firstTime) {
					counters_iterate(currCounters, andCounters, counters_union);
					firstTime = false;
				}
				else counters_intersection(&andCounters, currCounters);
			}
			// the word doesn't exist in the index, so set andCounters to NULL
			// to signal that further intersection is impossible
			else {
				counters_delete(andCounters);
				andCounters = NULL;
			}
			// move on to the next token
			i++;
		}

		// continue if and-sequence is NULL (no need to union with empty set)
		if (andCounters == NULL) continue;
		// else, do a union between the non-empty and-sequence and the result
		// if it's the first and-sequence, just assign the pointer to resultCounters
		if (resultCounters == NULL) {
			resultCounters = andCounters;
			// continue without deleting the andCounters, which became resultCounters
			continue;
		}
		counters_iterate(currCounters, andCounters, counters_union);
		counters_delete(andCounters);
	}

	// success
	free(str);
	free(query);
	return resultCounters;

syntaxError:
	free(str);
	free(query);
	return NULL;
}

/* 
 * helper function to identify operators
 * return based on the input word
 * 	1 for "and"
 * 	2 for "or"
 * 	0 for anything else
 */
int wordType(char *word) {
	if (strcmp(word, "and") == 0) return 1;
	if (strcmp(word, "or") == 0) return 2;
	return 0;
}

/*
 * union operation. 
 * Take in the result counters; iterate through the reference counters
 * modify 'res' to be the union between 'res' and 'ref'
 * (including score modification)
 */
void counters_union(void *arg, const int key, int count) {
	counters_t *res = arg; 			// the result (counters to be modified)
	int resCount;

	// get the count of current key in res (could be 0), and add the current count to it
	resCount = counters_get(res, key);
	counters_set(res, key, resCount + count);
}

/*
 * intersection operation.
 * Take in the result counters plus the 'save' counters;
 * iterate through the reference counters.
 * modify 'save' to be the intersection between 'res' and 'ref'
 * (including score modification)
 */
void counters_saveIntersection(void *arg, const int key, int count) {
	twoCounters *tc = arg;
	int resCount;

	// if the current key exists in result counters, too, add one with the lower score to 'save'
	if ((resCount = counters_get(tc->res, key)) != 0) {
		if (count > resCount) count = resCount;
		counters_set(tc->save, key, count);
	}
}

/*
 * intersection operation - initialized stuff, and get help from
 * counters_saveIntersection();
 * in the end, *resP contains intersection, with score modification
 * CANNOT MODIFY RES IN PLACE BECAUSE WE DON'T HAVE A counters_deleteKey()...
 */
void counters_intersection(counters_t **resP, counters_t *ref) {
	// assuming memory allocations don't fail, as of now
	twoCounters *tc = malloc(sizeof(twoCounters));
	tc->save = counters_new();
	tc->res = *resP;
	
	counters_iterate(ref, tc, counters_saveIntersection);
	// replace the older result with the result of intersection
	counters_delete(*resP);
	*resP = tc->save;
	free(tc);
}

/*
 * takes in the score table produced by queryToCounters and the pageDirectory path
 * sort the scores (with insertion sort, during iteration) and print accordingly
 * deletes the input counter in the end
 */
void printResult(counters_t *result, char *dir) {
	int numKey = 0;					// number of document matches
	int currKey;						// integer value of the current doc id
	char idString[12];			// string form of the current doc id
	char *pagePath, *url;		// the path to the current file, and the url inside
	keyCount_t *currKc;			// current key-counter pair
	keyCount_t **sorted;		// the sorted array of keyCount_t's
	FILE *file;							// the file of the current doc id

	counters_iterate(result, &numKey, counters_numKey);

	keyCount_t **iterator = (keyCount_t **)(calloc(numKey, sizeof(keyCount_t *)));
	sorted = iterator;
	
	counters_iterate(result, iterator, counters_put);

	qsort(sorted, numKey, sizeof(keyCount_t *), compare_keyCount_t);
	
	printf("Matches %d document(s) (ranked):\n", numKey);
	for (i = 0; i < numKey; i++) {
		currKc = sorted[i];

		// get the current key in its string form and get its file path name with catPath() 
		currKey = currKc->key;
		sprintf(idString, "%d", currKey);
		pagePath = catPath(dir, idString);

		if ((file = fopen(pagePath, "r")) != NULL) {
			url = readlinep(file);
			printf("score %4d doc %4s: %s\n", currKc->count, idString, url);
			free(url);
		}
		else {
			printf("score %4d doc %4s: <no url; failed to read file>\n", currKc->count, idString);
		}
		
		free(currKc);
		free(pagePath);
	}
	
	// clean up.
	free(sorted);
	counters_delete(result);
}

// make a keyCount_t for each key-count pair and append to the array input
void counters_put(void *arg, const int key, int count) {
	keyCount_t **array = arg;
	keyCount_t *currKc = malloc(sizeof(keyCount_t));
	currKc->key = key;
	currKc->count = count;

	array[0] = currKc;
	// so the next time this array is used, it's pointing at the next index
	// another approach is to make ANOTHER data structure that also holds the current index...
	array = &sorted[1];
}

// counts the number of keys (iterations) for the score table
void counters_numKey(void *arg, const int key, int count) {
	int *numKey = arg;

	(*numKey)++;
}

// comparison function for qsort(), to compare keyCount_t's according to count
int compare_keyCount_t(const void *a, const void *b) {
	const keyCount_t *kc1 = a;
	const keyCount_t *kc2 = b;

	return kc1->count - kc2->count;
}

