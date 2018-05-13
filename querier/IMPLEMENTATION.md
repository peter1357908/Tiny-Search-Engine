# IMPLEMENTATION.md for CS50 Lab 6 Querier
## Shengsong Gao, May 2018

### Pseudo Code (C and Implement-dependent)

#### 'querier'

##### main()

1. parse command-line arguments and maintains command-line user interface
2. keep reading from the file, until EOF or error (based on readlinep())
  1. tokenize the query into an array of normalized words including operators
	  2. query - build a `counters` based on the tokenized query
		  3. print the result

##### tokenize()

1. count the number of tokens
  1. if a whitespace is preceded by an alpha, it's a word, increase token count
	  2. replace that whitespace characters with the null character to terminate this token as a string
		2. if no tokens were recorded, return due to trivial query (empty or all-whitespace)
		3. allocate space for the array based on the token count
		4. normalize each word, add its pointer to the array, and print it out

##### queryToCounters()

1. validate the input before actually querying
  1. first word cannot be an operator
  2. last word cannot be an operator
  3. annot have consecutive operators
2. parse and query! Go through all tokens...
  1. initialize a counters for a new and-sequence of words
  2. keep building this and-sequence until "or" or end of query
    1. get the current token
    2. quit this and-sequence when encountering an "or" operator
    3. jump to the next token if and-sequence is NULL (impossible to have intersection)
    4. jump to the next token also when encountering "and" operators (meaningless token)
    5. if the word exists in the index
      1. if it's the first time into an and-sequence, copy the content of currCounters into andCounters
      2. else, do an and-intersection between the currCounters and andCounters
    6. else, the word doesn't exist in the index, so set andCounters to NULL to signal that further intersection is impossible
  3. continue if and-sequence is NULL (no need to union with empty set)
  4. else, do a union between the non-empty and-sequence and the result
    1. if it's the first and-sequence, just assign its pointer to resultCounters
    2. continue without deleting the andCounters, because it's now resultCounters
3. if the resultCounters was never initialized, no word matched anything. Signal and return NULL.
4. else, return resultCounters

##### printResult()

1. first find out how many keys are in the score table
2. if counters contains nothing, then no document was matched
3. then make an array of corresponding size, and copy its pointer to 'sorted'
4. put every key-count pair into the array
5. sort the array of (keyCount_t *)
6. print based on the sorted array.

### inconveniences
Made 3 different special types to combat some inconveniences in the given environment
1. saveRes_t - used to pack two counters `save` and `res` into one "thing"
2. keyCount_t - used to pack two integers `key` and `count` into one "thing"
3. arrayIndex_t - used to pack an array of (keyCount_t *) and an index into one "thing"

### Assumptions

`pageDirectory` has files named 1, 2, 3, …, without gaps.
The content of files in `pageDirectory` follow the format as defined in the specs
The content of the file named by 'indexFilename' follows the following format:
1. one line per word, one word per line
2. each line provides the word and one or more (docID, count) pairs, in the format
  * _word docID count [docID count]…_
    * where word is a string of lower-case letters,
    * where docID is a positive non-zero integer,
    * where count is a positive non-zero integer,
    * where the word and integers are separated by spaces.
