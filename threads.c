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

void *producer(void *param)
{
	buffer_item item;

	while (true) {
		sleep(rand() % 5 + 1); // picking a random number to sleep (arbitrary)
		item = rand();
		if (insert_item(item))
			fprintf(stderr, "producer error condition\n");
		else
			printf("producer produced %d\n", item);
	}
	return NULL;
}

void *consumer(void *param)
{
	buffer_item item;
    
	while (true) {
		sleep(rand() % 5 + 1); // picking a random number to sleep 
		if (remove_item(&item))
			fprintf(stderr, "consumer error condition\n");
		else
			printf("consumer consumed %d\n", item);
	}
	return NULL;

// skeleton for inserting and removing taken from class textbook (Operating System Concepts by Silberschatz, Galvin, and Gagne)
// around section 7.11
int insert_item(buffer_item item) {
    //Wait for an empty slot
    if (sem_wait(&empty) != 0) {
        return -1;
    }

    //Lock the buffer
    if (pthread_mutex_lock(&mutex) != 0) {
        sem_post(&empty);   //give the slot back since we didn't use it
        return -1;
    }

    //Critical section: add the item and advance the circular index
    buffer[in] = item;
    in = (in + 1) % BUFFER_SIZE;

    //Unlock the buffer
    if (pthread_mutex_unlock(&mutex) != 0) {
        return -1;
    }

    //Signal that there is one more full slot
    if (sem_post(&full) != 0) {
        return -1;
    }

    return 0;
}

int remove_item(buffer_item *item) {
    //Wait for a full slot
    if (sem_wait(&full) != 0) {
        return -1;
    }

    //Lock the buffer
    if (pthread_mutex_lock(&mutex) != 0) {
        sem_post(&full);    //give it back since we didn't consume
        return -1;
    }

    //Critical section: take the item and advance the circular index
    *item = buffer[out];
    out = (out + 1) % BUFFER_SIZE;

    //Unlock the buffer
    if (pthread_mutex_unlock(&mutex) != 0) {
        return -1;
    }

    //Signal that there is one more empty slot
    if (sem_post(&empty) != 0) {
        return -1;
    }

    return 0;
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
	// reserve enough memory for "n" ids
	pthread_t *producers = malloc(num_producers * sizeof(pthread_t));
	pthread_t *consumers = malloc(num_consumers * sizeof(pthread_t));
	if (!producers || !consumers) {
		fprintf(stderr, "malloc failed\n");
		return 1;
	}
	// seeding random numbers
	srand(time(NULL));

	for (int i = 0; i < num_producers; i++) {
		if (pthread_create(&producers[i], NULL, producer, NULL)) {
			fprintf(stderr, "pthread_create failed\n");
			return 1;
		}
	}

	/* 4. Create consumer thread(s) */
	for (int i = 0; i < num_consumers; i++) {
		if (pthread_create(&consumers[i], NULL, consumer, NULL)) {
			fprintf(stderr, "pthread_create failed\n");
			return 1;
		}
	}

    /* 5. Sleep */
    sleep(sleep_time);

    /* 6. Exit */
    return 0;
}
