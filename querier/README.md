# README.md for CS50 Lab 5 Indexer
## Shengsong Gao, May 2018

### indexer requirements spec

The job of the `indexer` is to read the documents in the `pageDirectory` output by the `crawler`, build an inverted index mapping from words to documents, and write that index to a file. (Later, the `querier` will read the index and use it to answer queries.)

#### The `indexer` (through `index`) did:

1. execute from a command line with usage syntax
  * `./indexer pageDirectory indexFilename`
  * where `pageDirectory` is the pathname of a directory produced by the Crawler, and
  * where `indexFilename` is the pathname of a file into which the index should be written; the indexer creates the file (if needed) and overwrites the file (if it already exists).
2. read documents from the `pageDirectory`, each of which has a unique document ID, wherein
  * the document `id` starts at 1 and increments by 1 for each new page,
  * and the filename is of form `pageDirectory/id`,
  * and the first line of the file is the URL,
  * and the second line of the file is the depth,
  * and the rest of the file is the page content (the HTML, unchanged).
3. build an inverted-index data structure mapping from _words_ to _(documentID, count)_ pairs, wherein each _count_ represents the number of occurrences of the given word in the given document. Ignore words with fewer than three characters, and “normalize” the word before indexing. (Here, “normalize” means to convert all letters to lower-case.)
4. create a file `indexFilename` and write the index to that file, in the format described below.
5. validate its command-line arguments:
  * pageDirectory is the pathname for an existing directory produced by the crawler, and
  * indexFilename is the pathname of a writeable file; it may or may not already exist.

#### The `indextest` (also through `index`) did:

1. execute from a command line with usage syntax
  * ./indextest oldIndexFilename newIndexFilename
  * where oldIndexFilename is the name of a file produced by the indexer, and
  * where newIndexFilename is the name of a file into which the index should be written.
2. load the index from the `oldIndexFilename` into an inverted-index data structure.
3. create a file `newIndexFilename` and write the index to that file, in the format described below.


### Implementation and Assumptions

please refer to the [IMPLEMENTATION](IMPLEMENTATION.md) in the same directory.

### Compilation

To compile both `indexer` and `indextest`, simply `make`.

To compile each, just `make indexer` or `make indextest`.

To test both `indexer` and `indextest`, simply `make test`.

See [TESTING](TESTING.md) for details of testing!
