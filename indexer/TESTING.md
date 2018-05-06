# TESTING.md for CS50 Lab 5 Indexer
## Shengsong Gao, May 2018

### Testing `indexer` and `indextest`

Testing has never been made easier! Just do `make test` in the indexer
directory! That command will run the two shell scripts, `badCommandLine.sh` and
`indexing.sh` - both scripts will test BOTH `indexer` and `indextest`.

`badCommandLine.sh` tests many different invalid command line arguments on both
`indexer` and `indextest` - it echoes out each command before their output by
using bash utitlity `set -x` - it's imperfect, though, as the `&&` command
chaining is not displayed - please refer to the script for precise commands!
(This problem becomes obvious in indexing.sh) It constructs index based on the
demo crawler output in the cs50 demo directory, saves the index into the output
directory, sorts the output, and compare the output to the sorted demo output.

`indexing.sh` test-runs `indexer`and `indextest`. It echoes out commands like
badCommandLine.sh above. It loads index from the demo index output in the cs50
demo directory, saves the index into the output directory, sorts the output,
and compare the output to the sorted demo output.

Tested with myvalgrind, and there was no memory leak whatsoever! I didn't
include valgrind testing in the test scripts because it's rather slow - but do
try it out if you wish!
