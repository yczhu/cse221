#include<pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <unistd.h>

pthread_t tid;
int test_cond = 0;

pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void write_fd(void *arg) {
    pthread_mutex_lock(&mut);
    //printf("Pthread wait!\n");
    //test_cond = 1;
    pthread_cond_wait(&cond, &mut);
    //printf("Pthread wait done!\n");
    pthread_mutex_unlock(&mut);
    pthread_exit(0);
}

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
        test_cond = 0;
        int err = pthread_create(&tid, NULL, (void *) &write_fd, NULL);

        if (err != 0) {
            perror("Pthread creation failed!\n");
            exit(1);
        }

        //printf("Main ready to signal\n");
        pthread_mutex_lock(&mut);
            asm volatile ("CPUID\n\t"
                    "RDTSC\n\t"
                    "mov %%edx, %0\n\t"
                    "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::
                    "%rax", "%rbx", "%rcx", "%rdx");
            pthread_cond_signal(&cond);
            asm volatile("RDTSCP\n\t"
                    "mov %%edx, %0\n\t"
                    "mov %%eax, %1\n\t"
                    "CPUID\n\t": "=r" (cycles_high1), "=r" (cycles_low1):: "%rax",
                    "%rbx", "%rcx", "%rdx");
        pthread_mutex_unlock(&mut);

        //printf("Back to main!\n");

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