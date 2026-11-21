#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "buffer.h"

int main(int argc, char *argv[]) { 
    //Error handling for a bad or missing command line arguments
    if (argc != 4) {
        fprintf(stderr, "usage: %s <sleep_time> <num_producers> <num_consumers>\n", argv[0]);
        return 1;
    } 
    /* 1. Get command line arguments  argv[1],argv[2],argv[3] */ 
    int sleep_time = atoi(argv[1]);
    int num_producers = atoi(argv[2]);
    int num_consumers = atoi(argv[3]);
    
    if (sleep_time <= 0 || num_producers <= 0 || num_consumers <= 0) {
        fprintf(stderr, "usage: %s <sleep_time> <num_producers> <num_consumers>\n", argv[0]);
        return 1;
    } else{
        //If arguments are correct test
        //To be removed later
        printf("Testing command line arguments: sleep_time=%d num_producers=%d num_consumers=%d\n", 
            sleep_time, num_producers, num_consumers);
    }

    /* 2. Initialize buffer */
    /* 3. Create producer thread(s) */
    /* 4. Create consumer thread(s) */

    /* 5. Sleep */
    sleep(sleep_time);

    /* 6. Exit */
    return 0;
}