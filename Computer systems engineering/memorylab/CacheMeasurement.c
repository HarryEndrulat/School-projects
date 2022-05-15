#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 8 MB
//#define MAX_WORKING_SET_SIZE (1 << 23)
// 16 MB
#define MAX_WORKING_SET_SIZE (1 << 24)
// 32 MB
//#define MAX_WORKING_SET_SIZE (1 << 25)

struct option {
    char description[80];
    void (*function)(void);
};

#define NUMBER_OF_OPTIONS 3

int running = 1;

void measure_cache_sizes();
void measure_cache_lines();
void terminate();

int main() {
    struct option options[NUMBER_OF_OPTIONS] = {
            {
                    .description = "Measure the sizes of cache levels",
                    .function = measure_cache_sizes,
            },
            {
                    .description = "Determine the size of a cache line",
                    .function = measure_cache_lines,
            },
            {
                    .description = "Exit program",
                    .function = terminate,
            },
    };
    int choice;
    while (running) {
        for (int i = 0; i < NUMBER_OF_OPTIONS; i++) {
            printf("%3d. %s\n", i + 1, options[i].description);
        }
        printf("Please choose the measurement you wish to take: ");
        scanf("%d", &choice); // NOLINT(cert-err34-c)
        if (choice < 1 || choice > NUMBER_OF_OPTIONS) {
            printf("Invalid selection: %d. Please enter a value between 1 and %d.\n", choice, NUMBER_OF_OPTIONS);
        } else {
            options[choice - 1].function();
        }
    }
    return 0;
}


void fill_cache_with_junk(uint8_t *junk, long data_size) {
    for (long i = 0; i < data_size; i++)
        junk[i] = (uint8_t)i;
}


void measure_helper(int stride, uint8_t *start_of_data, int working_set_size) {
    struct timespec start, stop;
    register uint8_t *data = start_of_data;
    register uint8_t *end_of_data = start_of_data + working_set_size;
    uint8_t datum;
    register long number_of_iterations = (long)(1 << 30) * stride / working_set_size;
    // load the working set from the end to the start, so that the start of the data will be least-recently-used
    for (register int i = working_set_size - 1; i >= 0; i--) {
        data[i] = (uint8_t)i;
    }
    // caches are now primed, and pages should be in physical memory
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
    for (register long i = 0; i < number_of_iterations; i++) {
        for (data = start_of_data; data < end_of_data; data += stride) {
            datum = *data;
        }
    }
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);
    long duration = (stop.tv_sec - start.tv_sec) * 1000000000 + (stop.tv_nsec - start.tv_nsec);
    printf("Time spent working with data when working set is %d bytes: %.2f sec.\n",
           working_set_size, (double)duration / 1000000000);
}

void measure_cache_sizes() {
    printf("\nTrying differently-sized working sets to determine cache sizes.\n");
    register long data_size = MAX_WORKING_SET_SIZE;
    register int stride = 128;          // exceeds line size (shouldn't matter because of how we're going to load the cache)
    register uint8_t *data = malloc(data_size);
    for (register long working_set_size = stride; working_set_size <= (MAX_WORKING_SET_SIZE >> 1); working_set_size *= 2) {
        measure_helper(stride, data, working_set_size);
        measure_helper(stride, data, working_set_size + working_set_size / 4); // approximately +1dB
        measure_helper(stride, data, working_set_size + working_set_size / 2); // approximately +2dB
    }
    free(data);
    printf("\n");
}


void measure_cache_lines() {
    printf("\nTrying different strides to determine cache line size.\n");
    struct timespec start, stop;
    register int data_size = 1 << 20;   // 1 MB
    register int minimum_stride = 2;
    register uint8_t *data = malloc(data_size);
    register uint8_t *junk = malloc(data_size);
    register uint8_t *start_of_data = data;
    register uint8_t *end_of_data = start_of_data + data_size;
    register uint8_t datum;
    for (register int i = 0; i < data_size; i++) {
        data[i] = (uint8_t)i;
    }
    for (register int stride = minimum_stride; stride <= 2048; stride *= 2) {
        fill_cache_with_junk(junk, data_size);
        register int number_of_iterations = (1 << 11) * stride / minimum_stride;
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
        for (register int i = 0; i < number_of_iterations; i++) {
            for (data = start_of_data; data < end_of_data; data += stride) {
                datum = *data;
            }
        }
        clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);
        long duration = (stop.tv_sec - start.tv_sec) * 1000000000 + (stop.tv_nsec - start.tv_nsec);
        printf("Time spent working with data when stride is %d bytes: %.2f sec.\n", stride, (double)duration / 1000000000);
    }
    free(start_of_data);
    free(junk);
    printf("\n");
}


void terminate() {
    running = 0;
}
