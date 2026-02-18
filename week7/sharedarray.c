#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define ARRAY_SIZE 100000
#define THREADS 10

int big_ol_array[ARRAY_SIZE] = {0};

void *run(void *args)
{
    (void)args;
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        big_ol_array[i]++;
    }
    return NULL;
}

int main(void)
{
    pthread_t thread[THREADS];
    int failed = 0;

    for (int i = 0; i < THREADS; i++)
    {
        pthread_create(thread + i, NULL, run, NULL);
    }

    for (int i = 0; i < THREADS; i++)
    {
        pthread_join(thread[i], NULL);
    }

    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        if (big_ol_array[i] != THREADS)
        {
            failed++;
            printf("Error at index %d: expected %d, got %d\n", i, THREADS, big_ol_array[i]);
        }
    }

    printf("Finished checking array. Errors: %d/%d\n", failed, ARRAY_SIZE);

    return 0;
}

// The reason why the output is not always the same is because of the race condition. When the threads are reading the value of i and incrementing it, they are having to do 7 instructions and with such a large array there will be concurrency issues.