#include <stdio.h>
#include <pthread.h>

#define ELEMENTS 1000000
#define THREADS 4

int a[ELEMENTS] = {0};

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

volatile int spinlock1 = 0;

void lock(void)
{
    while (__sync_lock_test_and_set(&spinlock1, 1))
        ;
}

void unlock(void)
{
    __sync_synchronize(); // Memory barrier.
    spinlock1 = 0;
}

void *run(void *arg)
{
    (void)arg;

    lock();
    for (int i = 0; i < ELEMENTS; i++)
    {
        a[i]++;
    }
    unlock();
    // No more parrallelization.

    return NULL;
}

// When adding the mutexes inside the loop, it takaes around 0.5 seconds to run. But when I moved them outside the loop. It's around 0.01 seconds to run. Which is magnitudes faster. When we lock inside of the loop, each thread still needs to loop through the entire array while locking and unlocking within each iteration. Which is very time consuming.
// When using spinlocks, it seemed a tad slower outside the loop, but isn't anything to bad. Maybe around 0.2 seconds. And putting them inside the loop was even slower. Around a full second. It makes sense that spinlocks are slower because they are busy waiting.

int main(void)
{
    pthread_t thread[THREADS];

    for (int i = 0; i < THREADS; i++)
        pthread_create(thread + i, NULL, run, NULL);

    for (int i = 0; i < THREADS; i++)
        pthread_join(thread[i], NULL);

    int expected = THREADS;

    for (int i = 0; i < ELEMENTS; i++)
        if (a[i] != expected)
            printf("a[%d] = %d, expected %d\n", i, a[i], expected);
}