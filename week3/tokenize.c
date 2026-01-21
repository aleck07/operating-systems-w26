#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
    (void)argc; // stop compiler warnings about ununsed variables

    char* token = strtok(argv[1], " ");
    int i = 0;

    if (token != NULL) do {
        printf("%d: %s\n", i, token);      
        i += 1;
    } while ((token = strtok(NULL, " ")) != NULL);
}