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
        printf("%"PRIu64"\n",times[i]);
    }

    //printf("The average overhead is: %f cycles\n", sum / loops);

   /* asm volatile ("CPUID\n\t"
            "RDTSC\n\t"
            "mov %%edx, %0\n\t"
            "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::
            "%rax", "%rbx", "%rcx", "%rdx");
    sleep(10);
    asm volatile("RDTSCP\n\t"
            "mov %%edx, %0\n\t"
            "mov %%eax, %1\n\t"
            "CPUID\n\t": "=r" (cycles_high1), "=r" (cycles_low1):: "%rax",
            "%rbx", "%rcx", "%rdx");
    start = (((uint64_t)cycles_high << 32)| cycles_low );
    end= (((uint64_t)cycles_high1<< 32) | cycles_low1 );
    printf("The 10s sleep is %" PRIu64 " cycles\n", (end - start));

    asm volatile ("CPUID\n\t"
            "RDTSC\n\t"
            "mov %%edx, %0\n\t"
            "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::
            "%rax", "%rbx", "%rcx", "%rdx");
    for(i = 0; i < loops; i++) {
    }
    asm volatile("RDTSCP\n\t"
            "mov %%edx, %0\n\t"
            "mov %%eax, %1\n\t"
            "CPUID\n\t": "=r" (cycles_high1), "=r" (cycles_low1):: "%rax",
            "%rbx", "%rcx", "%rdx");
    start = (((uint64_t)cycles_high << 32)| cycles_low );
    end= (((uint64_t)cycles_high1<< 32) | cycles_low1 );
    printf("The loop overhead is %f cycles\n", (double) (end - start)/loops);
*/
    return 0;
}
