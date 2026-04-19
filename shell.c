#include "shell.h"

type_builtin global_builtin[] =
{
    {.builtin_name = "echo", .foo = shell_echo},
    {.builtin_name = "exit", .foo = shell_exit},
    {.builtin_name = NULL}
};

int status = 0;

void prompt_user(void)
{
    char current_dir[BUFSIZ];

    if (NULL == getcwd(current_dir, sizeof(current_dir)))
        perror(RED"Não conseguiu encontrar o diretório atual"RST);
    printf(G"[%d]"C"%s"RST"$ ", status, current_dir);
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
    char **tokens = malloc((bufsiz+1) * sizeof(char*));

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

            char **temp = realloc(tokens, (bufsiz+1) * sizeof(char*));
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

void shell_launch(char **args)
{
    pid_t pid = fork();

    if (pid < 0)
    {
        perror(RED"Falha no fork"RST);
        exit(EXIT_FAILURE);
    }

    if (pid == CHILD)
    {
        if (!args || !args[0])
        {
            fprintf(stderr, RED"execvp: argumentos invalidos\n"RST);
            exit(EXIT_FAILURE);
        }

        execvp(args[0], args);

        fprintf(stderr, RED"%s: comando não encontrado\n"RST, args[0]);
        exit(127);
    }
    else
    {
        if (wait(&status) == -1)
        {
            perror(RED"Falha no wait"RST);
            return;
        }

        if (WIFEXITED(status))
            status = WEXITSTATUS(status);
    }
}

// Se estiver implementado no proprio shell, executa
// Senao, fork -> exec -> wait
void shell_exec(char **args)
{
    if (!args || !args[0])
        return;

    unsigned int i = 0;
    const char *curr;

    while ((curr = global_builtin[i].builtin_name))
    {
        if (!strcmp(curr, args[0]))
        {
            status = global_builtin[i].foo(args);
            return;
        }
        ++i;
    }

    shell_launch(args); // fork -> exec
}

int main(int arg_counter, char **arg_vector)
{
    char *line;
    char **args;

    // REPL
    // Ler -> Avaliar -> Imprimir/Executar -> Loop

    // 1) Leitura e Loop
    while ((line = shell_read_line()) != NULL)
    {

        // 2) Get token
        // Talvez trocar pra lexing e parsing
        args = shell_split_line(line);

        // 3) Executar
        shell_exec(args);

        // 4) Liberar memória
        free(args);
        free(line);
    }

    return EXIT_SUCCESS;
}