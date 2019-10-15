#include "parser.h"

// Transforma uma linha de comando em uma struct com os dados
// line: linha de comando
// return: ponteiro para a struct com os dados
ParsedData *parser(char * line) {
    ParsedData *pData = (ParsedData *) mallocSafe(sizeof(ParsedData));

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
        // Tamanho do registro da tabela
        int *length = &(table->length);

        // Auxiliar para guardar o tipo e o nome das colunas
        char *aux[NUMBER_COLUMNS_LIMIT];
        // Ponteiro auxiliar, pega o nome da tabela
        char *ptr = strtok(NULL, " ");

        // Verifica se há nome
        if (!ptr) {
            fprintf(stderr, "Nome da tabela não encontrado.\n");
            return NULL;
        }

        // Verifica o tamanho do nome
        if (strlen(ptr) >= TABLE_NAME_LIMIT) {
            fprintf(stderr, "Nome da tabela é muito longo.\n");
            return NULL;
        }

        // Salva o nome
        strncpy(table->name, ptr, TABLE_NAME_LIMIT);
        // Seta o número de rows
        table->rows = 0;
        // Seta o número de colunas
        *cols = 0;
        // Seta o tamanho de um registro
        *length = 0;
        
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

        // Verifica se há pelo menos uma coluna
        if (!(*cols)) {
            fprintf(stderr, "Nenhuma coluna foi encontrada.\n");
            return NULL;
        }

        // Separa os blocos de informação das colunas
        for (int i = 0; i < *cols; i++) {
            // Pega o tipo
            ptr = strtok(aux[i], ":");
            toUpperCase(ptr);
            // Valida o tipo
            char c = validateType(ptr);
            if (!c) {
                fprintf(stderr, "Tipo de dado não suportado.\n");
                return NULL;
            } else if (c == 'i') {
                *length += sizeof(int);
            } else if (c == 's') {
                *length += sizeof(long int);
            } else if (c == 'f') {
                *length += sizeof(float);
            } else if (c == 'b') {
                *length += sizeof(long int);
            }

            // Salva o tipo
            (*types)[i] = c;

            // Pega o nome do campo
            ptr = strtok(NULL, "\0");

            // Verifica o nome
            if (!ptr) {
                fprintf(stderr, "O nome do campo não foi encontrado.\n");
                return NULL;
            }

            // Verifica o tamanho do dome do campo
            if (strlen(ptr) >= FIELD_NAME_LIMIT) {
                fprintf(stderr, "O nome do campo é muito longo.\n");
                return NULL;
            }

            // Salva o nome
            if(replaceSpace(ptr, '_')) {
                printf("Espaços foram substituídos no nome do campo: %s.\n", ptr);
            }
            strncpy((*fields)[i], ptr, FIELD_NAME_LIMIT);
        }
    } else if (!strncmp(*cmd, RT, CMD_LIMIT) || !strncmp(*cmd, AT, CMD_LIMIT)) {

        // Ponteiro para a tabela
        Table *table = &(pData->data.table);

        // Ponteiro auxiliar, pega o nome da tabela
        char *ptr = strtok(NULL, " ");

        // Verifica se há nome
        if (!ptr) {
            fprintf(stderr, "O nome da tabela não foi encontrado.\n");
            return NULL;
        }

        // Verifica o tamanho do nome
        if (strlen(ptr) >= TABLE_NAME_LIMIT) {
            fprintf(stderr, "Nome da tabela é muito longo.\n");
            return NULL;
        }

        // Salva o nome
        strncpy(table->name, ptr, TABLE_NAME_LIMIT);

        // Verifica se há mais na linha
        ptr = strtok(NULL, "\0");
        if (ptr) {
            fprintf(stderr, "A estrutura do comando não foi reconhecida.\n");
            return NULL;
        }
    } else if (!strncmp(*cmd, LT, CMD_LIMIT)) {
        // Ponteiro auxiliar
        char *ptr = strtok(NULL, "\0");

        // Verifica se há mais na linha
        if (ptr) {
            fprintf(stderr, "A estrutura do comando não foi reconhecida.\n");
            return NULL;
        }
    } else if (!strncmp(*cmd, IR, CMD_LIMIT)) {
        // Ponteiro para o registro
        Row *row = &(pData->data.row);
        // Ponteiro para os valores
        ValueArr *values = &(row->values);
        // Ponteiro para o número de colunas
        int *cols = &(row->cols);

        // Ponteiro auxiliar, pega o nome da tabela
        char *ptr = strtok(NULL, " ");

        // Verifica se há nome
        if (!ptr) {
            fprintf(stderr, "O nome da tabela não foi encontrado.\n");
            return NULL;
        }

        // Verifica o tamanho do nome
        if (strlen(ptr) >= TABLE_NAME_LIMIT) {
            fprintf(stderr, "Nome da tabela é muito longo.\n");
            return NULL;
        }

        // Salva o nome
        strncpy(row->tableName, ptr, TABLE_NAME_LIMIT);
        // Seta o número de colunas
        *cols = 0;
        
        // Pega um valor
        ptr = strtok(NULL, ";");
        
        while (ptr) {
            // Salva o bloco no vetor de valores
            (*values)[*cols] = (void *)ptr;
            // Incrementa o número de colunas
            (*cols)++;
            // Pega o próximo bloco
            ptr = strtok(NULL, ";");
        }

        // Verifica se há pelo menos uma coluna
        if (!(*cols)) {
            fprintf(stderr, "Nenhuma coluna foi encontrada.\n");
            return NULL;
        }
    } else if (!strncmp(*cmd, BR, CMD_LIMIT)) {
        // Ponteiro para a seleção
        Selection *selection = &(pData->data.selection);

        // Ponteiro auxiliar, pega o parâmetro
        char *ptr = strtok(NULL, " ");

        // Verifica se há parâmetro
        if (!ptr) {
            fprintf(stderr, "O parâmetro não foi encontrado.\n");
            return NULL;
        }

        // Verifica o parâmetro
        toUpperCase(ptr);

        if (strncmp(ptr, U, PARAMETER_LIMIT) && strncmp(ptr, N, PARAMETER_LIMIT)) {
            fprintf(stderr, "O parâmetro não foi reconhecido\n");
            return NULL;
        }

        // Salva o parâmetro
        selection->parameter = ptr[0];
        
        // Pega o nome da tabela
        ptr = strtok(NULL, " ");
        
        // Verifica se há nome da tabela
        if (!ptr) {
            fprintf(stderr, "O nome da tabela não foi encontrado.\n");
            return NULL;
        }

        // Verifica o tamanho do nome
        if (strlen(ptr) >= TABLE_NAME_LIMIT) {
            fprintf(stderr, "Nome da tabela é muito longo.\n");
            return NULL;
        }

        // Salva o nome da tabela
        strncpy(selection->tableName, ptr, TABLE_NAME_LIMIT);

        // Pega o nome do campo
        ptr = strtok(NULL, ":");

        // Verifica se há nome do campo
        if (!ptr) {
            fprintf(stderr, "O nome do campo não foi encontrado.\n");
            return NULL;
        }

        // Verifica o tamanho do nome do campo
        if (strlen(ptr) >= FIELD_NAME_LIMIT) {
            fprintf(stderr, "Nome do campo é muito longo.\n");
            return NULL;
        }

        // Salva o nome do campo
        if(replaceSpace(ptr, '_')) {
            printf("Espaços foram substituídos no nome do campo: %s.\n", ptr);
        }
        strncpy(selection->field, ptr, FIELD_NAME_LIMIT);

        // Pega o valor
        ptr = strtok(NULL, "\0");

        // Verifica se há valor
        if (!ptr) {
            fprintf(stderr, "O valor não foi encontrado.\n");
            return NULL;
        }

        // Salva o valor
        selection->value = ptr;
    } else if (!strncmp(*cmd, AR, CMD_LIMIT) || !strncmp(*cmd, RR, CMD_LIMIT)) {
        // Ponteiro para a seleção
        Selection *selection = &(pData->data.selection);

        // Ponteiro auxiliar, pega o nome da tabela
        char *ptr = strtok(NULL, " ");
        
        // Verifica se há nome da tabela
        if (!ptr) {
            fprintf(stderr, "O nome da tabela não foi encontrado.\n");
            return NULL;
        }

        // Verifica o tamanho do nome
        if (strlen(ptr) >= TABLE_NAME_LIMIT) {
            fprintf(stderr, "Nome da tabela é muito longo.\n");
            return NULL;
        }

        // Salva o nome da tabela
        strncpy(selection->tableName, ptr, TABLE_NAME_LIMIT);

        // Verifica se há mais na linha
        ptr = strtok(NULL, "\0");
        if (ptr) {
            fprintf(stderr, "A estrutura do comando não foi reconhecida.\n");
            return NULL;
        }
    } else if (!strncmp(*cmd, CI, CMD_LIMIT)) {
        // Ponteiro para a seleção
        Selection *selection = &(pData->data.selection);

        // Ponteiro auxiliar, pega o parâmetro
        char *ptr = strtok(NULL, " ");

        // Verifica se há parâmetro
        if (!ptr) {
            fprintf(stderr, "O parâmetro não foi encontrado.\n");
            return NULL;
        }

        // Verifica o parâmetro
        toUpperCase(ptr);

        if (strncmp(ptr, H, PARAMETER_LIMIT) && strncmp(ptr, A, PARAMETER_LIMIT)) {
            fprintf(stderr, "O parâmetro não foi reconhecido\n");
            return NULL;
        }

        // Salva o parâmetro
        selection->parameter = ptr[0];
        
        // Pega o nome da tabela
        ptr = strtok(NULL, " ");
        
        // Verifica se há nome da tabela
        if (!ptr) {
            fprintf(stderr, "O nome da tabela não foi encontrado.\n");
            return NULL;
        }

        // Verifica o tamanho do nome
        if (strlen(ptr) >= TABLE_NAME_LIMIT) {
            fprintf(stderr, "Nome da tabela é muito longo.\n");
            return NULL;
        }

        // Salva o nome da tabela
        strncpy(selection->tableName, ptr, TABLE_NAME_LIMIT);

        // Pega o nome do campo
        ptr = strtok(NULL, "\0");

        // Verifica se há nome do campo
        if (!ptr) {
            fprintf(stderr, "O nome do campo não foi encontrado.\n");
            return NULL;
        }

        // Verifica o tamanho do nome do campo
        if (strlen(ptr) >= FIELD_NAME_LIMIT) {
            fprintf(stderr, "Nome do campo é muito longo.\n");
            return NULL;
        }

        // Salva o nome do campo
        if(replaceSpace(ptr, '_')) {
            printf("Espaços foram substituídos no nome do campo: %s.\n", ptr);
        }
        strncpy(selection->field, ptr, FIELD_NAME_LIMIT);
    } else if (!strncmp(*cmd, RI, CMD_LIMIT) || !strncmp(*cmd, GI, CMD_LIMIT)) {
        // Ponteiro para a seleção
        Selection *selection = &(pData->data.selection);

        // Ponteiro auxiliar, pega o nome da tabela
        char *ptr = strtok(NULL, " ");
        
        // Verifica se há nome da tabela
        if (!ptr) {
            fprintf(stderr, "O nome da tabela não foi encontrado.\n");
            return NULL;
        }

        // Verifica o tamanho do nome
        if (strlen(ptr) >= TABLE_NAME_LIMIT) {
            fprintf(stderr, "Nome da tabela é muito longo.\n");
            return NULL;
        }

        // Salva o nome da tabela
        strncpy(selection->tableName, ptr, TABLE_NAME_LIMIT);

        // Pega o nome do campo
        ptr = strtok(NULL, "\0");

        // Verifica se há nome do campo
        if (!ptr) {
            fprintf(stderr, "O nome do campo não foi encontrado.\n");
            return NULL;
        }

        // Verifica o tamanho do nome do campo
        if (strlen(ptr) >= FIELD_NAME_LIMIT) {
            fprintf(stderr, "Nome do campo é muito longo.\n");
            return NULL;
        }

        // Salva o nome do campo
        if(replaceSpace(ptr, '_')) {
            printf("Espaços foram substituídos no nome do campo: %s.\n", ptr);
        }
        strncpy(selection->field, ptr, FIELD_NAME_LIMIT);
    } else if (!strncmp(*cmd, EB, CMD_LIMIT)) {
        salvaBTrees(lista_btree);
        // Ponteiro auxiliar
        char *ptr = strtok(NULL, "\0");

        // Verifica se há mais na linha
        if (ptr) {
            fprintf(stderr, "A estrutura do comando não foi reconhecida.\n");
            return NULL;
        }
    } else {
        fprintf(stderr, "O comando não foi reconhecido.\n");
        return NULL;
    }

    return pData;
}