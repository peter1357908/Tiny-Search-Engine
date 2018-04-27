# Makefile for 'crawler' module of the TSE project
#
# Really, I just want this Makefile to be as quite as possible, so it's in fact
# checking dependencies in other Makefiles' stead. Can be simplified, but it's
# definitely faster than asking other Makefiles to do their job every time
# (then crawler is always recompiled, which loses the point of having a
# Makefile)
#
# Shengsong Gao, April 2018

CS50PATH = ../libcs50/
CS50HEADERS = $(CS50PATH)bag.h $(CS50PATH)counters.h $(CS50PATH)file.h $(CS50PATH)hashtable.h $(CS50PATH)jhash.h $(CS50PATH)set.h $(CS50PATH)webpage.h $(CS50PATH)memory.h
CS50FILES = $(CS50PATH)bag.c $(CS50PATH)counters.c $(CS50PATH)file.c $(CS50PATH)hashtable.c $(CS50PATH)jhash.c $(CS50PATH)set.c $(CS50PATH)webpage.c $(CS50PATH)memory.c
COMMONPATH = ../common/
COMMONHEADERS = $(COMMONPATH)pagedir.h $(COMMONPATH)word.h
COMMONFILES = $(COMMONPATH)pagedir.c $(COMMONPATH)word.c
PROG = crawler
OBJS = crawler.o
LIBS = -lcurl -lcs50 -lcommon


CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$(CS50PATH) -I$(COMMONPATH) -L$(CS50PATH) -L$(COMMONPATH)
CC = gcc
MAKE = make

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

crawler.o: $(CS50PATH)libcs50.a $(COMMONPATH)libcommon.a

$(CS50PATH)libcs50.a: $(CS50FILES) $(CS50HEADERS)
	$(MAKE) -C $(CS50PATH)

$(COMMONPATH)libcommon.a: $(COMMONFILES) $(COMMONHEADERS)
	$(MAKE) -C $(COMMONPATH)


.PHONY: test clean

test: $(PROG)
	./$(PROG)

clean:
	rm -f *~ *.o *.dSYM
	rm -f $(PROG)
