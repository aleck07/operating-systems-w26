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