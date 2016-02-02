#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <inttypes.h>

static inline unsigned long long tick() 
{
    unsigned a, d;
    __asm__ __volatile__("rdtsc": "=a" (a), "=d" (d) );
    return (((unsigned long long)a) | (((unsigned long long)d) << 32));
}

void helper(){}
void helper1 (char a1){}
void helper2 (char a1,char a2){}
void helper3 (char a1,char a2,char a3){}
void helper4 (char a1,char a2,char a3,char a4){}
void helper5 (char a1,char a2,char a3,char a4,char a5){}
void helper6 (char a1,char a2,char a3,char a4,char a5,char a6){}
void helper7 (char a1,char a2,char a3,char a4,char a5,char a6,char a7){}

int main(int argc, char* argv[])
{
    int loops;
    int args;
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
       // args = atoi (argv[2]);
    }

    uint64_t start, end;
    unsigned cycles_low, cycles_high, cycles_low1, cycles_high1;
    uint64_t *times;

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
        asm volatile ("CPUID\n\t"
                "RDTSC\n\t"
                "mov %%edx, %0\n\t"
                "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::
                "%rax", "%rbx", "%rcx", "%rdx");

        // Call the function to measure here
        helper1('1');
        asm volatile("RDTSCP\n\t"
                "mov %%edx, %0\n\t"
                "mov %%eax, %1\n\t"
                "CPUID\n\t": "=r" (cycles_high1), "=r" (cycles_low1):: "%rax",
                "%rbx", "%rcx", "%rdx");

        start = (((uint64_t)cycles_high << 32)| cycles_low );
        end= (((uint64_t)cycles_high1<< 32) | cycles_low1 );
        times[i] =end - start;
    }

    double sum = 0;
    for (i = 0; i < loops; i++) {
        sum += times[i];
        printf("Loop %d: overhead = %" PRIu64 " cycles\n", i, times[i]);
    }

    printf("The average overhead is: %f cycles\n", sum / loops);

    return 0;
}
