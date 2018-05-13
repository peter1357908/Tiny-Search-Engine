#!/bin/bash
# expected to be run in the same directory as this script is in
echo -e "now testing 'querier'\n"
echo "('set -x' doesn't print '&&' chaining for the last two commands)"
echo -e "(please read my script for clarification!)\n"

set -x
../querier ~cs50/data/tse-output/cs50-3 ~cs50/data/tse-output/cs50-index3
{ set +x; } 2>/dev/null

echo "querier test finished!"
echo -e "\n-----------------\n"
