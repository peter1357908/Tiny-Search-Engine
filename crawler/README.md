# README.md for CS50 Lab 4 Crawler
## Shengsong Gao, April 2018

### crawler requirements spec

The TSE crawler is a standalone program that crawls the web and retrieves webpages starting from a “seed” URL. It parses the seed webpage, extracts any embedded URLs, then retrieves each of those pages, recursively, but limiting its exploration to a given “depth”.

#### The crawler shall:

1. execute from a command line with usage syntax
  * ./crawler seedURL pageDirectory maxDepth
  * where seedURL is used as the initial URL,
  * where pageDirectory is the pathname for an existing directory in which to write downloaded webpages, and
  * where maxDepth is a non-negative integer representing the maximum crawl depth.
2. crawl all pages reachable from seedURL, following links to a maximum depth of maxDepth; where maxDepth=0 means that crawler only explores the page at seedURL, maxDepth=1 means that crawler only explores the page at seedURL and those pages to which seedURL links, and so forth inductively.
3. pause at least one second between page fetches.
4. ignore URLs that are not “internal” (meaning, outside the designated CS50 server).
5. write each explored page to the pageDirectory with a unique document ID, wherein
  * the document id starts at 1 and increments by 1 for each new page,
  * and the filename is of form pageDirectory/id,
  * and the first line of the file is the URL,
  * and the second line of the file is the depth,
  * and the rest of the file is the page content (the HTML, unchanged).

To be polite, our crawler purposely slows its behavior by introducing a delay, sleeping for one second between fetches.

Furthermore, our crawler will limit its crawl to a specific web server inside CS, so we don’t bother any other servers on campus or beyond.

### Usage
```
crawler seedURL pageDirectory maxDepth
```

### Implementation and Assumptions

please refer to the [IMPLEMENTATION](IMPLEMENTATION.md) in the same directory.

### Compilation

To compile, simply `make`.

To test, simply `make test`.

See [TESTING](TESTING.md) for details of testing!
