#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"
#include "tools.h"
#include "commands.h"

// Erro global de interpretação de comandos
extern int CMD_ERROR_CODE;

// Marca a posição corrente no buffer de comando
extern char *parsing;

// Buffers: Nome da tabela, nome do campo(chave) e valor
// Usados em comando com único parâmetro
extern char table_name[TABLE_NAME_MAX];
extern char field_name[FIELD_MAX];
extern char value[VALUE_MAX];

// Buffers: Tipos, nomes do campo(chave) e valores
// Usados em comando com mutiplos parâmetros
extern char type_name_arr[NUMBER_COLUMNS][TYPE_MAX];
extern char field_name_arr[NUMBER_COLUMNS][FIELD_MAX];
extern char value_arr[NUMBER_COLUMNS][VALUE_MAX];

// Index dos vetores a cima
extern int index_arr;

// Identifica o comando
// command: String com a linha de comando em questão
// Lógica básica: Procura o comando e pula o ponteiro para após o mesmo, lê o nome da tabela, outro parâmetro até o ':' e outro até o ';' ou fim, repete
int parser(char *command);

#endif /* PARSER_H */