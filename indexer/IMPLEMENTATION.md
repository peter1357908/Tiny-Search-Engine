# IMPLEMENTATION.md for CS50 Lab 5 Indexer
## Shengsong Gao, April 2018

### Pseudo Code (C and Implement-dependent)
#### `indexer`:
1. validate command-line arguments. Exit if:
  1. `argc` isn't 3 or
	2. the directory isn't a crawler directory or
	3. the target `indexFile` cannot be opened for write (opening the file for write in the process)
2. get index from `indexMaker`, a function of the `index` module.
3. save index through `indexSaver`, a function of the `index` module.
4. close the file
5. free all the allocated space

#### `indextest`:
1. validate command-line arguments. Exit if:
  1. `argc` isn't 3 or
	2. `oldIndexFile` cannot be opened for read (opening the file for read in the process)
	3. `newIndexFile` cannot be opened for write (opening the file for write in the process)
2. load index from oldIndexFile through `indexLoader`, a function of the `index` module.
3. close oldIndexFile
4. save index through `indexSaver`, a function of the `index` module.
5. close newIndexFile
6. free all the allocated space

#### `index` module:
`indexMaker`:
1. make a new index hashtable (of size 900, for this lab)
2. for each crawler-produced file in the pageDirectory
  1. load the file back into a webpage_t through `loadPage` function from `pagedir` module
  2. for each word in the webpage_t
    1. skip words with fewer than 3 characters
    2. normalize the word (tolower)
    3. try to get its `counters` from the hashtable, and if failed, make a `counters` for the word
    4. increment the count for the current file for the current word
3. return the index hashtable

`indexLoader`:
1. make a new index hashtable (as big as twice the number of words in the indexFile)
2. for each word read from the file
  1. make a new `counters` (because we expect each word to be unique)
  2. insert the `counters` into the hashtable
  3. for each pair of integers read from the file
    1. set the `counters` structure accordingly - first int is ID, second int is tally.
3. return the index hashtable

`indexSaver`:
1. iterate through the index hashtble with `indexPrinter`

`indexPrinter`:
1. print the word to the file, and iterate through the `counters` structure for this word with `counterPrinter`
2. go to the next line

`counterPrinter`:
1. scan the integer ID and tally into strings
2. print according to the format *docID tally [docID tally]...*

#### `loadPage` function from `pagedir` module:
1. concatenate the directory string with a file separator, and the file name string
2. open the crawler-produced file for read
3. store the first line as url
4. store the second line as depth
5. store the rest of the file as html
6. make a webpage_t with the url, depth, and html above
7. return the webpage_t

### Assumptions

####`index` module
`pageDirectory` has files named 1, 2, 3, …, without gaps.
The content of files in `pageDirectory` follow the format as defined in the specs
The content of the index file follows the following format:
1. one line per word, one word per line
2. each line provides the word and one or more (docID, count) pairs, in the format
  *_word docID count [docID count]…_
    *where word is a string of lower-case letters,
    *where docID is a positive non-zero integer,
    *where count is a positive non-zero integer,
    *where the word and integers are separated by spaces.