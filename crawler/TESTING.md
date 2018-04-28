# TESTING.md for CS50 Lab 4 Crawler
## Shengsong Gao, April 2018

### Testing 'crawler'

Testing has never been made easier! Just do "make test" in the crawler
directory! That command will run the two shell scripts, "badCommandLine.sh" and
"crawling.sh", I made for, well, bad command line arguments and successful
crawling!

"badCommandLine.sh" tests many different invalid command line arguments on my
crawler - it echoes out each command line before it's run, so it's easy to see
"what went wrong."

"crawling.sh" tests 3 supposedly successful crawls. It compares the process
printed to stdout by my crawler and the demo crawler in ~cs50/demo/crawler -
they should be perfectly matching, and if so, my success message will be echoed
to stdout.

I didn't compare the HTML output files because of a strange difference between
each file my crawler produces and the demo crawler produces - upon human eye
examination, any pair of those two files are the same, but "diff" program still
points out that the files produced by the demo crawler has some extra random
characters compared to corresponding files produced by my crawler - again, they
are exactly the same otherwise, and since my files don't have those random
characters (and are thus about each 1 byte smaller), I think my solution works
out better.

Also, if you test with myvalgrind, you'll notice that my crawler allocates
fewer times, with fewer space total - supposedly better, because I don't observe a
significant performance drop, if there was any.

