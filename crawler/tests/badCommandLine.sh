echo bad arguments

echo $ ../crawler 1 2 3
../crawler 1 2 3

echo $ ../crawler http://whattheheckisthis1234567890.com . 2
../crawler http://whattheheckisthis1234567890.com . 2

echo $ ../crawler http://old-www.cs.dartmouth.edu/~cs50/index.htmlllllllllllllll . 2
../crawler http://old-www.cs.dartmouth.edu/~cs50/index.htmlllllllllllllll . 2

echo $ ../crawler http://old-www.cs.dartmouth.edu/~cs50/index.html thiedirectorydoesntexist 2
../crawler http://old-www.cs.dartmouth.edu/~cs50/index.html thiedirectorydoesntexist 2

echo $ ../crawler http://old-www.cs.dartmouth.edu/~cs50/index.html . -10
../crawler http://old-www.cs.dartmouth.edu/~cs50/index.html . -10

echo insufficient arguments

echo $ ../crawler 
../crawler 

echo $ ../crawler http://old-www.cs.dartmouth.edu/~cs50/index.html
../crawler http://old-www.cs.dartmouth.edu/~cs50/index.html 

echo $ ../crawler http://old-www.cs.dartmouth.edu/~cs50/index.html .
../crawler http://old-www.cs.dartmouth.edu/~cs50/index.html .

rm -f .crawler
