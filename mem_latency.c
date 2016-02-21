#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>

void mem_latency(unsigned long size, int stride, unsigned long iters) {
    char** array = (char **) malloc(sizeof(char*) * size);

    uint64_t start, end;
    uint64_t times;
    unsigned cycles_low, cycles_high, cycles_low1, cycles_high1;
    unsigned long k;

    for (k = 0; k < size; k++) {
        array[k] = (char*) &array[(k + stride + ((rand() % 10) - 10)) % size];
    }
   
    // Warm up the system
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

    asm volatile ("CPUID\n\t"
            "RDTSC\n\t"
            "mov %%edx, %0\n\t"
            "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::
            "%rax", "%rbx", "%rcx", "%rdx");

    int i;
    char ** p;
    p = array;

    for (i = 0; i < iters; i++) {
        //array[(i * stride) % size]++;
        p = (char **)*p;
    }

    asm volatile("RDTSCP\n\t"
            "mov %%edx, %0\n\t"
            "mov %%eax, %1\n\t"
            "CPUID\n\t": "=r" (cycles_high1), "=r" (cycles_low1):: "%rax",
            "%rbx", "%rcx", "%rdx");

    free(array);

    start = (((uint64_t)cycles_high << 32)| cycles_low );
    end= (((uint64_t)cycles_high1<< 32) | cycles_low1 );
    times = end - start;
    printf("Size %lu KB: overhead = %" PRIu64 " cycles\n", size / 1024, times);

}
int main(int argc, char * argv[]) {
    int stride = 32; // Stride in Bytes
    unsigned long size_in_kb = 16;  // Size in KB
    unsigned long iters = 1000000;

    printf("Stride: %d\n", stride);
    while(size_in_kb <= 1024*32) {
        mem_latency(size_in_kb * 1024, stride, iters);
        size_in_kb *= 2;
    }
    return 0;
}
