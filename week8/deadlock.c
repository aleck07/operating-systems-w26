#include <stdio.h>
#include <pthread.h>

#define ELEMENTS 50
#define THREADS 8

pthread_mutex_t lock_a = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t lock_b = PTHREAD_MUTEX_INITIALIZER;

int a[ELEMENTS] = {0};
int b[ELEMENTS] = {0};

void *run1(void *arg)
{
    (void)arg;

    for (int i = 0; i < ELEMENTS; i++)
    {
        pthread_mutex_lock(&lock_b);
        pthread_mutex_lock(&lock_a);
        a[i]++;
        pthread_mutex_unlock(&lock_a);
        pthread_mutex_unlock(&lock_b);
    }

    return NULL;
}

void *run2(void *arg)
{
    (void)arg;

    for (int i = 0; i < ELEMENTS; i++)
    {
        pthread_mutex_lock(&lock_b);
        pthread_mutex_lock(&lock_a);
        a[i]++;
        pthread_mutex_unlock(&lock_a);
        pthread_mutex_unlock(&lock_b);
    }

    return NULL;
}

// This program will deadlock because run1 locks lock_a and then lock_b, while run2 locks lock_b and then lock_a. If run1 locks lock_a and run2 locks lock_b at the same time, they will both be waiting for each other to release the locks, resulting in a deadlock.

int main(void)
{
    pthread_t t1, t2;

    pthread_create(&t1, NULL, run1, NULL);
    pthread_create(&t2, NULL, run2, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
}
