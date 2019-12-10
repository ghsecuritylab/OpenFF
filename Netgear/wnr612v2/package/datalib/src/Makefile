#
#	Copyright (C) 2007-2008 Delta Networks Inc.
#
#CC = gcc

all: datalib

CFLAGS += -Wall -Wunused -g -O2

%.o: %.c
	$(CC) $(CFLAGS) -c $<

datalib: datalib.o defaults.o mtd.o
	$(CC) -o $@ $^

clean:
	rm -f defaults.o datalib
