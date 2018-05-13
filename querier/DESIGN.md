# DESIGN.md for CS50 Lab 6 Querier
## Shengsong Gao, May 2018

#### `querier` data flow
From main parsing, to tokenizing the line input, to making the counters scoretable, to finally printing out the result!

#### High-level pseudocode
Very high-level description of the `querier` and `querytest`.

`querier`:

1. parses command-line arguments and maintains command-line user interface
2. keep reading from stdin until EOF or error
3. turn the line read from stdin into an array of tokens - each is a normalize word and extra whitespace are redacted
4. make a `counter` data structure based on the tokenized query, namely the "scoretable"
5. sort the scoretable
6. print it out accordingly!

For detailed pseudocode, please refer to [IMPLEMENTATION](IMPLEMENTATION.md) file in the same directory. 
