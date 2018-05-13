# TESTING.md for CS50 Lab 6 Querier
## Shengsong Gao, May 2018

### Testing `querier`

Testing has never been made easier! Just do `make test` in the querier
directory! That command will run the two shell scripts, `badCommandLine.sh` and
`querying.sh` - both scripts will test `querier`.

`badCommandLine.sh` tests many different invalid command line arguments on
`querier` - it echoes out each command before their output by
using bash utitlity `set -x`.

`querying.sh` test-runs `querier`. It echoes out commands like
badCommandLine.sh above. Please refer to the script for more info!

Tested with myvalgrind, and there was no memory leak whatsoever! I didn't
include valgrind testing in the test scripts because it's rather slow - but do
try it out if you wish!
