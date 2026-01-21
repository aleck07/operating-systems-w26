#include <stdio.h>
#include <unistd.h>

int main(void)
{
    printf("And...\n");

    fork();

    printf("here we go!\n");
}
