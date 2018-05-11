# DESIGN.md for CS50 Lab 5 Indexer
## Shengsong Gao, May 2018

### Purpose of `indexer`
The job of the `indexer` is to read the documents in the `pageDirectory` output by the `crawler`, build an inverted index mapping from words to documents, and write that index to a file. (Later, the `querier` will read the index and use it to answer queries.)

### `indexer` design
#### Data structure: Inverted index
We need a data structure to represent the inverted index. It’s called an inverted index because it maps from words to documents, the opposite of a document (itself a sequence of words).
A hashtable of countersets is perfect to map from a word to a list of (docID, count) pairs.

#### `indexer` data flow

We get to use three out of our four data structures: hashtable, set, and counters!

When processing a given document (identified by docID), consider each word; look up that word in the hashtable and retrieve a pointer to a counters_t* item; then use counters_add() on that item, using the docID as a key.

#### High-level pseudocode
Very high-level description of the `indexer` and `indextest`.

`indexer`:

1. Process and validate command-line parameters
2. Initialize data structure index
3. index_build(directory, index)
4. index_save(file, index);
5. clean up data structures

`indextest`:

1. Process and validate command-line parameters
2. Initialize data structure index
3. index_load(file1, index)
4. index_save(file2, index)
5. clean up data structures

For detailed pseudocode, please refer to [IMPLEMENTATION](IMPLEMENTATION.md) file in the same directory. 
