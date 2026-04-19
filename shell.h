#ifndef SHELL_H
#define shell_h

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

/*
** Cores em ANSI para formatar as saídas do terminal
**
** Y - Amarelo
** G - Verde
** C - Ciano
** RED - Vermelho
** RST - Reset
**
*/

#define Y "\033[1;33m"
#define G "\033[1;32m"
#define C "\033[1;36m"
#define RED "\033[1;31m"
#define RST "\033[0m"

#define DEL "\n\t \v\f\r"

#endif