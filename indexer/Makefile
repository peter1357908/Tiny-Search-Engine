# Makefile for 'indexer' module of the TSE project
#
# Really, I just want this Makefile to be as quite as possible, so it's in fact
# checking dependencies in other Makefiles' stead. Can be simplified, but it's
# definitely faster than asking other Makefiles to do their job every time
# (then the indexer is always recompiled, which loses the point of having a
# Makefile)
#
# Shengsong Gao, May 2018

CS50PATH = ../libcs50/
CS50HEADERS = $(CS50PATH)bag.h $(CS50PATH)counters.h $(CS50PATH)file.h $(CS50PATH)hashtable.h $(CS50PATH)jhash.h $(CS50PATH)set.h $(CS50PATH)webpage.h $(CS50PATH)memory.h
CS50FILES = $(CS50PATH)bag.c $(CS50PATH)counters.c $(CS50PATH)file.c $(CS50PATH)hashtable.c $(CS50PATH)jhash.c $(CS50PATH)set.c $(CS50PATH)webpage.c $(CS50PATH)memory.c
COMMONPATH = ../common/
COMMONHEADERS = $(COMMONPATH)pagedir.h $(COMMONPATH)word.h $(COMMONPATH)index.h
COMMONFILES = $(COMMONPATH)pagedir.c $(COMMONPATH)word.c $(COMMONPATH)index.c
PROG1 = indexer
PROG2 = indextest
OBJS1 = indexer.o
OBJS2 = indextest.o
LIBS = -lcs50 -lcommon


CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$(CS50PATH) -I$(COMMONPATH) -L$(CS50PATH) -L$(COMMONPATH)
CC = gcc
MAKE = make

.PHONY: test clean all

all: $(PROG1) $(PROG2) 

$(PROG1): $(OBJS1)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

$(PROG2): $(OBJS2)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

indexer.o: $(CS50PATH)libcs50.a $(COMMONPATH)libcommon.a

indextest.o: $(CS50PATH)libcs50.a $(COMMONPATH)libcommon.a

$(CS50PATH)libcs50.a: $(CS50FILES) $(CS50HEADERS)
	$(MAKE) -C $(CS50PATH)

$(COMMONPATH)libcommon.a: $(COMMONFILES) $(COMMONHEADERS)
	$(MAKE) -C $(COMMONPATH)


test: $(PROG)
	cd tests; bash ./badCommandLine.sh; echo; bash ./indexing.sh; bash ./indextesttest.sh

clean:
	rm -f *~ *.o *.dSYM
	rm -f $(PROG)
	rm -f ./tests/output/*
