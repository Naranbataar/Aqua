CFLAGS= -O3 -Wall
main: main.c
	$(CC) $(CFLAGS) src/* -Iinclude/ main.c -o aqua

all: main

clean: 
	rm -f aqua

install:
	cp aqua /usr/local/bin/

PHONY = all clean install
