#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <inttypes.h>

/* 
	mkfile 1G 1G.tmp
	ls -lh 1G.tmp
*/

static inline unsigned long long tick() 
{
    unsigned a, d;
    __asm__ __volatile__("rdtsc": "=a" (a), "=d" (d) );
    return (((unsigned long long)a) | (((unsigned long long)d) << 32));
}


#define BLOCK_SIZE 4096

int main(int argc, char* argv[]) {

	/* system("sudo purge"); */

	int file_size_gb; // GB
	int loops = 1;

    if (argc <= 1) {
        exit(0);
    }
    else {
        file_size_gb = atoi (argv[1]);
        if (file_size_gb <= 0) {
            printf ("Invalid arguments.\n");
            exit(0);
        }
    }

	char file_name[10];
	strcpy(file_name, argv[1]);
	strcat(file_name, "G.tmp");

	printf("Open File: %s.\n", file_name);

	void *buffer = malloc(BLOCK_SIZE);

	/* read for the first time*/
	printf ("1st read.\n");
	int file = open(file_name, O_RDONLY);
	if (file == -1){
		printf ("Invalid file name.\n");
        exit(0);
	}

	lseek(file, 0, SEEK_SET);
	int gbCount = 0;
	unsigned long long len = 1 * 1024 * 1024 * 1024; // 1 GB
	int bytes_read = 0;
	unsigned long long total_bytes_read = 0;

	while(gbCount < file_size_gb){
		// read 1 GB
		while (total_bytes_read < len){
			bytes_read = read(file, buffer, BLOCK_SIZE);
			total_bytes_read += bytes_read;
		}
		total_bytes_read = 0;
		gbCount += 1;

		// while(offset < len){
		// 	fseek(fp, -2 * BLOCK_SIZE, SEEK_CUR);
		// 	fread(p, 1, BLOCK_SIZE, fp);
		// 	offset += BLOCK_SIZE;
		// }
		// printf("%d GB read.\n", offset/(1024*1024*1024));
		// offset = 0;
		// gbCount += 1;
		// fseek(fp, len, SEEK_CUR);
	}
	printf("%d GB read.\n", gbCount);

	// start to measure time
	uint64_t start, end;
    uint64_t *times;
    unsigned cycles_low, cycles_high, cycles_low1, cycles_high1;
    int i;

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

    printf ("Start to measure.\n");

    for(i = 0; i < loops; i++) {

    	gbCount = 0;
    	total_bytes_read = 0;
		lseek(file, 0, SEEK_SET);

        asm volatile ("CPUID\n\t"
                "RDTSC\n\t"
                "mov %%edx, %0\n\t"
                "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::
                "%rax", "%rbx", "%rcx", "%rdx");

		// Call the function to measure here
       	while(gbCount < file_size_gb){
			// read 1 GB
			while (total_bytes_read < len){
				bytes_read = read(file, buffer, BLOCK_SIZE);
				total_bytes_read += bytes_read;			}
			//printf("%llu GB read.\n", total_bytes_read/(1024*1024*1024));
			total_bytes_read = 0;
			gbCount += 1;
		}
		printf("%d GB read.\n", gbCount);

        asm volatile("RDTSCP\n\t"
                "mov %%edx, %0\n\t"
                "mov %%eax, %1\n\t"
                "CPUID\n\t": "=r" (cycles_high1), "=r" (cycles_low1):: "%rax",
                "%rbx", "%rcx", "%rdx");

        start = (((uint64_t)cycles_high << 32)| cycles_low );
        end = (((uint64_t)cycles_high1<< 32) | cycles_low1 );
        times[i] = end - start;
    }

    double seconds = 0;
    for (i = 0; i < loops; i++) {
    	seconds = times[i] / 2.2 * 0.000000001;
        printf("%f GB/s\n", file_size_gb / seconds);
    }

	close(file);
    free(buffer);
	return 0;
}