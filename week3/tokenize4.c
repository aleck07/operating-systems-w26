#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
    char line[1024];
    char *tokens[128] = {0};

    printf("prompt> ");
    fflush(stdout); // flushes output

    fgets(line, sizeof line, stdin);
    char *token = strtok(line, " \n");

    int i = 0;
    while (token != NULL) {
        tokens[i++] = token;
        token = strtok(NULL, " \n");
    }

    if (tokens[0] != NULL) {
        execvp(tokens[0], tokens);
        perror("Invalid command");
    }
}

// When you enter random characters, execvp returns a status error. The file can't be found. We can have it print in error by using perror as shown above.