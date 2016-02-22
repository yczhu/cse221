#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>

//static inline void dummy (int d) {}
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

    for (int i=0;i<size-50;i+=50){
        array[i]=1;
        array[i+1]=1; 
        array[i+2]=1;
        array[i+3]=1;
        array[i+4]=1;
        array[i+5]=1;
        array[i+6]=1;
        array[i+7]=1;
        array[i+8]=1;
        array[i+9]=1;
        array[i+10]=1;
        array[i+11]=1;
        array[i+12]=1;
        array[i+13]=1;
        array[i+14]=1;
        array[i+15]=1;
        array[i+16]=1;
        array[i+17]=1;
        array[i+18]=1;
        array[i+19]=1;
        array[i+20]=1;
        array[i+21]=1;
        array[i+22]=1;
        array[i+23]=1;
        array[i+24]=1;
        array[i+25]=1;
        array[i+26]=1;
        array[i+27]=1;
        array[i+28]=1;
        array[i+29]=1;
        array[i+30]=1;
        array[i+31]=1;
        array[i+32]=1;
        array[i+33]=1;
        array[i+34]=1;
        array[i+35]=1;
        array[i+36]=1;
        array[i+37]=1;
        array[i+38]=1;
        array[i+39]=1;
        array[i+40]=1;
        array[i+41]=1;
        array[i+42]=1;
        array[i+43]=1;
        array[i+44]=1;
        array[i+45]=1;
        array[i+46]=1;
        array[i+47]=1;
        array[i+48]=1;
        array[i+49]=1;
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
