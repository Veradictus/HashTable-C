CC = gcc
CFLAGS = -Wall -std=c99
RM = rm -rf
TAR = tar -zcvf

all: test

hashP: hashP.c
	$(CC) $(CFLAGS) -g -o hashP hashP.c

test: test.c hashP.c
	$(CC) $(CFLAGS) -o test test.c hashP.c

clean:
	$(RM) test hashP
.PHONY: clean all
