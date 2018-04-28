# IMPLEMENTATION.md for CS50 Lab 4 Crawler
## Shengsong Gao, April 2018

### Pseudo Code (C and Implement-dependent)
The following is a modified transcript of the relevant comments in my crawler.c
My program perfectly follows the pseudo code suggested by the TSE Crawler Design Spec
(Which is why I dare to compare the process printed by my crawler with the process printed by ~cs50/demo/crawler in [TESTING](TESTING.md))

### main()
	1. parse the command line, validate parameters, initialize other modules
	2. easiest check first: maxDepth is an int and >= 0
	3. normalize the seedURL first and exit on error
	4. try to initialize the normalized seedURL as a webpage_t based on input
	5. test writing to pageDirectory by creating .crawler there
	6. create it with fopen(w), and immediately close it
	7. initialize the bag of webpages we have yet to explore
	8. initialize the hashtable of URLs we've seen so far - each URL as key, and a dummy character pointer as value (because NULL is not allowed as value)

### crawler()

#### setup
	1. do the actual crawlin'! 
	2. add the seedPage to the bag of webpages to crawl, assuming no error
	3. mark its normalized URL as visited, assuming no error
#### major loop
  1. while there are more webpages to crawl, extract one from the bag.
    1. retrieve the webpage, exit upon failure.
    2. webpage_fetch() already implements a 1-sec delay after each attempt
    3. if for some reason its html is null, ignore this URL and continue
    4. else, fetch succeeded, write it to the specified path with current id.
    5. Increment id and exit on failure
    6. explore the webpage if its depth is < maxDepth
      1. get the nextURL if possible
      2. currpage's html is compressed as a side effect, but doesn't matter, because it's already "fetched" into a file and won't be used again
        1. only proceed with URL that's valid and internal
          1. if it's never visited before, insert it into the bag
          2. (it's marked as visited in the process)
        2. free the memory allocated by webpage_getNextURL()
      3. reset pos    
    7. free the current webpage - it's no longer useful
  2. clean up! Free every allocation.	

### Note:
1. BEWARE: hashtable size is set to be 30... under the current requirements, it's not easy to and thus I didn't make it adaptive.
2. I made my own notNull() function, which is similar to notNull(), but actually frees everything before exiting.


