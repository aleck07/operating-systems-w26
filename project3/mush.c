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
        // Check if a path is provided
        if(tokens[1] == NULL){
            fprintf(stderr, "Invalid use: cd requires a path argument\n");
            return;
        }
        if(chdir(tokens[1]) == -1) perror("Invalid use: ");
        return;
    }
    // Check if command is `exit`
    if(strcmp(tokens[0], "exit") == 0){
        exit(0);
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
    tokens[i] = NULL;
}

int main(void){
    while(1){
        char line[MAX_LINE];
        char *tokens[MAX_TOKENS] = {0};

        printf("prompt> ");
        fflush(stdout); // flushes output
        
        // Read user input
        fgets(line, sizeof line, stdin);

        if(feof(stdin)) exit(0);

        getTokens(line, tokens);
        if (tokens[0] != NULL) {
            runCommand(tokens);
        }
    }
    return 0;
}
