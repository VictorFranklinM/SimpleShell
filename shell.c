#include "shell.h"

int main(int arg_counter, char **arg_vector)
{
    (void)arg_counter;
    int status;

    // processo filho
    if (fork() == 0){
        execvp(arg_vector[1], arg_vector+1);
    }

    wait(&status);

    return EXIT_SUCCESS;
}