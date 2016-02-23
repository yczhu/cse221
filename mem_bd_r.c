#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>

static inline void dummy (int d) {}
int main(int argc, char * argv[]) {
    
    uint64_t start, end;
    uint64_t times;
    unsigned cycles_low, cycles_high, cycles_low1, cycles_high1;
    unsigned long k;
    unsigned int arraySize = 64*1024*1024*16/sizeof(int);
    int* array=(int *)malloc(arraySize*sizeof(int));

    memset (array, 1,arraySize);

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

   /* for (int i=0;i<size/2;i++){
        temp+=array[i];
        temp+=array[size/2+i];
        dummy(temp);
    }*/
    
    for (int i=0;i<arraySize-320;i+=320){
        temp+=array[i]
        // temp+=array[i+1];
        // temp+=array[i+2];
        // temp+=array[i+3];
        // temp+=array[i+4];
        // temp+=array[i+5];
        // temp+=array[i+6]; 
        // temp+=array[i+7];
        // temp+=array[i+8];
        // temp+=array[i+9];
        // temp+=array[i+10];
        
        // temp+=array[i+11];
        // temp+=array[i+12];
        // temp+=array[i+13];
        // temp+=array[i+14];
        // temp+=array[i+15];
        +array[i+16]
        +array[i+32]
        +array[i+48]
        +array[i+64]
        +array[i+80]
        +array[i+96]
        +array[i+112]
        +array[i+128]
        +array[i+144]
        +array[i+160]
        +array[i+176]
        +array[i+192]
        +array[i+208]
        +array[i+224]
        +array[i+240]
        +array[i+256]
        +array[i+272]
        +array[i+288]
        +array[i+304];

        dummy(temp);
    }
    asm volatile("RDTSCP\n\t"
            "mov %%edx, %0\n\t"
            "mov %%eax, %1\n\t"
            "CPUID\n\t": "=r" (cycles_high1), "=r" (cycles_low1):: "%rax",
            "%rbx", "%rcx", "%rdx");

    start = (((uint64_t)cycles_high << 32)| cycles_low );
    end= (((uint64_t)cycles_high1<< 32) | cycles_low1 );
    times = end - start;
    printf("%" PRIu64 "\n",times);
    double s=0;
    s=times/2.2*0.000000001;
    printf("%f MB/s\n", 64/s);
    return 0;
}