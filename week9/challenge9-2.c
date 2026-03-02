#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int count;

// Main thread: Launch thread#1
// Main thread: Launch thread#2
// Main thread: Set the value of a global to 6
// Thread#1   : Wait until the value of the global to get to 3
//            : When it does, print "Half way there!" and exit

// Thread#2   : Wait until the value of the global to get to 0
//            : When it does, print "Liftoff!" and exit
// Main thread: Loop, decrementing the value while it's greater than 0
//            : Broadcast to all the waiting threads.
//            : Sleep 1 second between decrements!
// Main thread: Join the two child threads

void *run1(void *arg)
{
    (void)arg;
    pthread_mutex_lock(&mutex);
    while (count > 3)
    {
        pthread_cond_wait(&cond, &mutex);
    }
    printf("Half way there!\n");
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void *run2(void *arg)
{
    (void)arg;
    pthread_mutex_lock(&mutex);
    while (count > 0)
    {
        pthread_cond_wait(&cond, &mutex);
    }
    printf("Liftoff!\n");
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main(void)
{
    pthread_t t1, t2;

    pthread_create(&t1, NULL, run1, NULL);
    pthread_create(&t2, NULL, run2, NULL);

    count = 6;
    while (count > 0)
    {
        count--;
        printf("%d\n", count);
        pthread_cond_broadcast(&cond);
        // When we use signal instead of broadcast, it prints halfway after the main thread prints 2, instead of 3.
        sleep(1);
    }

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    return 0;
}