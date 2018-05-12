# Makefile for 'querier' module of the TSE project
#
# After reading the sample solution to indexer, I chose to imitate its decision
# on Makefile - assume that the library is already up-to-date compared to its
# individual modules.
#
# Shengsong Gao, May 2018

CS50PATH = ../libcs50/
COMMONPATH = ../common/
PROG = querier
OBJS = querier.o
LIBS = -lcs50 -lcommon


CFLAGS = -Wall -pedantic -std=gnu11 -ggdb -I$(CS50PATH) -I$(COMMONPATH) -L$(CS50PATH) -L$(COMMONPATH)
CC = gcc
MAKE = make

.PHONY: test clean

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

querier.o: $(CS50PATH)libcs50.a $(COMMONPATH)libcommon.a


test: $(PROG)
	cd tests; ./badCommandLine.sh; ./querying.sh

clean:
	rm -f *~ *.o *.dSYM
	rm -f $(PROG)
	rm -f ./tests/output/*
