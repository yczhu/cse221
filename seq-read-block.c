//#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <inttypes.h>

#define BLOCK_SIZE 4096
#define MEGA_BLOCKS 1024*1024

char *buffer;

int main(int argc, char** argv) {
    int fd, val;
    int total_size_kb;
    unsigned long long total_blocks;
    unsigned long long total_size, total_read;

    uint64_t start, end;
    unsigned cycles_low, cycles_high, cycles_low1, cycles_high1;

    fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        perror("File open failed!\n");
        exit(1);
    }
    // Non-cache set
    fcntl(fd, F_NOCACHE, 1);

    total_size_kb = atoi(argv[2]);
    total_size = total_size_kb * 1024;
    total_read = 0;
    //posix_memalign((void*)&buffer, MEGA_BLOCKS, MEGA_BLOCKS);
    posix_memalign((void*)&buffer, BLOCK_SIZE, BLOCK_SIZE);

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


    int count = 0;
    asm volatile ("CPUID\n\t"
            "RDTSC\n\t"
            "mov %%edx, %0\n\t"
            "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::
            "%rax", "%rbx", "%rcx", "%rdx");
    
    while(1) {
        //val = read(fd, buffer, MEGA_BLOCKS);
        val = read(fd, buffer, BLOCK_SIZE);
        if (val <= 0) {
            perror("Read file failed!\n");
            exit(1);
        }

        total_read += val;

        if (total_read >= total_size) {
            asm volatile("RDTSCP\n\t"
                    "mov %%edx, %0\n\t"
                    "mov %%eax, %1\n\t"
                    "CPUID\n\t": "=r" (cycles_high1), "=r" (cycles_low1):: "%rax",
                    "%rbx", "%rcx", "%rdx");

            start = (((uint64_t)cycles_high << 32)| cycles_low );
            end= (((uint64_t)cycles_high1<< 32) | cycles_low1 );
            //printf("Iter %d, Overhead = %" PRIu64 " cycles\n", count, end-start);
            printf("%" PRIu64 "\n",end-start);

            count++;
            if  (count == 100) {
                break;
            }

            val = lseek(fd, 0, SEEK_SET);
            if (val < 0) {
                perror("lseek error!\n");
                exit(1);
            }
            total_read = 0;
            asm volatile ("CPUID\n\t"
                    "RDTSC\n\t"
                    "mov %%edx, %0\n\t"
                    "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::
                    "%rax", "%rbx", "%rcx", "%rdx");
            continue;
        }
    }
}
