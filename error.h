#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <stdlib.h>

typedef int Error;

#define NONE               0x0000
#define IN_ERROR           0x0001
#define UNSUPORTED_PARAM   0x0002 // Parâmetros do programa incompletos
#define NO_COMMAND         0x0003 // Comando não suportado
#define MALLOC_ERROR       0x0004
#define FOPEN_ERROR        0x0005
#define STRCAT_ERROR       0x0006
#define CANT_REMOVE_FILE   0x0007 // O arquivo não pode ser removido
#define TABLE_EXISTS       0x0008 // A tabela já existe
#define UNSUPORTED_TYPE    0x0009 // Tipo de dado não é suportado
#define CANT_FIND_TABLE    0x000a // Tabela não foi encontrada
#define DIFF_PARAM_NUMB    0x000b // Número de parâmetros diferente
#define WRONG_SINTAX       0x000c // Sintax do comando errada
#define WRONG_VALUE        0x000d // Valor incompatível
#define WRONG_PARAMETER    0x000e // Parâmetro não reconhecido
#define FIELD_NOT_FOUND    0x000f // Campo não encontrado
#define NOT_INT            0x0010 // O valor não é um int
#define NOT_FLOAT          0x0011 // O valor não é um float
#define TODO               0x00f0
#define EXIT               0x00f1

void raiseError(Error error);

#endif /* ERROR_H */
