#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 10

typedef struct {
    int buffer[BUFFER_SIZE];
    int in;
    int out;
} BoundedBuffer;

BoundedBuffer bb;
sem_t empty, full;
pthread_mutex_t mutex;

void display_buffer() {
    printf("Buffer: ");
    for (int i = 0; i < BUFFER_SIZE; i++) {
        printf("%-5d ", bb.buffer[i]);
    }
    printf("\n\n");
}

void* producer(void* arg) {
    int producer_id = *(int*)arg;
    while (1) {
        int item = rand() % 1000;

        sem_wait(&empty);
        pthread_mutex_lock(&mutex);

        bb.buffer[bb.in] = item;
        bb.in = (bb.in + 1) % BUFFER_SIZE;

        printf("Producer %d produced: %d\n", producer_id, item);
        display_buffer();

        pthread_mutex_unlock(&mutex);
        sem_post(&full);

        sleep(rand() % 5);
    }
}

void* consumer(void* arg) {
    int consumer_id = *(int*)arg;
    while (1) {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);

        int item = bb.buffer[bb.out];
        bb.buffer[bb.out] = 0;
        bb.out = (bb.out + 1) % BUFFER_SIZE;

        printf("Consumer %d consumed: %d\n", consumer_id, item);
        display_buffer();

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);

        sleep(rand() % 5);
    }
}

int main(int argc, char *argv[]) {
    
    if(argc != 4)
    {
        printf("Usage: <sleep_time> <producer_number> <consumer_number>\n");
        exit(EXIT_FAILURE);
    }

    int sleep_time = atoi(argv[1]);
    int producer_number = atoi(argv[2]);
    int consumer_number = atoi(argv[3]);

    pthread_t producer_threads[producer_number], consumer_threads[consumer_number];
    int producer_ids[producer_number], consumer_ids[consumer_number];

    bb.in = 0;
    bb.out = 0;
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < producer_number; i++) {
        producer_ids[i] = i + 1;
        pthread_create(&producer_threads[i], NULL, producer, &producer_ids[i]);
    }

    for (int i = 0; i < consumer_number; i++) {
        consumer_ids[i] = i + 1;
        pthread_create(&consumer_threads[i], NULL, consumer, &consumer_ids[i]);
    }

    sleep(sleep_time);

    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}