#include "commands.h"

void criarTabela(Table *table) {
    // Quantidade de tabelas
    int qtTables = 0;

    // Pula para o começo do arquivo
    fseek(tablesIndex, 0, SEEK_SET);
    // Lê a quantidade de tabelas
    fread(&qtTables, sizeof(int), 1, tablesIndex);

    int i = 0;
    // Para cada tabela
    if (i < qtTables) {
        // Verifica se o nome é unico
        int unique = tableNameIsUnique(qtTables, table->name, NULL);

        // Se o novo nome é diferente
        if (unique) {
            goto criar;
        } else {
            fprintf(stderr, "Uma tabela com o mesmo nome já existe!\n");
        }
    // Se não existem tabelas
    } else {
        goto criar;
    }

    return;

    // GOTO??????????
    // Bloco de criação
    criar:
        // Path do arquivo da tabela
        char *path = glueString(2, TABLES_DIR, table->name);
        // O novo nome é colocado no index
        addTableName(qtTables, table->name);
        // É criado o arquivo da tabela
        createFile(path);
        // Arquivo da tabela
        FILE *tableFile = fopenSafe(path, "rb+");
        // Salva os metadados
        fwrite(table, sizeof(Table), 1, tableFile);
        // Fecha o arquivo
        fclose(tableFile);
        // Path do arquivo de blocos deletados
        path = glueString(2, path, ".empty");
        // É criado o arquivo de blocos deletados
        createFile(path);
        // Arquivo de blocos deletados
        tableFile = fopenSafe(path, "rb+");
        // Grava zero
        fwrite(&zero, sizeof(int), 1, tableFile);
        // Fecha o arquivo
        fclose(tableFile);

        free(path);

        printf("Tabela %s criada\n", table->name);
}

void removerTabela(Table *table) {
    // Quantidade de tabelas
    int qtTables = 0;

    // Pula para o começo do arquivo
    fseek(tablesIndex, 0, SEEK_SET);
    // Lê a quantidade de tabelas
    fread(&qtTables, sizeof(int), 1, tablesIndex);
    
    if (!qtTables) {
        printf("Não existem tabelas!\n");
        return;
    }

    // Marcador da posição do igual ou última
    long int marker;
    // Número de blocos
    int blocks;

    // Verifica e pega a posição do igual
    int exists = !tableNameIsUnique(qtTables, table->name, &marker);

    // Se a tabela existe
    if (exists) {
        // Pula para a posição do marcador
        fseek(tablesIndex, marker, SEEK_SET);
        // Lê o número de blocos
        fread(&blocks, sizeof(int), 1, tablesIndex);
        // Invalida os blocos
        blocks *= -1;
        // Pula para a posição do marcador
        fseek(tablesIndex, marker, SEEK_SET);
        // Escreve o número de blocos inválidados
        fwrite(&blocks, sizeof(int), 1, tablesIndex);
        // Path do arquivo da tabela
        char *path = glueString(2, TABLES_DIR, table->name);
        // Remove o arquivo da tabela
        removeFile(path);
        free(path);
        // Decrementa o número de tabelas
        qtTables--;
        // Pula para o começo do arquivo
        fseek(tablesIndex, 0, SEEK_SET);
        // Escreve o novo número de tabelas
        fwrite(&qtTables, sizeof(int), 1, tablesIndex);

        printf("Tabela %s removida\n", table->name);
    } else {
        fprintf(stderr, "Tabela não encontrada!\n");
    }
}

void apresentarTabela(Table *table) {
    // Quantidade de tabelas
    int qtTables = 0;

    // Pula para o começo do arquivo
    fseek(tablesIndex, 0, SEEK_SET);
    // Lê a quantidade de tabelas
    fread(&qtTables, sizeof(int), 1, tablesIndex);
    
    if (!qtTables) {
        printf("Não existem tabelas!\n");
        return;
    }

    // Verifica se a tabela existe
    int exists = !tableNameIsUnique(qtTables, table->name, NULL);

    // Se a tabela existe
    if (exists) {
        // Path do arquivo da tabela
        char *path = glueString(2, TABLES_DIR, table->name);

        // Abre o arquivo da tabela
        FILE *tableFile = fopenSafe(path, "rb+");
        // Lê os metadados
        fread(table, sizeof(Table), 1, tableFile);
        // Fecha o arquivo
        fclose(tableFile);

        // Printa o nome da tabela
        printf("Mostrando tabela: %s\n", table->name);
        // Printa a quantidade de registros
        if (table->rows) {
            printf("> Registros: %d\n", table->rows);
        } else {
            printf("> Nenhum registro\n");
        }

        printf("> Tamanho do registro: %d\n", table->length);

        // Printa as colunas
        for (int i = 0; i < table->cols; i++) {
            if (table->types[i] == 'i') {
                printf("- INT ");
            } else if (table->types[i] == 's') {
                printf("- STR ");
            } else if (table->types[i] == 'f') {
                printf("- FLT ");
            } else if (table->types[i] == 'b') {
                printf("- BIN ");
            }

            printf("%s\n", table->fields[i]);
        }
    } else {
        fprintf(stderr, "Tabela não encontrada!\n");
    }
}

void listarTabela() {
    // Quantidade de tabelas
    int qtTables = 0;

    // Pula para o começo do arquivo
    fseek(tablesIndex, 0, SEEK_SET);
    // Lê a quantidade de tabelas
    fread(&qtTables, sizeof(int), 1, tablesIndex);

    if (!qtTables) {
        printf("Não existem tabelas!\n");
        return;
    }

    printf("Mostrando tabelas:\n");

    int i = 0;
    while (i < qtTables) {
        // Número de blocos
        int blocks = 0;

        // Lê o número de blocos
        fread(&blocks, sizeof(int), 1, tablesIndex);

        // Se o espaço possuí informações válidas
        if (blocks > 0) {
            // Tamanho real do nome
            int size = blocks*BLOCK_SIZE;
            char *buf = (char *)mallocSafe(size);
            // Lê o nome
            fread(buf, size, 1, tablesIndex);
            // Printa o nome
            printf("- %s\n", buf);
            free(buf);
            // Incrementa só se a tabela é válida
            i++;
        } else {
            // Pula o espaço no caso de informações inválidas
            blocks *= -1;
            fseek(tablesIndex, blocks*BLOCK_SIZE, SEEK_CUR);
        }
    }
}

void incluirRegistro(Row *row) {
    // Quantidade de tabelas
    int qtTables = 0;

    // Pula para o começo do arquivo
    fseek(tablesIndex, 0, SEEK_SET);
    // Lê a quantidade de tabelas
    fread(&qtTables, sizeof(int), 1, tablesIndex);

    if (!qtTables) {
        printf("Não existem tabelas!\n");
        return;
    }

    // Verifica se a tabela existe
    int exists = !tableNameIsUnique(qtTables, row->tableName, NULL);

    // Se o marcador é válido
    if (exists) {
        // Path do arquivo da tabela
        char *path = glueString(2, TABLES_DIR, row->tableName);
        // Abre o arquivo da tabela
        FILE *tableFile = fopenSafe(path, "rb+");
        // Path do arquivo de blocos deletados
        path = glueString(2, path, ".empty");
        // Abre o arquivo de blocos deletados
        FILE *tableFileEmpty = fopenSafe(path, "rb+");
        // Lê os metadados
        Table table;
        fread(&table, sizeof(Table), 1, tableFile);

        // Quantidade de blocos deletados
        int qtOpenRow = 0;
        // Endereço da row livre
        long int openRow = 0;
        // Lê a quantidade de blocos deletados
        fread(&qtOpenRow, sizeof(int), 1, tableFileEmpty);

        // Se existem blocos deletados
        if (qtOpenRow) {
            qtOpenRow--;
            // Pula para o começo
            fseek(tableFileEmpty, 0, SEEK_SET);
            // Escreve o novo valor
            fwrite(&qtOpenRow, sizeof(int), 1, tableFileEmpty);
            // Pula para o último endereço
            fseek(tableFileEmpty, sizeof(int) + qtOpenRow*sizeof(long int), SEEK_SET);
            // Lê o endereço da row livre
            fread(&openRow, sizeof(long int), 1, tableFileEmpty);
        } else {
            // Pula outros registros
            fseek(tableFile, table.rows * table.length, SEEK_CUR);
        }

        // Verifica o número de valores
        if (row->cols == table.cols) {
            // Bit de validade
            fwrite(&valido, sizeof(int), 1, tableFile);
            // Para cada coluna
            for (int i = 0; i < table.cols; i++) {
                // Verifica o tipo de dado da coluna
                if (table.types[i] == 'i') {
                    // Auxiliares
                    int numb;
                    char *rest;
                    // Converte o dado
                    if (sscanf((char *)row->values[i], "%d %[^\n]", &numb, rest) != 1) {
                        fprintf(stderr, "O valor %s não corresponde ao tipo da coluna %s!\n", (char *)row->values[i], table.fields[i]);
                        // Fecha os arquivos
                        fclose(tableFile);
                        fclose(tableFileEmpty);
                        return;
                    }
                    // Escreve no arquivo da tabela
                    fwrite(&numb, sizeof(int), 1, tableFile);
                } else if (table.types[i] == 's') {
                    // Escreve a string no arquivo de strings
                    long int pos = addToExFile((char *)row->values[i], stringsFile, &stringEBlocks);
                    // Escreve a posição da string no arquivo da tabela
                    fwrite(&pos, sizeof(long int), 1, tableFile);
                } else if (table.types[i] == 'f') {
                    // Auxiliares
                    float numb;
                    char *rest;
                    // Converte o dado
                    if (sscanf((char *)row->values[i], "%f %[^\n]", &numb, rest) != 1) {
                        fprintf(stderr, "O valor %s não corresponde ao tipo da coluna %s!\n", (char *)row->values[i], table.fields[i]);
                        // Fecha os arquivos
                        fclose(tableFile);
                        fclose(tableFileEmpty);
                        return;
                    }
                    // Escreve no arquivo da tabela
                    fwrite(&numb, sizeof(float), 1, tableFile);
                } else if (table.types[i] == 'b') {
                    // Abre o arquivo de dados
                    FILE *inputFile = fopenSafe((char *)row->values[i], "rb");
                    // Pula para o fim
                    fseek(inputFile, 0, SEEK_END);
                    // Salva o tamanho dos dados
                    int inputSize = ftell(inputFile);
                    // Aloca memória para os dados
                    char *data = (char *)mallocSafe(inputSize);
                    // Pula para o começo do arquivo de dados
                    fseek(inputFile, 0, SEEK_SET);
                    // Lê o arquivo de dados
                    fread(data, inputSize, 1, inputFile);
                    // Fecha o arquivo de dados
                    fclose(inputFile);

                    // Escreve a string no arquivo de strings
                    long int pos = addToExFile(data, binariesFile, &binaryEBlocks);
                    // Escreve a posição da string no arquivo da tabela
                    fwrite(&pos, sizeof(long int), 1, tableFile);
                }
            }

            // Printa a mensagem de sucesso
            printf("Registro criado: ");
            for (int i = 0; i < table.cols; i++) {
                printf("%s ", (char *)row->values[i]);
            }
            printf("\n");
        } else {
            fprintf(stderr, "O número de valores não corresponde ao número de colunas da tabela!\n");
            // Fecha os arquivos
            fclose(tableFile);
            fclose(tableFileEmpty);
            return;
        }

        // Incrementa o número de registros
        table.rows++;
        // Pula o número de colunas
        fseek(tableFile, sizeof(int), SEEK_SET);
        // Salva o número de registros
        fwrite(&(table.rows), sizeof(int), 1, tableFile);
        // Fecha os arquivos
        fclose(tableFile);
        fclose(tableFileEmpty);
    } else {
        fprintf(stderr, "Tabela não encontrada!\n");
    }
}

void buscarRegistros(Selection *selection) {
    // Limite de busca
    int searchLimit = (selection->parameter == 'U' ? 1 : 2147483647);
    // Quantidade de tabelas
    int qtTables = 0;

    // Pula para o começo do arquivo
    fseek(tablesIndex, 0, SEEK_SET);
    // Lê a quantidade de tabelas
    fread(&qtTables, sizeof(int), 1, tablesIndex);
    
    if (!qtTables) {
        printf("Não existem tabelas!\n");
        return;
    }

    // Verifica se a tabela existe
    int exists = !tableNameIsUnique(qtTables, selection->tableName, NULL);

    // Se a tabela existe
    if (exists) {
        // Tabela em questão
        Table table;
        // Path do arquivo da tabela
        char *path = glueString(2, TABLES_DIR, selection->tableName);
        // Abre o arquivo da tabela
        FILE *tableFile = fopenSafe(path, "rb+");
        // Lê os metadados
        fread(&table, sizeof(Table), 1, tableFile);

        // Offset do campo nos dados
        int offset = 0;
        // Tipo do campo
        char fieldType = '\0';

        int i = 0;
        // Procura o campo da busca
        while (i < table.cols && !fieldType) {
            // Se o campo foi encontrado
            if (!strcmp(table.fields[i], selection->field)) {
                // É definido o tipo do campo
                fieldType = table.types[i];
                break;
            // Se não for encontrado
            } else {
                // É incrementado o offset
                if (table.types[i] == 'i') {
                    offset += sizeof(int);
                } else if (table.types[i] == 's') {
                    offset += sizeof(long int);
                } else if (table.types[i] == 'f') {
                    offset += sizeof(float);
                } else if (table.types[i] == 'b') {
                    offset += sizeof(long int);
                }
            }

            i++;
        }

        // Se o campo não for encontrado
        if (i == table.cols) {
            fprintf(stderr, "Campo %s não encontrado!\n", selection->field);
            // Fecha o arquivo
            fclose(tableFile);
            return;
        }

        // Posição do registro
        long int rowPos = 0;

        // Resto do sscanf
        char *rest = NULL;

        // Auxiliar, conversão do valor de pesquisa
        int selNumbI;
        float selNumbF;

        // Converte os valores
        if (sscanf((char *)selection->value, "%d %[^\n]", &selNumbI, rest) != 1) {
            fprintf(stderr, "Erro ao converter o valor %s para inteiro!", (char *)selection->value);
            // Libera o resto, se leu a mais
            if (rest) {
                free(rest);
            }
            // Fecha o arquivo
            fclose(tableFile);
            return;
        }
        if (sscanf((char *)selection->value, "%f %[^\n]", &selNumbF, rest) != 1) {
            fprintf(stderr, "Erro ao converter o valor %s para ponto flutuante!", (char *)selection->value);
            // Libera o resto, se leu a mais
            if (rest) {
                free(rest);
            }
            // Fecha o arquivo
            fclose(tableFile);
            return;
        }

        // Auxiliar, bytes lidos
        int read = 0;
        // Auxiliar de leitura
        int numbI;
        float numbF;
        long int pos;
        int strSize;
        char *str;
        // Flag de validade
        int valido = 0;

        // Lista de resultados
        ResultList *resultList = NULL;

        // Compara os registros
        i = 0;
        while (i < table.rows && i < searchLimit) {
            // Salva a posição do registro
            rowPos = ftell(tableFile);
            // Lê a flag de validade
            fread(&valido, sizeof(int), 1, tableFile);

            if (valido) {
                // Pula o offset
                fseek(tableFile, offset, SEEK_CUR);

                // Lê o campo
                if (fieldType == 'i') {
                    // Bytes lidos
                    read = sizeof(int);
                    // Lê o número
                    fread(&numbI, read, 1, tableFile);

                    // Compara com o valor pesquisado
                    if (numbI == selNumbI) {
                        // Adiciona a posição a lista de resultados
                        addToResultList(&resultList, rowPos);
                    }
                } else if (fieldType == 's') {
                    // Lê a posição da string
                    fread(&pos, sizeof(long int), 1, tableFile);
                    // Pula para posição da string
                    fseek(stringsFile, pos, SEEK_SET);
                    // Lê o tamanho
                    fread(&strSize, sizeof(int), 1, stringsFile);
                    // Aloca memória para string
                    str = (char *)mallocSafe(strSize+1);
                    // Lê a string
                    fread(str, strSize, 1, stringsFile);
                    // Termina a string
                    str[strSize] = '\0';
                    // Bytes lidos
                    read = strSize;

                    // Compara com o valor pesquisado
                    if (!strcmp(str, (char *)selection->value)) {
                        // Adiciona a posição a lista de resultados
                        addToResultList(&resultList, rowPos);
                    }
                    
                    free(str);
                } else if (fieldType == 'f') {
                    // Bytes lidos
                    read = sizeof(float);
                    // Lê o número
                    fread(&numbF, read, 1, tableFile);

                    // Compara com o valor pesquisado
                    if (numbF == selNumbF) {
                        // Adiciona a posição a lista de resultados
                        addToResultList(&resultList, rowPos);
                    }
                } else if (fieldType == 'b') {
                    fprintf(stderr, "Busca em campos binários não é suportada!\n");
                    // Fecha o arquivo
                    fclose(tableFile);
                    return;
                }

                // Pula os campos restantes
                fseek(tableFile, table.length-offset-read-sizeof(int), SEEK_CUR);
            } else {
                // Pula o registro
                fseek(tableFile, table.length-sizeof(int), SEEK_CUR);
            }

            i++;
        }

        // Adiciona o resultado à arvore de resultados
        addToResultTree(&resultTree, resultList, selection->tableName);

        apresentarRegistros(selection);

        // Fecha o arquivo
        fclose(tableFile);
    } else {
        fprintf(stderr, "Tabela não encontrada!\n");
    }
}

void apresentarRegistros(Selection *selection) {
    // Quantidade de tabelas
    int qtTables = 0;

    // Pula para o começo do arquivo
    fseek(tablesIndex, 0, SEEK_SET);
    // Lê a quantidade de tabelas
    fread(&qtTables, sizeof(int), 1, tablesIndex);
    
    if (!qtTables) {
        printf("Não existem tabelas!\n");
        return;
    }

    // Verifica se a tabela existe
    int exists = !tableNameIsUnique(qtTables, selection->tableName, NULL);

    // Se a tabela existe
    if (exists) {
        // Recupera a pesquisa
        ResultList *list = searchResultList(resultTree, selection->tableName);

        // Tabela em questão
        Table table;
        // Path do arquivo da tabela
        char *path = glueString(2, TABLES_DIR, selection->tableName);
        // Abre o arquivo da tabela
        FILE *tableFile = fopenSafe(path, "rb+");
        // Lê os metadados
        fread(&table, sizeof(Table), 1, tableFile);

        // Auxiliar de leitura
        int numbI;
        float numbF;
        long int pos;
        int strSize;
        char *str;

        if (list) {
            printf("Mostrando resultado para %s\n", selection->tableName);
        } else {
            printf("Nenhum resultado para %s\n", selection->tableName);
        }
        
        // Printa os registros
        while (list) {
            // Pula para posição, mais a flag de validade
            fseek(tableFile, list->pos+sizeof(int), SEEK_SET);

            printf("Registro:\n");
            for (int i = 0; i < table.cols; i++) {
                printf("- %s: ", table.fields[i]);
                if (table.types[i] == 'i') {
                    fread(&numbI, sizeof(int), 1, tableFile);

                    printf("%d\n", numbI);
                } else if (table.types[i] == 's') {
                    // Lê a posição da string
                    fread(&pos, sizeof(long int), 1, tableFile);
                    // Pula para posição da string
                    fseek(stringsFile, pos, SEEK_SET);
                    // Lê o tamanho
                    fread(&strSize, sizeof(int), 1, stringsFile);
                    // Aloca memória para string
                    str = (char *)mallocSafe(strSize+1);
                    // Lê a string
                    fread(str, strSize, 1, stringsFile);
                    // Termina a string
                    str[strSize] = '\0';

                    printf("%s\n", str);

                    free(str);
                } else if (table.types[i] == 'f') {
                    fread(&numbF, sizeof(int), 1, tableFile);

                    printf("%f\n", numbF);
                } else if (table.types[i] == 'b') {
                    printf("**BINARY**\n");
                }
            }

            list = list->next;
        }

        printf("\n");

        // Fecha o arquivo
        fclose(tableFile);
    } else {
        fprintf(stderr, "Tabela não encontrada!\n");
    }
}

void removerRegistros(Selection *selection) {
    // Quantidade de tabelas
    int qtTables = 0;

    // Pula para o começo do arquivo
    fseek(tablesIndex, 0, SEEK_SET);
    // Lê a quantidade de tabelas
    fread(&qtTables, sizeof(int), 1, tablesIndex);
    
    if (!qtTables) {
        printf("Não existem tabelas!\n");
        return;
    }

    // Verifica se a tabela existe
    int exists = !tableNameIsUnique(qtTables, selection->tableName, NULL);

    // Se a tabela existe
    if (exists) {
        // Recupera a pesquisa
        ResultList *list = searchResultList(resultTree, selection->tableName);

        if (list) {
            // Path do arquivo da tabela
            char *path = glueString(2, TABLES_DIR, selection->tableName);
            // Abre o arquivo da tabela
            FILE *tableFile = fopenSafe(path, "rb+");
            // Path do arquivo de blocos deletados da tabela
            path = glueString(2, path, ".empty");
            // Abre o arquivo de blocos deletados da tabela
            FILE *tableFileEmpty = fopenSafe(path, "rb+");

            free(path);

            // Número de blocos deletados
            int empty = 0;
            // Lê o número de blocos deletados
            fread(&empty, sizeof(int), 1, tableFileEmpty);

            while (list) {
                // Grava a posição deletada
                fwrite(&(list->pos), sizeof(long int), 1, tableFileEmpty);
                // Pula para posição
                fseek(tableFile, list->pos, SEEK_SET);
                // Invalida o registro
                fwrite(&invalido, sizeof(int), 1, tableFile);
                // Incrementa
                empty++;
                // Avança na lista
                list = list->next;
            }

            // Pula para o começo
            fseek(tableFileEmpty, 0, SEEK_SET);
            // Escreve o número de blocos deletados
            fwrite(&empty, sizeof(int), 1, tableFileEmpty);

            // Fecha os arquivos
            fclose(tableFile);
            fclose(tableFileEmpty);
        } else {
            fprintf(stderr, "Não existe pesquisa para essa tabela!\n");
        }
    } else {
        fprintf(stderr, "Tabela não encontrada!\n");
    }
}

void criarIndex(Selection *selection) {
}

void removerIndex(Selection *selection) {
}

void gerarIndex(Selection *selection) {
}

void start() {
    mkdir(TABLES_DIR, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    createFile(TABLES_INDEX);
    createFile(STRINGS_FILE);
    createFile(STRINGS_EMPTY_LIST);
    createFile(BINARIES_FILE);
    createFile(BINARIES_EMPTY_LIST);

    tablesIndex = fopenSafe(TABLES_INDEX, "rb+");
    stringsFile = fopenSafe(STRINGS_FILE, "rb+");
    stringsEmptyList = fopenSafe(STRINGS_EMPTY_LIST, "rb+");
    binariesFile = fopenSafe(BINARIES_FILE, "rb+");
    binariesEmptyList = fopenSafe(BINARIES_EMPTY_LIST, "rb+");

    loadEmptyList(stringsEmptyList, &stringEBlocks);
    loadEmptyList(binariesEmptyList, &binaryEBlocks);
}

void end() {
    saveEmptyList(stringsEmptyList, &stringEBlocks);
    saveEmptyList(binariesEmptyList, &binaryEBlocks);

    fclose(tablesIndex);
    fclose(stringsFile);
    fclose(stringsEmptyList);
    fclose(binariesFile);
    fclose(binariesEmptyList);

    freeResultTree(resultTree);
}

// // Busca registros na tabela
// // table_name: Nome da tabela
// // field_name: Nome do campo(chave)
// // value: Nome do valor
// // matchings: quantidade máxima de resultados
// void busReg(TableName table_name, Field field_name, Value value, int matchings) {
//     // Lê a tabela
//     TableWRep *meta = read_table_metadata(table_name);
//     if (!meta) {
//         raiseError(CANT_FIND_TABLE);
//     }

//     // Abre o arquivo da tabela
//     char *path = glueString(3, TABLES_DIR, table_name, TABLE_EXTENSION);

//     FILE *table_file = safe_fopen(path, "rb+");

//     // Lê o tamanho de uma row
//     int row_length = meta->row_bytes_size;

//     // Lê o número de rows
//     int qt_row = 0;
//     fseek(table_file, sizeof(TableWRep), SEEK_CUR);
//     fread(&qt_row, sizeof(int), 1, table_file);

//     // Offset, tamanho e tipo do campo
//     int *field_info = getOffset(meta, field_name);
//     int offset = field_info[0];
//     int field_size = field_info[1];
//     char field_type = field_info[2];

//     if (offset == -1) {
//         raiseError(FIELD_NOT_FOUND);
//     }
    
//     // Auxiliar da leitura para cada tipo de dado
//     char *s = safe_malloc(STR_SIZE);
//     int i;
//     float f;
//     char *b = safe_malloc(BIN_SIZE);

//     // Auxiliar da leitura para row
//     char *data = NULL;

//     // Backup da posição no arquivo
//     long fp = 0;

//     // Rows encontradas
//     int rows_found = 0;

//     // Flag de igualdade
//     int equal;

//     // Lê os dados das rows e salva os matchings
//     int j = 0;
//     while (j < qt_row && rows_found < matchings) {

//         // Pula o long int que representa o tamanho da row em bytes
//         fseek(table_file, sizeof(long int), SEEK_CUR);

//         equal = 0;

//         // Salva a posição no arquivo
//         fp = ftell(table_file);

//         // Avança o offset
//         fseek(table_file, offset, SEEK_CUR);

//         if (field_type == STR_REP) {
//             // Lê o campo
//             fread(s, STR_SIZE, 1, table_file);

//             // Compara
//             if (!strcmp(s, value)) {
//                 equal = 1;
//             }
//         } else if (field_type == INT_REP) {
//             // Lê o campo
//             fread(&i, sizeof(int), 1, table_file);

//             // Converte o valor para int
//             int v;
//             if (sscanf(value, "%d", &v) != 1) {
//                 raiseError(NOT_INT);
//             }
           
//             // Compara
//             if (i == v) {
//                 equal = 1;
//             }
//         } else if (field_type == FLT_REP) {
//             // Lê o campo
//             fread(&f, sizeof(float), 1, table_file);

//             // Converte o valor para int
//             float v;
//             if (sscanf(value, "%f", &v) != 1) {
//                 raiseError(NOT_FLOAT);
//             }

//             // Compara
//             if (f == v) {
//                 equal = 1;
//             }
//         } else if (field_type == BIN_REP) { // todo
//             // Lê o campo
//             fread(b, BIN_SIZE, 1, table_file);

//             // Compara
//             if (!strcmp(b, value)) {
//                 equal = 1;
//             }
//         } else {
//             raiseError(UNSUPORTED_TYPE);
//         }

//         if (equal) {
//             // Reseta a posição no arquivo para o começo de uma row
//             fseek(table_file, fp, SEEK_SET);

//             // Aloca para a row que será salva
//             data = safe_malloc(row_length*sizeof(char));

//             // Lê a row
//             fread(data, row_length, 1, table_file);

//             // Adiciona na lista
//             result_list = addResult(result_list, data);
//             rows_found++;
//         } else {
//             fseek(table_file, fp+row_length, SEEK_SET);
//         }

//         j++;
//     }

//     printf("Buscando %s igual à %s em %s.\n", field_name, value, table_name);

//     search_dict = addDnode(search_dict, meta, result_list);
//     result_list = NULL;

//     free(field_info);
//     free(path);

//     fclose(table_file);
// }
// // Apresenta registros pesquisados da tabela
// // table_name: Nome da tabela
// void apReg(TableName table_name) {
//     // Nó que foi salvo a pesquisa
//     Dnode *node = findResultList(search_dict, table_name);
//     if (!node) {
//         raiseError(NO_SEARCH_TABLE);
//     }

//     printf("Mostrando a pesquisa de %s.\n", table_name);

//     // A lista com os resultados
//     Result *lista = node->result_list;

//     // Posição nos dados
//     int index = 0;

//     // Auxiliares
//     char *s = safe_malloc(STR_SIZE);
//     int i;
//     float f;
//     long int b;

//     // Número de colunas
//     int cols = node->meta->cols;

//     // Nomes dos campos
//     FieldArr *fields = &node->meta->fields;

//     // Tipos
//     TypeRepArr *types = &node->meta->types;

//     // Dados
//     char *raw = NULL;

//     while (lista) {
//         index = 0;

//         raw = lista->row_raw;
//         printf("Reg:\n");
//         for (int j = 0; j < cols; j++) {
//             // Printa o nome do campo
//             printf("- %s: ", (*fields)[j]);

//             // Verifica o tipo de dado

//             if (node->meta->types[j] == STR_REP) {
//                 memcpy(s, &raw[index], STR_SIZE);
//                 printf("%s\n", s);
//                 index += STR_SIZE;
//             } else if ((*types)[j] == INT_REP) {
//                 memcpy(&i, &raw[index], sizeof(int));
//                 printf("%d\n", i);
//                 index += sizeof(int);
//             } else if ((*types)[j] == FLT_REP) {
//                 memcpy(&f, &raw[index], sizeof(float));
//                 printf("%f\n", f);
//                 index += sizeof(float);
//             } else if (node->meta->types[j] == BIN_REP) {
//                 memcpy(&b, &raw[index], BIN_SIZE); // BIN_SIZE = sizeof(long int)
//                 printf("%ld\n", b);
//                 index += BIN_SIZE;
//             } else {
//                 raiseError(UNSUPORTED_TYPE);
//             }
//         }

//         lista = lista->next;
//         printf("\n");
//     }

//     free(s);
// }
