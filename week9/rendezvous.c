#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>

sem_t *sem0, *sem1;

sem_t *sem_open_temp(const char *name, unsigned int value)
{
    sem_t *sem;

    // Create the semaphore
    if ((sem = sem_open(name, O_CREAT, 0666, value)) == SEM_FAILED)
        return SEM_FAILED;

    // Unlink it so it will go away after this process exits
    if (sem_unlink(name) == -1) {
        sem_close(sem);
        return SEM_FAILED;
    }

    return sem;
}

void *run(void *args)
{
    int *params = args;
    int thread_id = params[0];
    int sleep_time = params[1];

    printf("Thread %d: sleeping for %d seconds...\n", thread_id, sleep_time);
    sleep(sleep_time);

    printf("Thread %d: waiting for other thread...\n", thread_id);

    if (thread_id == 0) {
        sem_post(sem1);
        sem_wait(sem0);
    } else {
        sem_post(sem0);
        sem_wait(sem1);
    }

    printf("Thread %d: complete\n", thread_id);

    return NULL;
}

int main(void)
{
    int params[2][2] = {{0, 2}, {1, 5}};

    pthread_t t0, t1;

    sem0 = sem_open_temp("sem0", 0);
    sem1 = sem_open_temp("sem1", 0);

    pthread_create(&t0, NULL, run, params[0]);
    pthread_create(&t1, NULL, run, params[1]);

    pthread_join(t0, NULL);
    pthread_join(t1, NULL);

    sem_close(sem0);
    sem_close(sem1);
}

// If each thread waited before it posted, then they would be waiting indefinetely for the other thread to post. But since it's waiting, for the other one to post it won't. Deadlock basically.