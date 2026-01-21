#include <stdio.h>
#include <string.h>

int main(void)
{
    char line[1024];

    fgets(line, sizeof line, stdin);

    char* token = strtok(line, " \n");
    int i = 0;

    if (token != NULL) do {
        printf("%d: %s\n", i, token);      
        i += 1;
    } while ((token = strtok(NULL, " \n")) != NULL);
}

// If we didn't fix the new line issue in the strtok argument, we could instead replace the new line character on our input string and replace it with a null terminator.