#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE 2048
#define MAX_TOKENS 128

int main(void){
    while(1){
        char line[MAX_LINE];
        char *tokens[MAX_TOKENS] = {0};

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
}
