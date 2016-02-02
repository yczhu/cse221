CC = gcc
CFLAGS  = -O0

all: rdtsc2.c context_switch.c loop.c
	$(CC) $(CFLAGS) -o rdtsc rdtsc2.c
	$(CC) $(CFLAGS) -o loop loop.c
	$(CC) $(CFLAGS) -o context_switch context_switch.c

clean:
	$(RM) rdtsc
	$(RM) loop
	$(RM) context_switch
