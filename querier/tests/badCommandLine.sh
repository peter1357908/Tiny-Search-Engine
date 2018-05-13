#!/bin/bash
# expected to be run in the same directory as this script is in
echo -e "\n-----------------\n"
echo -e "now testing bad arguments for 'querier'\n"
set -x
../querier 1
../querier 1 2 3
../querier thisIsNotADirectory
../querier output ~cs50/data/tse-output/cs50-index3
../querier ~cs50/data/tse-output/cs50-3 thisIsNotAFile
{ set +x; } 2>/dev/null
echo -e "\nbad arguments test finished!"
echo -e "\n-----------------\n"
