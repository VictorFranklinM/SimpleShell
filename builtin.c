#include "shell.h"

int shell_echo(char **args)
{
    int i = 1;

    while (args[i])
    {
        printf(Y"%s"RST, args[i]);
        if (args[i + 1])
            printf(" ");
        i++;
    }

    printf("\n");
    return 0;
}

int shell_exit(char **args)
{
    (void)args;
    printf(G"Saindo...\n"RST);
    exit(EXIT_SUCCESS);
    return 0;
}