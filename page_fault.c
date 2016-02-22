#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <err.h>
#include <sys/mman.h> // mmap
#include <fcntl.h>  // for open
#include <unistd.h> // for close
#include <string.h>
#include <inttypes.h>

#define PAGE_SIZE 4096  // 4k page size
#define TEST_PAGES 300

int main(int argc, char* argv[]) {
    int fd = -1;
    char *zero;
    //const char str1[] = "string 1";

    uint64_t start, end;
    uint64_t times;
    unsigned cycles_low, cycles_high, cycles_low1, cycles_high1;
    uint64_t i;
    uint64_t size;

    if ((fd = open("foobar", O_RDWR, 0)) == -1)
        err(1, "open");

    size = 4096 * 100000;
    char c;

    zero = (char*)mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_FILE|MAP_SHARED, fd, 0);
    /*
    for (i=0; i < size / PAGE_SIZE; i++) {
        c = zero[(i * PAGE_SIZE) % size];
    }
    */
    asm volatile ("CPUID\n\t"
            "RDTSC\n\t"
            "mov %%edx, %0\n\t"
            "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::
            "%rax", "%rbx", "%rcx", "%rdx");

    /*
    for (i=0; i < TEST_PAGES; i++) {
        c = zero[rand() % size];
    }
    */
    c = zero[0];
    c = zero[12098];
    c = zero[90000];
    c = zero[450000];
    c = zero[500000];
    c = zero[600000];
    c = zero[890000];
    c = zero[1000000];
    c = zero[1006820];
    c = zero[2008720];

    asm volatile("RDTSCP\n\t"
            "mov %%edx, %0\n\t"
            "mov %%eax, %1\n\t"
            "CPUID\n\t": "=r" (cycles_high1), "=r" (cycles_low1):: "%rax",
            "%rbx", "%rcx", "%rdx");

    start = (((uint64_t)cycles_high << 32)| cycles_low );
    end= (((uint64_t)cycles_high1<< 32) | cycles_low1 );
    times = end - start;
    printf("Overhead = %" PRIu64 " cycles\n", times);

    //if (zero == MAP_FAILED)
    //    err(1, "mmap");

    munmap(zero, size);
    close(fd);
    return 0;
}
