#!/bin/bash
# expected to be run in the same directory as this script is in
echo -e "now testing 'indexer' by comparing the sorted output with the sorted demo output\n"
echo "('set -x' doesn't print '&&' chaining for the last two commands)"
echo -e "(please read my script for clarification!)\n"

set -x
../indexer ~cs50/demo/crawler-output-small/ output/mine.small
gawk -f ./indexsort.awk ~cs50/demo/index.small > output/demo.small.sorted
gawk -f ./indexsort.awk output/mine.small > output/mine.small.sorted
diff output/mine.small.sorted output/demo.small.sorted && echo "NO DIFFERENCE!"
{ set +x; } 2>/dev/null

echo -e "\n-----------------\n"
echo "now testing 'indextest' by loading from the demo index output 'index.small'."
echo -e "the output is compared to 'demo.small.sorted' (from the previous test).\n"

set -x
../indextest ~cs50/demo/index.small output/indextest
gawk -f ./indexsort.awk output/indextest > output/indextest.sorted
diff output/indextest.sorted output/demo.small.sorted && echo "NO DIFFERENCE AGAIN! HOORAY!"
{ set +x; } 2>/dev/null

echo -e "\n-----------------\n"
