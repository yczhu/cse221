#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static inline unsigned long long tick() 
{
    unsigned a, d;
    __asm__ __volatile__("rdtsc": "=a" (a), "=d" (d) );
    return (((unsigned long long)a) | (((unsigned long long)d) << 32));
    //unsigned long long d;
    //__asm__ __volatile__ ("rdtsc" : "=A" (d) );
    //return d;
}

int main(int argc, char* argv[])
{
    int loops;
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

    unsigned long long start, end;
    int i;
    start = tick();
    for(i=0; i < loops; i++) {
        __asm__ __volatile__("rdtsc");
    }
    end = tick();
    printf("argv: %d, rdtsc: %f\n", loops, (double)(end- start) / (double)loops);

    start = tick();
    for(i=0; i < loops; i++) {
    }
    end = tick();
    printf("argv: %d, loops: %f\n", loops, (double)(end- start) / (double)loops);


    start = tick();
    sleep(10);
    end = tick();
    printf("cycles: %llu\n",(end- start));

    return 0;
}
