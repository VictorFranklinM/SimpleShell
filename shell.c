#include "shell.h"

void prompt_user(void)
{
    char current_dir[BUFSIZ];

    if (NULL == getcwd(current_dir, sizeof(current_dir)))
        perror(RED"Não conseguiu encontrar o diretório atual"RST);
    printf(C"%s"RST"$ ", current_dir);
}

char *shell_read_line(void)
{
    char *buffer = NULL;
    size_t bufsize = 0;

    prompt_user();

    if (getline(&buffer, &bufsize, stdin) == -1) // Checa se houve algum erro ou se o fim do arquivo foi alcançado
    {
        if (feof(stdin))
            printf(RED"[Fim do arquivo]\n"RST);
        else
            printf(RED"Erro no getline\n"RST);

        free(buffer);
        return NULL;
    }
    
    return buffer;
}

int main(int arg_counter, char **arg_vector)
{
    char *line;

    // REPL
    // Ler -> Avaliar -> Imprimir/Executar -> Loop

    while (1)
    {
        // 1) Leitura
        line = shell_read_line();

        if(line == NULL)
            break;

        printf("%s\n", line);

        free(line);

        // 2) Get token
        // Talvez trocar pra lexing e parsing

        // Exec

    }

    return EXIT_SUCCESS;
}