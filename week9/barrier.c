#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>

#define THREAD_COUNT 10

sem_t *countSem, *barrierSem;
int count = 0;

sem_t *sem_open_temp(const char *name, unsigned int value)
{
    sem_t *sem;

    // Create the semaphore
    if ((sem = sem_open(name, O_CREAT, 0666, value)) == SEM_FAILED)
        return SEM_FAILED;

    // Unlink it so it will go away after this process exits
    if (sem_unlink(name) == -1)
    {
        sem_close(sem);
        return SEM_FAILED;
    }

    return sem;
}

void *run(void *args)
{
    int thread_id = *(int *)args;

    sem_wait(countSem);
    count++;
    printf("Thread %d: Waiting for turnstile\n", thread_id);

    if (count == THREAD_COUNT)
    {
        printf("Thread %d: Opening turnstile\n", thread_id);
        for (int i = 0; i < THREAD_COUNT; i++)
        {
            sem_post(barrierSem);
        }
    }
    sem_post(countSem);

    sem_wait(barrierSem);
    printf("Thread %d: In turnstile\n", thread_id);

    sem_wait(countSem);
    count--;
    sem_post(countSem);

    sleep(1);

    printf("Thread %d: Reopening turnstile\n", thread_id);
    printf("Thread %d: complete\n", thread_id);

    return NULL;
}

int main(void)
{
    pthread_t thread[THREAD_COUNT];
    int thread_id[THREAD_COUNT];

    // TODO: Add sem_open_temp() calls
    countSem = sem_open_temp("countSem", 1);
    barrierSem = sem_open_temp("barrierSem", 0);

    for (int i = 0; i < THREAD_COUNT; i++)
    {
        thread_id[i] = i;
        pthread_create(thread + i, NULL, run, thread_id + i);
    }

    for (int i = 0; i < THREAD_COUNT; i++)
        pthread_join(thread[i], NULL);

    printf("Count should be 0, and it is %d.\n", count);

    sem_close(countSem);
    sem_close(barrierSem);
}

// If we wanted the turnstile closed after the last thread passed through it, then we could check the thread count after the sleep and if it's zero, then we could close the turnstile.