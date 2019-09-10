#include "commands.h"

FILE *tables_index = NULL; // Variável global utilizada para manipular arquivos

int qt_tables = 0; // Quantidade de tabelas

Result *result_list = NULL; // Lista de resultados

Dnode *search_dict = NULL; // Árvore com os resultados das pesquisas

// Cria tabela
// table: Struct com as informações para crira uma tabela
void createTable(TableWType *table) {
    // Novo nome a ser comparado
    char *novo = table->name;

    // Abre o arquivo de index das tabelas
    tables_index = safe_fopen(TABLES_INDEX, "rb+");

    // Lê a quantidade de tabelas
    qt_tables = read_qt_tables(tables_index);

    // Se existir alguma tabela
    if (qt_tables) {
        //  Verifica se o nome já existe

        // Pega todos os nomes das tabelas
        TableName *names = read_tables_names(tables_index, qt_tables);

        if (tableNameExists(names, novo, qt_tables)) {
            raiseError(TABLE_EXISTS);
        }

        free(names);
    }

    // Nesse ponto o novo nome não existe

    // Converte a tabela
    TableWRep tableData;

    if(!convertToRep(&tableData, table)) {
        raiseError(UNSUPORTED_TYPE);
    }

    // Escreve a tabela no arquivo
    write_table_metadata(tables_index, &tableData, qt_tables);

    qt_tables++;

    // Salva o novo número de tabelas
    write_qt_tables(tables_index, qt_tables);

    fclose(tables_index);

    printf("Criando tabela %s.\n\n", table->name);
}

// Remove tabela
// table_name: Nome da tabela
// OBS: remove o nome da lista de index e salva o último no lugar, a menos que seja o último esteja sendo removido
void removeTable(TableName table_name) {
    tables_index = safe_fopen(TABLES_INDEX, "rb+");

    qt_tables = read_qt_tables(tables_index);

    if (!qt_tables) {
        printf("Nenhuma tabela no sistema.\n");
        fclose(tables_index);
        return;
    }

    TableName *names = read_tables_names(tables_index, qt_tables);

    // Procura o nome da tabela
    int i = 0;
    while (i < qt_tables) {
        if (!strcmp(names[i], table_name)) {
            break;
        }
        i++;
    }

    // Verifica se a tabela foi achada
    if (i == qt_tables) {
        raiseError(CANT_FIND_TABLE);
    }

    printf("Removendo a tabela %s.\n", table_name);

    qt_tables--;

    // Arruma os nomes colocando o último no lugar do que foi removido
    if (i != qt_tables) {
        strncpy(names[i], names[qt_tables], sizeof(TableName));
        write_tables_names(tables_index, names, qt_tables);
    }

    // Salva o novo número de tabelas
    write_qt_tables(tables_index, qt_tables);

    // Deleta o arquivo da tabela
    char *path = glueString(3, TABLES_DIR, table_name, TABLE_EXTENSION);

    safe_remove(path);

    free(names);

    fclose(tables_index);
}

// Apresenta tabela tabela
// table_name: Nome da tabela
void apTable(TableName table_name) {
    TableWRep *tableData = read_table_metadata(table_name);

    if (!tableData) {
        raiseError(CANT_FIND_TABLE);
    }

    // Converte a tabela
    TableWType *table = safe_malloc(sizeof(TableWType));

    convertToType(table, tableData);

    printf("Mostrando a tabela %s.\n\n", table_name);

    printf("%s\n", table->name);

    for (int j = 0; j < table->cols; j++) {
        printf("- %s:%s\n", table->types[j], table->fields[j]);
    }

    free(tableData);
    free(table);
}

// Lista tabelas
void listTables() {
    tables_index = safe_fopen(TABLES_INDEX, "rb+");

    qt_tables = read_qt_tables(tables_index);

    if (!qt_tables) {
        printf("Nenhuma tabela no sistema.\n");
        fclose(tables_index);
        return;
    }

    printf("Listando %d tabelas.\n", qt_tables);

    // Pega os nomes das tabelas
    TableName *names = read_tables_names(tables_index, qt_tables);

    for (int i = 0; i < qt_tables; i++) {
        printf("- %s\n", names[i]);
    }

    free(names);

    fclose(tables_index);

    printf("\n");
}

// Inclui registro na tabela
// row: Struct com os valores de um registro
void includeReg(Row *row) {
    // Lê a tabela
    TableWRep *meta = read_table_metadata(row->table_name);
    if (!meta) {
        raiseError(CANT_FIND_TABLE);
    }

    // Abre o arquivo da tabela
    char *path = glueString(3, TABLES_DIR, row->table_name, TABLE_EXTENSION);

    FILE *table_file = safe_fopen(path, "rb+");

    // Confere o número de valores
    if (meta->cols != row->size) {
        raiseError(DIFF_PARAM_NUMB);
    }

    // Lê o número de rows
    int qt_row = 0;
    fseek(table_file, sizeof(TableWRep), SEEK_CUR);
    fread(&qt_row, sizeof(int), 1, table_file);

    // Muda o ponteiro para o lugar onde a row será salva: no final do arquivo
    fseek(table_file, 0, SEEK_END);

    long int row_len;

    // Conta o tamanho total da row em bytes
    for (int i = 0; i < row->size; i++) {
        if (meta->types[i] == STR_REP) {
            row_len += STR_SIZE;
        } else if (meta->types[i] == INT_REP) {
            row_len += sizeof(int);
        } else if (meta->types[i] == FLT_REP) {
            row_len += sizeof(float);
        } else if (meta->types[i] == BIN_REP) {
            char bin[BIN_FILE_NAME_SIZE] = "";
            if (sscanf(row->values[i], "%[^\n]", bin) == 1) {
                FILE * binary_file;
                binary_file = fopen(bin, "rb");
                fseek(binary_file, 0, SEEK_END);
                row_len += ftell(binary_file);
                fclose(binary_file);
            } else {
                raiseError(WRONG_VALUE);
            }

        }
    }

    // Escreve o tamanho total da row em bytes, na frente da row
    fwrite(&row_len, sizeof(long int), 1, table_file);

    // Escreve o registro no arquivo, campo por campo
    for (int i = 0; i < row->size; i++) {
        if (meta->types[i] == STR_REP) {
            char str[STR_SIZE] = "";
            if (sscanf(row->values[i], "%[^\n]", str) == 1) {        
                fwrite(str, STR_SIZE * sizeof(char), 1, table_file);
            } else {
                raiseError(WRONG_VALUE);
            }
        } else if (meta->types[i] == INT_REP) {
            int i = 0;
            if (sscanf(row->values[i], "%d", &i) == 1) {        
                fwrite(&i, sizeof(int), 1, table_file);
            } else {
                raiseError(WRONG_VALUE);
            }
        } else if (meta->types[i] == FLT_REP) {
            float f = 0.0;
            if (sscanf(row->values[i], "%f", &f) == 1) {        
                fwrite(&f, sizeof(float), 1, table_file);
            } else {
                raiseError(WRONG_VALUE);
            }
        } else if (meta->types[i] == BIN_REP) {
            char bin[BIN_FILE_NAME_SIZE] = "";
            if (sscanf(row->values[i], "%[^\n]", bin) == 1) {    
                // Lê os dados do arquivo binaŕio e escreve no campo da row  
                // Escreve o tamanho (em bytes) do campo binário e depois seus dados
                FILE * binary_file;
                binary_file = fopen(bin, "rb");
                fseek(binary_file, 0, SEEK_END);
                long int file_size = ftell(binary_file);
                fseek(binary_file, 0, SEEK_SET);
                char * buffer = (char *) malloc(sizeof(char*) * file_size);
                fread(buffer, file_size, 1, binary_file);
                fclose(binary_file);
                fwrite(&file_size, sizeof(long int), 1, table_file);
                FILE * binary_table_file = fopen(BINARY_TABLE, "ab");
                long int pos_in_binary_table_file = ftell(binary_table_file);
                fwrite(&pos_in_binary_table_file, sizeof(long int), 1, table_file);
                fwrite(buffer, file_size, 1, binary_table_file);
                fclose(binary_table_file);

            } else {
                raiseError(WRONG_VALUE);
            }
        }
    }


    // Aumenta o número de row
    qt_row++;
    // Pula os metadados
    fseek(table_file, sizeof(TableWRep), SEEK_SET);
    fwrite(&qt_row, sizeof(int), 1, table_file);

    printf("Novo registro na tabela %s.\n", row->table_name);

    for (int i = 0; i < row->size; i++) {
        printf("- %s\n", row->values[i]);
    }
    printf("\n");

    free(path);
    free(meta);

    fclose(table_file);
}

// Busca registros na tabela
// table_name: Nome da tabela
// field_name: Nome do campo(chave)
// value: Nome do valor
// matchings: quantidade máxima de resultados
void busReg(TableName table_name, Field field_name, Value value, int matchings) {
    // Lê a tabela
    TableWRep *meta = read_table_metadata(table_name);
    if (!meta) {
        raiseError(CANT_FIND_TABLE);
    }

    // Abre o arquivo da tabela
    char *path = glueString(3, TABLES_DIR, table_name, TABLE_EXTENSION);

    FILE *table_file = safe_fopen(path, "rb+");

    // Lê o tamanho de uma row
    int row_length = meta->row_bytes_size;

    // Lê o número de rows
    int qt_row = 0;
    fseek(table_file, sizeof(TableWRep), SEEK_CUR);
    fread(&qt_row, sizeof(int), 1, table_file);

    // Offset, tamanho e tipo do campo
    int *field_info = getOffset(meta, field_name);
    int offset = field_info[0];
    int field_size = field_info[1];
    char field_type = field_info[2];

    if (offset == -1) {
        raiseError(FIELD_NOT_FOUND);
    }
    
    // Auxiliar da leitura para cada tipo de dado
    char *s = safe_malloc(STR_SIZE);
    int i;
    float f;
    char *b = safe_malloc(BIN_SIZE);

    // Auxiliar da leitura para row
    char *data = NULL;

    // Backup da posição no arquivo
    long fp = 0;

    // Rows encontradas
    int rows_found = 0;

    // Flag de igualdade
    int equal;

    // Lê os dados das rows e salva os matchings
    int j = 0;
    while (j < qt_row && rows_found < matchings) {

        // Pula o long int que representa o tamanho da row em bytes
        fseek(table_file, sizeof(long int), SEEK_CUR);

        equal = 0;

        // Salva a posição no arquivo
        fp = ftell(table_file);

        // Avança o offset
        fseek(table_file, offset, SEEK_CUR);

        if (field_type == STR_REP) {
            // Lê o campo
            fread(s, STR_SIZE, 1, table_file);

            // Compara
            if (!strcmp(s, value)) {
                equal = 1;
            }
        } else if (field_type == INT_REP) {
            // Lê o campo
            fread(&i, sizeof(int), 1, table_file);

            // Converte o valor para int
            int v;
            if (sscanf(value, "%d", &v) != 1) {
                raiseError(NOT_INT);
            }
           
            // Compara
            if (i == v) {
                equal = 1;
            }
        } else if (field_type == FLT_REP) {
            // Lê o campo
            fread(&f, sizeof(float), 1, table_file);

            // Converte o valor para int
            float v;
            if (sscanf(value, "%f", &v) != 1) {
                raiseError(NOT_FLOAT);
            }

            // Compara
            if (f == v) {
                equal = 1;
            }
        } else if (field_type == BIN_REP) { // todo
            // Lê o campo
            fread(b, BIN_SIZE, 1, table_file);

            // Compara
            if (!strcmp(b, value)) {
                equal = 1;
            }
        } else {
            raiseError(UNSUPORTED_TYPE);
        }

        if (equal) {
            // Reseta a posição no arquivo para o começo de uma row
            fseek(table_file, fp, SEEK_SET);

            // Aloca para a row que será salva
            data = safe_malloc(row_length*sizeof(char));

            // Lê a row
            fread(data, row_length, 1, table_file);

            // Adiciona na lista
            result_list = addResult(result_list, data);
            rows_found++;
        } else {
            fseek(table_file, fp+row_length, SEEK_SET);
        }

        j++;
    }

    printf("Buscando %s igual à %s em %s.\n", field_name, value, table_name);

    search_dict = addDnode(search_dict, meta, result_list);
    result_list = NULL;

    free(field_info);
    free(path);

    fclose(table_file);
}
// Apresenta registros pesquisados da tabela
// table_name: Nome da tabela
void apReg(TableName table_name) {
    // Nó que foi salvo a pesquisa
    Dnode *node = findResultList(search_dict, table_name);
    if (!node) {
        raiseError(NO_SEARCH_TABLE);
    }

    printf("Mostrando a pesquisa de %s.\n", table_name);

    // A lista com os resultados
    Result *lista = node->result_list;

    // Posição nos dados
    int index = 0;

    // Auxiliares
    char *s = safe_malloc(STR_SIZE);
    int i;
    float f;
    long int b;

    // Número de colunas
    int cols = node->meta->cols;

    // Nomes dos campos
    FieldArr *fields = &node->meta->fields;

    // Tipos
    TypeRepArr *types = &node->meta->types;

    // Dados
    char *raw = NULL;

    while (lista) {
        index = 0;

        raw = lista->row_raw;
        printf("Reg:\n");
        for (int j = 0; j < cols; j++) {
            // Printa o nome do campo
            printf("- %s: ", (*fields)[j]);

            // Verifica o tipo de dado

            if (node->meta->types[j] == STR_REP) {
                memcpy(s, &raw[index], STR_SIZE);
                printf("%s\n", s);
                index += STR_SIZE;
            } else if ((*types)[j] == INT_REP) {
                memcpy(&i, &raw[index], sizeof(int));
                printf("%d\n", i);
                index += sizeof(int);
            } else if ((*types)[j] == FLT_REP) {
                memcpy(&f, &raw[index], sizeof(float));
                printf("%f\n", f);
                index += sizeof(float);
            } else if (node->meta->types[j] == BIN_REP) {
                memcpy(&b, &raw[index], BIN_SIZE); // BIN_SIZE = sizeof(long int)
                printf("%ld\n", b);
                index += BIN_SIZE;
            } else {
                raiseError(UNSUPORTED_TYPE);
            }
        }

        lista = lista->next;
        printf("\n");
    }

    free(s);
}

// Remove registro da tabela
// table_name: Nome da tabela
void removeReg(TableName table_name) {
    printf("Registros removidos de %s.\n", table_name);

    raiseError(TODO);
}

// Cria index da tabela, árvore
// table_name: Nome da tabela
// field_name: Nome da campo(chave) a ser usado
void createIndexA(TableName table_name, Field field_name) {
    printf("Criado um índice com árvore para %s usando a coluna %s.\n", table_name, field_name);

    raiseError(TODO);
}

// Cria index da tabela, heap
// table_name: Nome da tabela
// field_name: Nome da campo(chave) a ser usado
void createIndexH(TableName table_name, Field field_name) {
    printf("Criado um índice com heap para %s usando a coluna %s.\n", table_name, field_name);

    raiseError(TODO);
}

// Remove index da tabela
// table_name: Nome da tabela
// field_name: Nome da campo(chave) a ser usado
void removeIndex(TableName table_name, Field field_name) {
    printf("Removendo o índice de %s referente a chave %s.\n", table_name, field_name);

    raiseError(TODO);
}

// Gera index
// table_name: Nome da tabela
// field_name: Nome da campo(chave) a ser usado
void genIndex(TableName table_name, Field field_name) {
    printf("Gerando um índice com %s para %s usando a coluna %s.\n", "-recuperar-", table_name, field_name);

    raiseError(TODO);
}