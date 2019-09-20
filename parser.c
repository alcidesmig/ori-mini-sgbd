#include "parser.h"

char validateType(char *type) {
    if (!strncmp(type, INT, TYPE_LIMIT)) {
        return 'i';
    } else if (!strncmp(type, STR, TYPE_LIMIT)) {
        return 's';
    } else if (!strncmp(type, FLT,TYPE_LIMIT)) {
        return 'f';
    } else if (!strncmp(type, BIN, TYPE_LIMIT)) {
        return 'b';
    } else {
        return '\0';
    }
}

ParsedData *parser(char * line) {
    ParsedData *pData = (ParsedData *)safe_malloc(sizeof(ParsedData));

    // Ponteiro para o comando
    char **cmd = &(pData->command);

    // Salva o comando
    *cmd = strtok(line, " ");
    toUpperCase(*cmd);

    // Identifica o comando
    if (!strncmp(*cmd, CT, CMD_LIMIT)) {
        // Ponteiro para a tabela
        Table *table = &(pData->data.table);
        // Ponteiro para os tipos
        TypeArr *types = &(table->types);
        // Ponteiro para os campos
        FieldArr *fields = &(table->fields);
        // Ponteiro para o número de colunas
        int *cols = &(table->cols);

        // Auxiliar para guardar o tipo e o nome das colunas
        char *aux[NUMBER_COLUMNS_LIMIT];
        // Ponteiro auxiliar, pega o nome da tabela
        char *ptr = strtok(NULL, " ");

        // Verifica se há nome
        if (!ptr) {
            fprintf(stderr, "Erro de sintax.\n");
            return NULL;
        }

        // Salva o nome
        table->name = ptr;
        // Seta o número de rows
        table->rows = 0;
        // Seta o número de colunas
        *cols = 0;
        
        // Pega o bloco de informações de uma coluna
        ptr = strtok(NULL, ";");
        
        while (ptr) {
            // Salva o bloco no vetor auxiliar
            aux[*cols] = ptr;
            // Incrementa o número de colunas
            (*cols)++;
            // Pega o próximo bloco
            ptr = strtok(NULL, ";");
        }

        // Separa os blocos de informação das colunas
        for (int i = 0; i < *cols; i++) {
            // Pega o tipo
            ptr = strtok(aux[i], ":");
            toUpperCase(ptr);
            // Valida o tipo
            char c = validateType(ptr);
            if (!c) {
                fprintf(stderr, "Erro de sintax.\n");
                return NULL;
            }
            // Salva o tipo
            (*types)[i] = c;

            // Pega o nome do campo
            ptr = strtok(NULL, "\0");
            // Verifica o nome
            if (!ptr) {
                fprintf(stderr, "Erro de sintax.\n");
                return NULL;
            }
            // Salva o nome
            if(replaceSpace(ptr, '_')) {
                printf("Espaços foram substituídos no nome do campo: %s.\n", ptr);
            }
            (*fields)[i] = ptr;
        }
    } else {
        fprintf(stderr, "Comando não reconhecido.\n");
        return NULL;
    }

    return pData;
}