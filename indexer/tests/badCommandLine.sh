#!/bin/bash
# expected to be run in the same directory as this script is in
echo -e "\n-----------------\n"
echo -e "now testing bad arguments for both 'indexer' and 'indextest'\n"
set -x
../indexer 1
../indexer 1 2 3
../indexer notADirectory output/mine.small
../indexer output output/mine.small
../indexer ~cs50/demo/crawler-output-small/ ./output
../indextest 1
../indextest 1 2 3
../indextest notAFile output/mine.small.indextest
../indextest ~cs50/demo/index.small ./output
{ set +x; } 2>/dev/null
echo -e "\nbad arguments test finished!"
echo -e "\n-----------------\n"
