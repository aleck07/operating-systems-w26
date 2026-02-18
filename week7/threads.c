#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>

#define THREADS 10

void *run(void *args)
{
    int *id = args;
    printf("running thread %d!\n", *id);
    return NULL;
}

int main(void)
{
    pthread_t thread[THREADS];
    int thread_id[THREADS];

    for (int i = 0; i < THREADS; i++)
    {
        thread_id[i] = i;
        pthread_create(thread + i, NULL, run, (void *)&thread_id[i]);
    }

    for (int i = 0; i < THREADS; i++)
    {
        pthread_join(thread[i], NULL);
    }

    puts("Main thread complete!");
}