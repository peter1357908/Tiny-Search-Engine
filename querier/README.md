# README.md for CS50 Lab 6 Querier
## Shengsong Gao, May 2018

### `querier` requirements spec

The TSE Querier is a standalone program that reads the index file produced by the TSE Indexer, and page files produced by the TSE Querier, and answers search queries submitted via stdin.

#### The `querier` did:

1. execute from a command line with usage syntax
  * `./querier pageDirectory indexFilename`
  * where `pageDirectory` is the pathname of a directory produced by the Crawler, and
  * where `indexFilename` is the pathname of a file produced by the Indexer.
2. load the index from `indexFilename` into an internal data structure.

3. read search queries from stdin, one per line, until EOF.
  1. clean and parse each query according to the syntax described below.
  2. if the query syntax is somehow invalid, print an error message, do not perform the query, and prompt for the next query.
  3. print the ‘clean’ query for user to see.
  4. use the index to identify the set of documents that satisfy the query, as described below.
  5. if the query is empty (no words), print nothing.
  6. if no documents satisfy the query, print No documents match.
  7. otherwise, rank the resulting set of documents according to its score, as described below, and print the set of documents in decreasing rank order; for each, list the score, document ID and URL. (Obtain the URL by reading the first line of the relevant document file from the pageDirectory.)
  8. Exit with zero status when EOF is reached on stdin.

#### The `querier` validates its command-line arguments:

  * pageDirectory is the pathname for an existing directory produced by the Crawler, and
  * indexFilename is the pathname of a readable file.

### Implementation and Assumptions

please refer to the [IMPLEMENTATION](IMPLEMENTATION.md) in the same directory.

### Compilation

To compile `querier`, simply `make`.

To test `querier`, simply `make test`.

See [TESTING](TESTING.md) for details of testing!
