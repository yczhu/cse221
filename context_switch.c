#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int loops;
    int i, j;

    uint64_t *times;
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

    times = malloc(loops*sizeof(uint64_t*));

    uint64_t start, end;
    unsigned cycles_low, cycles_high, cycles_low1, cycles_high1;

    pid_t childpid;
    char buffer;

    // Pipe
    int fd[2];
    if (pipe(fd) == -1) {
        perror("Pipe setup error!");
        exit(1);
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

    for (i = 0; i < loops; i++) {
        if ((childpid = fork()) == -1) {
            perror("fork");
            exit(1);
        }

        if(childpid == 0) {
        // In the child process
            write(fd[1], "a", 1);
            exit(0);
        }
        else {
            // In the parent process
            // Start rdtsc
            asm volatile ("CPUID\n\t"
                    "RDTSC\n\t"
                    "mov %%edx, %0\n\t"
                    "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::
                    "%rax", "%rbx", "%rcx", "%rdx"); 
            read(fd[0], &buffer, 1);
        }
        
        // End rdtsc
        asm volatile("RDTSCP\n\t"
                "mov %%edx, %0\n\t"
                "mov %%eax, %1\n\t"
                "CPUID\n\t": "=r" (cycles_high1), "=r" (cycles_low1):: "%rax",
                "%rbx", "%rcx", "%rdx");

        start = (((uint64_t)cycles_high << 32)| cycles_low );
        end= (((uint64_t)cycles_high1<< 32) | cycles_low1 );
        times[i] = end - start;
    }

    double sum = 0.0;

    for (i = 0; i < loops; i++) {
        sum += times[i];
        printf("Loop %d: overhead = %" PRIu64 " cycles\n", i, times[i]);
    }
    printf("The average overhead is: %f cycles\n", sum / loops);

    return 0;
}
