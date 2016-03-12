#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>

#define BYTES_MB (1024*1024)
#define MB 1024
#define SIZE (BYTES_MB*MB)


static inline void dummy (size_t d) {}

void read_mem(size_t* array, size_t size) {
	size_t temp = 0;
	size_t i;
	for (i = 0; i < size / sizeof(size_t); i++) {
		temp += array[i];
	}
	// dummy(temp); // no much difference
}

int main(int argc, char * argv[]) {

	// init array
	//size_t* array = (size_t*) malloc(SIZE * sizeof(size_t));
	char* array;
	posix_memalign((void*)&array, SIZE, SIZE);
	memset(array, 0xFF, SIZE);
	
	// measure time
	uint64_t start, end;
    uint64_t times;
    unsigned cycles_low, cycles_high, cycles_low1, cycles_high1;

	asm volatile ("CPUID\n\t"
            "RDTSC\n\t"
            "mov %%edx, %0\n\t"
            "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::
            "%rax", "%rbx", "%rcx", "%rdx");
    asm volatile("RDTSCP\n\t"
            "mov %%edx, %0\n\t"
            "mov %%eax, %1\n\t"
            "CPUID\n\t": "=r" (cycles_high1), "=r" (cycles_low1):: "%rax",
            "%rbx", "%rcx", "%rdx");

    asm volatile ("CPUID\n\t"
            "RDTSC\n\t"
            "mov %%edx, %0\n\t"
            "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::
            "%rax", "%rbx", "%rcx", "%rdx");
    asm volatile("RDTSCP\n\t"
            "mov %%edx, %0\n\t"
            "mov %%eax, %1\n\t"
            "CPUID\n\t": "=r" (cycles_high1), "=r" (cycles_low1):: "%rax",
            "%rbx", "%rcx", "%rdx");
    int temp=0;
    asm volatile ("CPUID\n\t"
            "RDTSC\n\t"
            "mov %%edx, %0\n\t"
            "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low):: 
            "%rax", "%rbx", "%rcx", "%rdx");

    // measure time here
    read_mem((size_t*)array, SIZE);

    asm volatile("RDTSCP\n\t"
            "mov %%edx, %0\n\t"
            "mov %%eax, %1\n\t"
            "CPUID\n\t": "=r" (cycles_high1), "=r" (cycles_low1):: "%rax",
            "%rbx", "%rcx", "%rdx");

    start = (((uint64_t)cycles_high << 32)| cycles_low );
    end= (((uint64_t)cycles_high1<< 32) | cycles_low1 );
    times = end - start;

    double seconds = times / 2.2 * 0.000000001;
    double speed = ( MB / seconds) / 1024.0;
    printf("%f GB/s\n", speed);

	return 0;
}