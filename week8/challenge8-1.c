#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define THREAD_COUNT 4

int shared_value = 1;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *run(void *arg)
{
    (void)arg;
    
    pthread_mutex_lock(&mutex);
    if (shared_value > 0) {
        usleep(1);
        shared_value--;
    }
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main(void)
{
    pthread_t threads[THREAD_COUNT];

    for (int i = 0; i < THREAD_COUNT; i++)
        pthread_create(threads + i, NULL, run, NULL);

    for (int i = 0; i < THREAD_COUNT; i++)
        pthread_join(threads[i], NULL);

    if (shared_value < 0)
        printf("How'd shared_value get to be %d?\n", shared_value);
    else
        puts("OK");
}
