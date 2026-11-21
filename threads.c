#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "buffer.h"

//Implementing the circular buffer
buffer_item buffer[BUFFER_SIZE];
int in = 0;
int out = 0;

//Implementing unnamed semaphores for empty and full
sem_t empty;
sem_t full;

//Implementing mutex lock instead of binary semaphore
pthread_mutex_t mutex;

//Initializing the init_buffer function
int init_buffer(void) {
    //Mutex for mutual exclusion on shared buffer state
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        perror("pthread_mutex not initialized"); 
        return -1;
    }

    //Initializing the unnamed semaphores to track available empty slots
    if (sem_init(&empty, 0, BUFFER_SIZE) != 0) {
        perror("sem_init empty");
        return -1;
    }

    //Initialize unnamed semaphore to track available filled slots
    if (sem_init(&full, 0, 0) != 0) {
        perror("sem_init full");
        return -1;
    }

    //Indices start at 0
    in = 0;
    out = 0;
    return 0;
}

void *producer(void *param) { 
   buffer item item; 
   while (true) { 
      /* sleep for a random period of time */ 
      sleep(...); 
      /* generate a random number */ 
      item = rand(); 
      if (insert item(item)) 
         fprintf("report error condition"); 
      else 
         printf("producer produced %d\n",item); 
   } 
} 

void *consumer(void *param) { 

   buffer item item; 
   while (true) { 
      /* sleep for a random period of time */ 
      sleep(...); 
      if (remove item(&item)) 
         fprintf("report error condition"); 
      else 
         printf("consumer consumed %d\n",item); 

   } 
} 

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
    if (init_buffer() != 0)
        return 1;

    /* 3. Create producer thread(s) */
    /* 4. Create consumer thread(s) */

    /* 5. Sleep */
    sleep(sleep_time);

    /* 6. Exit */
    return 0;
}
