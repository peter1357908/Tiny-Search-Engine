# Makefile for 'crawler' module of the TSE project
#
# Shengsong Gao, April 2018

CS50PATH = ../libcs50/
CS50HEADERS = $(CS50PATH)bag.h $(CS50PATH)counters.h $(CS50PATH)file.h $(CS50PATH)hashtable.h $(CS50PATH)jhash.h $(CS50PATH)set.h $(CS50PATH)webpage.h $(CS50PATH)memory.h
COMMONPATH = ../common/
COMMONHEADERS = $(COMMONPATH)pagedir.h $(COMMONPATH)word.h
PROG = crawler
OBJS = crawler.o
LIBS = -lcurl -lcs50 -lcommon


CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$(CS50PATH) -I$(COMMONPATH) -L$(CS50PATH) -L$(COMMONPATH)
CC = gcc
MAKE = make

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

crawler.o: $(CS50PATH)libcs50.a $(COMMONPATH)libcommon.a

$(CS50PATH)libcs50.a: $(CS50HEADERS)
	$(MAKE) -C $(CS50PATH)

$(COMMONPATH)libcommon.a: $(COMMONHEADERS)
	$(MAKE) -C $(COMMONPATH)


.PHONY: test clean

test: $(PROG)
	./$(PROG)

clean:
	rm -f *~ *.o *.dSYM
	rm -f $(PROG)
