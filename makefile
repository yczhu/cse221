CC = gcc
CFLAGS  = -O0

all: rdtsc2.c pcall0.c pcall1.c pcall2.c pcall3.c pcall4.c pcall5.c pcall6.c pcall7.c context_switch.c loop.c syscall.c pthread_context_switch.c pcreate.c tcreate.c
	$(CC) $(CFLAGS) -o rdtsc rdtsc2.c
	$(CC) $(CFLAGS) -o proc1 pcall1.c
	$(CC) $(CFLAGS) -o proc2 pcall2.c
	$(CC) $(CFLAGS) -o proc3 pcall3.c
	$(CC) $(CFLAGS) -o proc4 pcall4.c
	$(CC) $(CFLAGS) -o proc5 pcall5.c
	$(CC) $(CFLAGS) -o proc6 pcall6.c
	$(CC) $(CFLAGS) -o proc7 pcall7.c
	$(CC) $(CFLAGS) -o proc0 pcall0.c
	$(CC) $(CFLAGS) -o loop loop.c
	$(CC) $(CFLAGS) -o context_switch context_switch.c
	$(CC) $(CFLAGS) -o pcreate pcreate.c
	$(CC) $(CFLAGS) -o tcreate tcreate.c
	$(CC) $(CFLAGS) -o pthread_context_switch pthread_context_switch.c
	$(CC) $(CFLAGS) -o syscall syscall.c
	$(CC) $(CFLAGS) -funroll-loops -o mem_latency mem_latency.c
clean:
	$(RM) rdtsc proc0 proc1 proc2 proc3 proc4 proc5 proc6 proc7 loop context_switch pcreate tcreate syscall pthread_context_switch mem_latency
