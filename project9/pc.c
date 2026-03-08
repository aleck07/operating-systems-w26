#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include "eventbuf.h"

int num_events;

struct eventbuf *eb;

sem_t *mutex;
sem_t *items;
sem_t *spaces;

sem_t *sem_open_temp(const char *name, int value)
{
    sem_t *sem;

    // Create the semaphore
    if ((sem = sem_open(name, O_CREAT, 0600, value)) == SEM_FAILED)
        return SEM_FAILED;

    // Unlink it so it will go away after this process exits
    if (sem_unlink(name) == -1)
    {
        sem_close(sem);
        return SEM_FAILED;
    }

    return sem;
}

void *producer(void *arg)
{
    int id = *(int *)arg;

    for (int i = 0; i < num_events; i++)
    {
        sem_wait(spaces);
        sem_wait(mutex);

        int event = id * 100 + i;
        eventbuf_add(eb, event);
        printf("P%d: adding event %d\n", id, event);

        sem_post(mutex);
        sem_post(items);
    }

    printf("P%d: exiting\n", id);
    return NULL;
}

void *consumer(void *arg)
{
    int id = *(int *)arg;

    while (1)
    {
        sem_wait(items);
        sem_wait(mutex);

        if (eventbuf_empty(eb))
        {
            sem_post(mutex);
            break;
        }

        int event = eventbuf_get(eb);
        printf("C%d: got event %d\n", id, event);

        sem_post(mutex);
        sem_post(spaces);
    }

    printf("C%d: exiting\n", id);
    return NULL;
}

void run_threads(int num_producers, int num_consumers, pthread_t *producer_threads, pthread_t *consumer_threads, int *producer_ids, int *consumer_ids)
{
    for (int i = 0; i < num_producers; i++)
    {
        producer_ids[i] = i;
        pthread_create(&producer_threads[i], NULL, producer, &producer_ids[i]);
    }

    for (int i = 0; i < num_consumers; i++)
    {
        consumer_ids[i] = i;
        pthread_create(&consumer_threads[i], NULL, consumer, &consumer_ids[i]);
    }

    for (int i = 0; i < num_producers; i++)
    {
        pthread_join(producer_threads[i], NULL);
    }

    for (int i = 0; i < num_consumers; i++)
    {
        sem_post(items);
    }

    for (int i = 0; i < num_consumers; i++)
    {
        pthread_join(consumer_threads[i], NULL);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        fprintf(stderr, "Usage: %s <num_producers> <num_consumers> <num_events> <max_buffer_size>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int num_producers = atoi(argv[1]);
    int num_consumers = atoi(argv[2]);
    num_events = atoi(argv[3]);
    int max_outstanding = atoi(argv[4]);

    eb = eventbuf_create();

    mutex = sem_open_temp("/pc-mutex", 1);
    items = sem_open_temp("/pc-items", 0);
    spaces = sem_open_temp("/pc-spaces", max_outstanding);

    pthread_t *producer_threads = malloc(sizeof(pthread_t) * num_producers);
    pthread_t *consumer_threads = malloc(sizeof(pthread_t) * num_consumers);
    int *producer_ids = malloc(sizeof(int) * num_producers);
    int *consumer_ids = malloc(sizeof(int) * num_consumers);

    run_threads(num_producers, num_consumers, producer_threads, consumer_threads, producer_ids, consumer_ids);

    eventbuf_free(eb);

    sem_close(mutex);
    sem_close(items);
    sem_close(spaces);

    free(producer_threads);
    free(consumer_threads);
    free(producer_ids);
    free(consumer_ids);

    return 0;
}
