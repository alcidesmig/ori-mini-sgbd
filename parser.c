#include "parser.h"

// Transforma uma linha de comando em uma struct com os dados
// line: linha de comando
// return: ponteiro para a struct com os dados
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

        // Verifica se há pelo menos uma coluna
        if (!(*cols)) {
            fprintf(stderr, "Erro de sintax.\n");
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
    } else if (!strncmp(*cmd, RT, CMD_LIMIT)
            || !strncmp(*cmd, AT, CMD_LIMIT)
            || !strncmp(*cmd, AR, CMD_LIMIT)
            || !strncmp(*cmd, RR, CMD_LIMIT)) {

        // Ponteiro para a tabela
        Table *table = &(pData->data.table);

        // Ponteiro auxiliar, pega o nome da tabela
        char *ptr = strtok(NULL, " ");

        // Verifica se há nome
        if (!ptr) {
            fprintf(stderr, "Erro de sintax.\n");
            return NULL;
        }

        // Salva o nome
        table->name = ptr;
    } else if (!strncmp(*cmd, LT, CMD_LIMIT)) {
        // Ponteiro auxiliar, pega o nome da tabela
        char *ptr = strtok(NULL, " ");

        // Verifica se não há nome
        if (ptr) {
            fprintf(stderr, "Erro de sintax.\n");
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
            fprintf(stderr, "Erro de sintax.\n");
            return NULL;
        }

        // Salva o nome
        row->tableName = ptr;
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
            fprintf(stderr, "Erro de sintax.\n");
            return NULL;
        }
    } else if (!strncmp(*cmd, BR, CMD_LIMIT)
            || !strncmp(*cmd, CI, CMD_LIMIT)
            || !strncmp(*cmd, RI, CMD_LIMIT)
            || !strncmp(*cmd, GI, CMD_LIMIT)) {

        int br = !strncmp(*cmd, BR, CMD_LIMIT);
        int ci = !strncmp(*cmd, CI, CMD_LIMIT);

        // Ponteiro para a seleção
        Selection *selection = &(pData->data.selection);

        char *ptr = NULL;

        if (br || ci) {
            // Ponteiro auxiliar, pega o parâmetro
            ptr = strtok(NULL, " ");

            // Verifica se há parâmetro
            if (!ptr) {
                fprintf(stderr, "Erro de sintax.\n");
                return NULL;
            }

            // Salva o parâmetro
            toUpperCase(ptr);

            // Verifica o parâmetro
            if ((br && strncmp(ptr, U, PARAMETER_LIMIT) && strncmp(ptr, N, PARAMETER_LIMIT))
                ||
                (ci && strncmp(ptr, H, PARAMETER_LIMIT) && strncmp(ptr, A, PARAMETER_LIMIT))) {

                fprintf(stderr, "Erro de sintax.\n");
                return NULL;
            }

            selection->parameter = ptr[0];
        }
        
        // Pega o nome da tabela
        ptr = strtok(NULL, " ");
        
        // Verifica se há nome da tabela
        if (!ptr) {
            fprintf(stderr, "Erro de sintax.\n");
            return NULL;
        }

        // Salva o nome da tabela
        selection->tableName = ptr;

        // Pega o nome do campo
        ptr = strtok(NULL, ":");

        // Verifica se há nome do campo
        if (!ptr) {
            fprintf(stderr, "Erro de sintax.\n");
            return NULL;
        }

        // Salva o nome do campo
        selection->field = ptr;

        if (!strncmp(*cmd, BR, CMD_LIMIT)) {
            // Pega o valor
            ptr = strtok(NULL, "\0");

            // Verifica se há valor
            if (!ptr) {
                fprintf(stderr, "Erro de sintax.\n");
                return NULL;
            }

            // Salva o valor
            selection->value = ptr;
        }
    } else if (!strncmp(*cmd, EB, CMD_LIMIT)) {
        printf("Saindo...\n");
        exit(0);
    } else {
        fprintf(stderr, "Comando não reconhecido.\n");
        return NULL;
    }

    return pData;
}