CC = gcc
CFLAGS  = -O0

all: rdtsc.c
	$(CC) $(CFLAGS) -o rdtsc rdtsc.c

clean:
	$(RM) rdtsc
