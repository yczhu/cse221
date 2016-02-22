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
    unsigned int size=1024*1024/sizeof(int);
    int* array=(int *)malloc(size*32);

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
    
    for (int i=0;i<size-50;i+=50){
        temp+=array[i];
        temp+=array[i+1];
        temp+=array[i+2];
        temp+=array[i+3];
        temp+=array[i+4];
        temp+=array[i+5];
        temp+=array[i+6]; 
        temp+=array[i+7];
        temp+=array[i+8];
        temp+=array[i+9];
        temp+=array[i+10];
        
        temp+=array[i+11];
        temp+=array[i+12];
        temp+=array[i+13];
        temp+=array[i+14];
        temp+=array[i+15];
        temp+=array[i+16]; 
        temp+=array[i+17];
        temp+=array[i+18];
        temp+=array[i+19];

        temp+=array[i+20];
        temp+=array[i+21];
        temp+=array[i+22];
        temp+=array[i+23];
        temp+=array[i+24];
        temp+=array[i+25];
        temp+=array[i+26]; 
        temp+=array[i+27];
        temp+=array[i+28];
        temp+=array[i+29];

        temp+=array[i+30];
        temp+=array[i+31];
        temp+=array[i+32];
        temp+=array[i+33];
        temp+=array[i+34];
        temp+=array[i+35];
        temp+=array[i+36]; 
        temp+=array[i+37];
        temp+=array[i+38];
        temp+=array[i+39];
        temp+=array[i+40];
        temp+=array[i+41];
        temp+=array[i+42];
        temp+=array[i+43];
        temp+=array[i+44];
        temp+=array[i+45];
        temp+=array[i+46]; 
        temp+=array[i+47];
        temp+=array[i+48];
        temp+=array[i+49];
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
    s=times/2.2*0.000001;
    printf("%f\n",32*1024*1024/s);
    return 0;
}
