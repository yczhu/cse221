CC = gcc
CFLAGS  = -O0

all: rdtsc.c
	$(CC) $(CFLAGS) -o rdtsc rdtsc2.c

clean:
	$(RM) rdtsc
