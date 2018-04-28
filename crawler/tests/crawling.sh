echo comparing stdout output between my crawler and the ~cs50/demo/crawler
echo using diff program. They should be perfectly matching - \"No difference!\"
echo first, ~cs50/index.html maxDepth=2, difference:
../crawler http://old-www.cs.dartmouth.edu/~cs50/index.html ./output 2 >mine
~cs50/demo/crawler http://old-www.cs.dartmouth.edu/~cs50/index.html ./output 2 >demo
diff mine demo && echo "No difference!"
rm -f mine demo ./output/*

echo next, ~cs50/data/tse/letters/ maxDepth=2, difference:
../crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/ ./output 2 >mine
~cs50/demo/crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/ ./output 2 >demo
diff mine demo && echo "No difference!"
rm -f mine demo ./output/*

echo and finally, ~cs50/data/tse/wikipedia/ maxDepth=1, difference:
../crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/ ./output 1 >mine
~cs50/demo/crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/ ./output 1 >demo
diff mine demo && echo "No difference!"
rm -f mine demo ./output/* ./output/.crawler

