# expected to be run in the same directory as this script is in
echo comparing sorted index file outputs
echo "the 'demo' file is sorted from ~cs50/demo/index.small"
echo "the 'mine' file is sorted from the index file by my indexer, based on"
echo "'~cs50/demo/crawler-output-small', which is a crawler directory"
echo "sorted the index files with 'indexsort.awk'; compared with 'diff'"
echo "if no difference, 'NO DIFFERENCE!' is echoed to stdout"
echo
../indexer ~cs50/demo/crawler-output-small/ output/mine.small

gawk -f ./indexsort.awk ~cs50/demo/index.small > output/demo.small.sorted

gawk -f ./indexsort.awk output/mine.small > output/mine.small.sorted

diff output/mine.small.sorted output/demo.small.sorted && echo "NO DIFFERENCE!"

