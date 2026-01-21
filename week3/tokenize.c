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

// We pass the pointer the first time but not the second time to the string because when call it the first time, the function stores the pointer to the string and remembers where it placed the null terminators.
// If we kept on passing the pointer, we would keep on getting the first token over and over forever.