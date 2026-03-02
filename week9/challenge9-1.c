#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int count = 0;

void *run1(void *arg)
{
    (void)arg;
    sleep(0.1);
    while (count < 5) {
        pthread_mutex_lock(&mutex);
        count++;
        printf("Thread 1: count is %d\n", count);
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }

    return NULL;
}

void *run2(void *arg)
{
    (void)arg;
    // sleep(3);
    // When we add sleep(3), Thread 2 will wait until the count is 4 before printing. Thread 1 signaled thread 2, but thread 2 was still sleeping so it missed those signals. After the sleep was over, it recieved the signal and printed out count is 4 when thread 1 printed count was 4.

    pthread_mutex_lock(&mutex);
    while (count < 5) {
        pthread_cond_wait(&cond, &mutex);
        printf("Thread 2: count is %d\n", count);
    }
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main(void)
{
    pthread_t t1;
    pthread_t t2;

    pthread_create(&t1, NULL, run1, NULL);
    pthread_create(&t2, NULL, run2, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
}