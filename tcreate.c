#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>
#include <pthread.h>

static inline unsigned long long tick() 
{
    unsigned a, d;
    __asm__ __volatile__("rdtsc": "=a" (a), "=d" (d) );
    return (((unsigned long long)a) | (((unsigned long long)d) << 32));
}

unsigned cycles_low, cycles_high;
uint64_t start;
void * runner (uint64_t* data){
    uint64_t end;
    unsigned cycles_low1,cycles_high1;
    asm volatile("RDTSCP\n\t"
          "mov %%edx, %0\n\t"
          "mov %%eax, %1\n\t"
          "CPUID\n\t": "=r" (cycles_high1), "=r" (cycles_low1):: "%rax",
          "%rbx", "%rcx", "%rdx");   
    end= (((uint64_t)cycles_high1<< 32) | cycles_low1 );
    printf("%" PRIu64 " cycles\n",end-*data);
}

int main(int argc, char* argv[])
{
    int loops;
    int i, j;
    if (argc <= 1) {
        exit(0);
    }
    else {
        loops = atoi (argv[1]);
        if (loops <= 0) {
            printf ("Invalid arguments.\n");
            exit(0);
        }
    }

    uint64_t *times;
    unsigned cycles_low, cycles_high, cycles_low1, cycles_high1;
    uint64_t start,end;
    times = malloc(loops * sizeof(uint64_t*));

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

    for(i = 0; i < loops; i++) { 
        pthread_t t;
        asm volatile ("CPUID\n\t"
                "RDTSC\n\t"
                "mov %%edx, %0\n\t"
                "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::
                "%rax", "%rbx", "%rcx", "%rdx"); 
        start = (((uint64_t)cycles_high << 32)| cycles_low );
        pthread_create (&t,(pthread_attr_t *) NULL,runner,&start);
        pthread_join(t,NULL);
    }

    return 0;
}
