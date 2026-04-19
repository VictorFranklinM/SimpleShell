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

// ls -la file
//  -> [ls]
//  -> [-la]
//  -> [file]
char **shell_split_line(char *line)
{
    size_t bufsiz = BUFSIZ;
    char **tokens = malloc(bufsiz * sizeof(char*));

    if (tokens == NULL)
    {
        perror(RED"Falha no malloc"RST);
        exit(EXIT_FAILURE);
    }

    unsigned int position = 0;

    for (char *token = strtok(line, DEL); token; token = strtok(NULL, DEL))
    {
        // Se tem mais argumentos que o tamanho do vetor
        if (position >= bufsiz)
        {
            bufsiz *= 2;

            char **temp = realloc(tokens, bufsiz * sizeof(char*));
            if (temp == NULL)
            {
                free(tokens);
                perror(RED"Falha no realloc"RST);
                exit(EXIT_FAILURE);
            }

            tokens = temp;
        }

        tokens[position++] = token;
    }

    tokens[position] = NULL; // Fim do comando
    return tokens;
}

int main(int arg_counter, char **arg_vector)
{
    char *line;
    char **args;

    // REPL
    // Ler -> Avaliar -> Imprimir/Executar -> Loop

    // 1) Leitura e Loop
    while (line = shell_read_line())
    {
        printf("%s\n", line);

        // 2) Get token
        // Talvez trocar pra lexing e parsing
        args = shell_split_line(line);

        // Exec
        free(args);
        free(line);
    }

    return EXIT_SUCCESS;
}