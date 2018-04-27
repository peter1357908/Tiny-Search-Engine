# Makefile for CS50 'common' library
#   Builds and compiles all these utility functions into a library.
#
# Shengsong Gao, April 2018. Modified from libcs50/Makefile

# object files, and the target library
LIBPATH = ../libcs50/
OBJS = pagedir.o word.o $(LIBPATH)webpage.o $(LIBPATH)file.o $(LIBPATH)memory.o
LIB = libcommon.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS) -I$(LIBPATH)
CC = gcc
MAKE = make

# Build the library by archiving object files
$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

# Dependencies: object files depend on header files
pagedir.o: pagedir.h $(LIBPATH)webpage.h $(LIBPATH)file.h $(LIBPATH)memory.h
word.o: word.h
../libcs50/webpage.o: $(LIBPATH)webpage.h $(LIBPATH)file.h $(LIBPATH)memory.h

.PHONY: clean sourcelist

# list all the sources and docs in this directory.
sourcelist: Makefile *.md *.c *.h
	@echo $^

# clean up after our compilation
clean:
	rm -f $(LIB) *~ *.o
