#include <stdio.h>
#include <string.h>

int main(void)
{
    char line[1024];
    char *tokens[128] = {0};

    fgets(line, sizeof line, stdin);
    char *token = strtok(line, " \n");

    int j = 0;
    if (token != NULL) do {
        tokens[j] = token;
        j += 1;
    } while ((token = strtok(NULL, " \n")) != NULL);

    for (int i = 0; tokens[i] != NULL; i++)
        printf("%d: %s\n", i, tokens[i]);
}

// If we didn't initialize the whole array full of null terminators, the program will keep on tokenizing after our last word we typed in.