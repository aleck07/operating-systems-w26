#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE 2048
#define MAX_TOKENS 128

void runCommand(char **tokens){
    // Check if command is 'cd'
    if(strcmp(tokens[0], "cd") == 0){
        if(chdir(tokens[1]) == -1) perror("Invalid use: ");
        return;
    }

    pid_t pid = fork();

    // Child process
    if(pid == 0){
        execvp(tokens[0], tokens);
        perror("Invalid command: ");
        exit(1);
    }
    wait(NULL);
    
}

void getTokens(char *line, char **tokens){
    char *token = strtok(line, " \n");

    // Tokenize input line
    int i = 0;
    while (token != NULL) {
        tokens[i++] = token;
        token = strtok(NULL, " \n");
    }
}

int main(void){
    while(1){
        char line[MAX_LINE];
        char *tokens[MAX_TOKENS] = {0};

        printf("prompt> ");
        fflush(stdout); // flushes output
        
        // Read user input
        fgets(line, sizeof line, stdin);

        getTokens(line, tokens);
        if (tokens[0] != NULL) {
            runCommand(tokens);
        }
    }
}
