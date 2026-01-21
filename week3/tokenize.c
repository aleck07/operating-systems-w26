#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    (void)argc; // stop compiler warnings about ununsed variables

    printf("The first argument is: \"%s\"\n", argv[1]); // just to demo
}