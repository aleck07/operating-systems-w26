#include <stdio.h>
#include <pthread.h>

void *run(void *args)
{
    (void)args;
    puts("child: hello!");
    puts("child: goodbye!");
    return NULL;
}

int main(void)
{
    pthread_t t1; // thread identifier
    puts("parent: hello!");
    // Create a new thread
    pthread_create(&t1, NULL, run, NULL);

    // Wait for the child thread to complete
    pthread_join(t1, NULL);     
    puts("parent: goodbye!");
}