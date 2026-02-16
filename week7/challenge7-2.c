#include <stdio.h>
#include <pthread.h>

struct message {
    char *hello;
    char *goodbye;
};

void *run(void *args)
{
    struct message *msg = (struct message *)args;
    printf("child: %s\n", msg->hello);
    printf("child: %s\n", msg->goodbye);
    return NULL;
}

int main(void)
{
    pthread_t t1; // thread identifier
    char *helloMessage = "hello world, from child!!";
    char *goodbyeMessage = "goodbye world, from child!!";
    
    struct message msg = {helloMessage, goodbyeMessage};

    puts("parent: hello!");
    // Create a new thread
    pthread_create(&t1, NULL, run, &msg);

    // Wait for the child thread to complete
    pthread_join(t1, NULL);     
    puts("parent: goodbye!");
}