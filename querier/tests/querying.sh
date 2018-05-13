#!/bin/bash
# expected to be run in the same directory as this script is in
echo -e "now testing 'querier'\n"
echo "querier is run twice with the same input from file 'querierInput'"
echo "each output is stored into a file and then compared with each other using 'diff'"
echo -e "if querier performed consistently, no difference would be printed out!\n"

set -x
../querier ~cs50/data/tse-output/cs50-3 ~cs50/data/tse-output/cs50-index3 <querierInput >output/output1
../querier ~cs50/data/tse-output/cs50-3 ~cs50/data/tse-output/cs50-index3 <querierInput >output/output2
diff output/output1 output/output2
{ set +x; } 2>/dev/null

echo -e "\nquerier test finished!"
echo -e "\n-----------------\n"
