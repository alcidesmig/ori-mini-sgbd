#include "commands.h"

// Cria uma tabela no banco
// table: ponteiro para tabela
void criarTabela(Table *table) {
    // Verifica se o nome é unico
    int unique = tableNameIsUnique(qtTables, table->name, NULL);

    // Se a tabela é unica
    if (unique) {
        // Path do arquivo da tabela
        char *path = glueString(2, TABLES_DIR, table->name);
        // O novo nome é colocado no index
        addTableName(qtTables, table->name); qtTables++;
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
    } else {
        fprintf(stderr, "Uma tabela com o mesmo nome já existe!\n");
    }
}

// Remove uma tabela do banco
// table: ponteiro para tabela
void removerTabela(Table *table) {
    // Verifica se existem tabelas
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
        // Abre o arquivo da tabela
        FILE *tableFile = fopenSafe(path, "rb+");
        // Lê os metadados
        fread(table, sizeof(Table), 1, tableFile);

        // Flag de validade
        int valido = 0;
        // Posição da string ou binário
        long int pos;
        // Remove as strings e binários
        printf("rows %d\n", table->rows);
        for (int i = 0; i < table->rows; i++) {
            printf("p %ld\n", ftell(tableFile));
            // Lê a flag de validade
            fread(&valido, sizeof(int), 1, tableFile);
            printf("valido %d\n", valido);
            if (valido) {
                for (int i = 0; i < table->cols; i++) {
                printf("type %c %d\n", table->types[i], table->types[i]);
                    if (table->types[i] == 'i') {
                        fseek(tableFile, sizeof(int), SEEK_CUR);
                    } else if (table->types[i] == 's') {
                        fread(&pos, sizeof(long int), 1, tableFile);
                        printf("pos %ld\n", pos);
                        removeFromExFile(pos, stringsFile, &stringEBlocks);
                    } else if (table->types[i] == 'f') {
                        fseek(tableFile, sizeof(float), SEEK_CUR);
                    } else if (table->types[i] == 'b') {
                        fread(&pos, sizeof(long int), 1, tableFile);
                        printf("pos %ld\n", pos);
                        removeFromExFile(pos, binariesFile, &binaryEBlocks);
                    } else {
                        printf("else\n");
                    }
                }
            } else {
                fseek(tableFile, table->length, SEEK_CUR);
            }
        }
        // Fecha o arquivo
        fclose(tableFile);
        // Remove o arquivo da tabela
        removeFile(path);
        // Path do arquivo de blocos deletados
        path = glueString(2, path, ".empty");
        // Remove o arquivo de blocos deletados
        removeFile(path);
        free(path);
        // Decrementa o número de tabelas
        qtTables--;

        printf("Tabela %s removida\n", table->name);
    } else {
        fprintf(stderr, "Tabela não encontrada!\n");
    }
}

// Mostra os dados de uma tabela
// table: ponteiro para tabela
void apresentarTabela(Table *table) {
    // Verifica se existem tabelas
    if (!qtTables) {
        printf("Não existem tabelas!\n");
        return;
    }

    // Verifica se a tabela existe
    int exists = tableExists(qtTables, table->name);

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

// Lista as tabelas do banco
void listarTabela() {
    // Verifica se existem tabelas
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

// Incluí um registro em uma tabela
// row: ponteiro para um regitro
void incluirRegistro(Row *row) {
    // Verifica se existem tabelas
    if (!qtTables) {
        printf("Não existem tabelas!\n");
        return;
    }

    // Verifica se a tabela existe
    int exists = tableExists(qtTables, row->tableName);

    // Se o marcador é válido
    if (exists) {
        // Path do arquivo da tabela
        char *path = glueString(2, TABLES_DIR, row->tableName);
        // Abre o arquivo da tabela
        FILE *tableFile = fopenSafe(path, "rb+");
        // Lê os metadados
        Table table;
        fread(&table, sizeof(Table), 1, tableFile);
        // Verifica o número de valores
        if (row->cols == table.cols) {
            // Path do arquivo de blocos deletados
            path = glueString(2, path, ".empty");
            // Abre o arquivo de blocos deletados
            FILE *tableFileEmpty = fopenSafe(path, "rb+");
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
                // Pula para a posição do registro inválido que sera sobrescrito        
                fseek(tableFile, openRow, SEEK_SET);
            } else {
                // Pula outros registros, mais as flags de validade
                fseek(tableFile, table.rows * (table.length + sizeof(int)), SEEK_CUR);
            }

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
            // Fecha arquivo de blocos deletados
            fclose(tableFileEmpty);
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
            return;
        }

        // Incrementa o número de registros
        table.rows++;
        // Pula o número de colunas
        fseek(tableFile, sizeof(int), SEEK_SET);
        // Salva o número de registros
        fwrite(&(table.rows), sizeof(int), 1, tableFile);
        // Fecha arquivo da tabela
        fclose(tableFile);
    } else {
        fprintf(stderr, "Tabela não encontrada!\n");
    }
}

// Busca um ou mais registros em uma tabela
// selection: ponteiro para uma seleção
void buscarRegistros(Selection *selection) {
    // Limite de busca
    int searchLimit = (selection->parameter == 'U' ? 1 : 2147483647);
    
    // Verifica se existem tabelas
    if (!qtTables) {
        printf("Não existem tabelas!\n");
        return;
    }

    // Verifica se a tabela existe
    int exists = tableExists(qtTables, selection->tableName);

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
        if (fieldType == 'i') {
            if(sscanf((char *)selection->value, "%d %[^\n]", &selNumbI, rest) != 1) {
                fprintf(stderr, "Erro ao converter o valor %s para inteiro!", (char *)selection->value);
                // Libera o resto, se leu a mais
                if (rest) {
                    free(rest);
                }
                // Fecha o arquivo
                fclose(tableFile);
                return;
            }
        } else if (fieldType == 'f') {
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
                    // Bytes lidos
                    read = sizeof(long int);
                    // Lê a posição da string
                    fread(&pos, read, 1, tableFile);
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
                fseek(tableFile, table.length-offset-read, SEEK_CUR);
            } else {
                // Pula o registro
                fseek(tableFile, table.length, SEEK_CUR);
            }

            i++;
        }

        if (resultList) {
            // Adiciona o resultado à arvore de resultados
            addToResultTree(&resultTree, resultList, selection->tableName);
        } else {
            printf("Nenhum resultado para %s\n", selection->tableName);
        }

        // Fecha o arquivo
        fclose(tableFile);
    } else {
        fprintf(stderr, "Tabela não encontrada!\n");
    }
}

// Apresenta os resultados de uma busca
// selection: ponteiro para uma seleção
void apresentarRegistros(Selection *selection) {
    // Verifica se existem tabelas
    if (!qtTables) {
        printf("Não existem tabelas!\n");
        return;
    }

    // Verifica se a tabela existe
    int exists = tableExists(qtTables, selection->tableName);

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
                    // Lê o número
                    fread(&numbI, sizeof(int), 1, tableFile);
                    // Printa o número
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
                    // Printa a string
                    printf("%s\n", str);
                    free(str);
                } else if (table.types[i] == 'f') {
                    // Lê o número
                    fread(&numbF, sizeof(int), 1, tableFile);
                    // Printa o número
                    printf("%f\n", numbF);
                } else if (table.types[i] == 'b') {
                    // Print simbólico do arquivo
                    printf("**BINARY**\n");
                    // Pula o tamanho do endereço para 
                    fseek(tableFile, sizeof(long int), SEEK_CUR);
                }
            }
            printf("\n");

            list = list->next;
        }

        // Fecha o arquivo
        fclose(tableFile);
    } else {
        fprintf(stderr, "Tabela não encontrada!\n");
    }
}

// Remove os registros da última busca
// selection: ponteiro para uma seleção
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
    int exists = tableExists(qtTables, selection->tableName);

    // Se a tabela existe
    if (exists) {
        // Recupera a pesquisa
        ResultList *list = searchResultList(resultTree, selection->tableName);

        if (list) {
            // Tabela em questão
            Table table;
            // Path do arquivo da tabela
            char *path = glueString(2, TABLES_DIR, selection->tableName);
            // Abre o arquivo da tabela
            FILE *tableFile = fopenSafe(path, "rb+");
            // Lê os metadados
            fread(&table, sizeof(Table), 1, tableFile);
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

                // Posição da string ou binário
                long int pos;
                // Remove as strings e binários
                for (int i = 0; i < table.cols; i++) {
                    if (table.types[i] == 'i') {
                        fseek(tableFile, sizeof(int), SEEK_CUR);
                    } else if (table.types[i] == 's') {
                        fread(&pos, sizeof(long int), 1, tableFile);
                        removeFromExFile(pos, stringsFile, &stringEBlocks);
                    } else if (table.types[i] == 'f') {
                        fseek(tableFile, sizeof(float), SEEK_CUR);
                    } else if (table.types[i] == 'b') {
                        fread(&pos, sizeof(long int), 1, tableFile);
                        removeFromExFile(pos, binariesFile, &binaryEBlocks);
                    }
                }

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
    if(tableExists(qtTables, selection->tableName)) {
        if(selection->parameter == 'H') { // se for index do tipo hash
            if(fieldExistInTable(selection->tableName, selection->field)){ // verifica se o campo a ser indexado existe na tabela
                if(getFieldType(selection->tableName, selection->field) != 'i') { // verifica se o campo a ser indexado é do tipo inteiro
                    fprintf(stderr, "O campo %s não é do tipo INT %s.\n", selection->field);
                    return;
                }
                char * filename = glueString(3, "tables_index/", selection->tableName, "_hash.index"); // elabora o nome do arquivo: tables_index/<nome-da-tabela>_hash.index
                FILE * aux;
                if(aux = fopen(filename, "rb") != NULL) { // verifica se o arquivo do index já existe
                    fprintf(stderr, "O campo %s já é indexado na tabela %s.\n", selection->field, selection->tableName);
                    fclose(aux);
                    return;
                }
                fclose(fopen(filename, "ab+")); // cria o arquivo do index
                // to continue

            } else {
                fprintf(stderr, "O campo %s não existe na tabela %s.\n", selection->field, selection->tableName);
            }
        } else if (selection->parameter == 'A') { // se for index do tipo árvore
            if(fieldExistInTable(selection->tableName, selection->field)){ // verifica se o campo a ser indexado existe na tabela
                if(getFieldType(selection->tableName, selection->field) != 'i') { // verifica se o campo a ser indexado é do tipo inteiro
                    fprintf(stderr, "O campo %s não é do tipo INT %s.\n", selection->field);
                    return;
                }
                char * filename = glueString(4, "tables_index/", selection->tableName, "_tree.index"); // elabora o nome do arquivo: tables_index/<nome-da-tabela>_tree.index
                FILE * aux;
                if(aux = fopen(filename, "rb") != NULL) { // verifica se o arquivo do index já existe
                    fprintf(stderr, "O campo %s já é indexado na tabela %s.\n", selection->field, selection->tableName);
                    fclose(aux);
                    return;
                }
                fclose(fopen(filename, "ab+")); // cria o arquivo do index
                // to continue

            } else {
                fprintf(stderr, "O campo %s não existe na tabela %s.\n", selection->field, selection->tableName);
            }
        } else {
            fprintf(stderr, "Opção %c inválida.\n", selection->parameter);
        }
    } else {
        fprintf(stderr, "A tabela %s não existe!\n", selection->tableName);
    }
}

void removerIndex(Selection *selection) {
}

void gerarIndex(Selection *selection) {
}

// Prepara o sistema
void start() {
    // Cria o diretório dos arquivos das tabelas
    mkdir(TABLES_DIR, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    // Cria os arquivos de indexação e dados
    createFile(TABLES_INDEX);
    createFile(STRINGS_FILE);
    createFile(STRINGS_EMPTY_LIST);
    createFile(BINARIES_FILE);
    createFile(BINARIES_EMPTY_LIST);

    // Abre os arquivos de indexação e dados
    tablesIndex = fopenSafe(TABLES_INDEX, "rb+");
    stringsFile = fopenSafe(STRINGS_FILE, "rb+");
    stringsEmptyList = fopenSafe(STRINGS_EMPTY_LIST, "rb+");
    binariesFile = fopenSafe(BINARIES_FILE, "rb+");
    binariesEmptyList = fopenSafe(BINARIES_EMPTY_LIST, "rb+");

    // Cria as listas de blocos vazios
    loadEmptyList(stringsEmptyList, &stringEBlocks);
    loadEmptyList(binariesEmptyList, &binaryEBlocks);

    // Pula para o começo do arquivo
    fseek(tablesIndex, 0, SEEK_SET);
    // Lê a quantidade de tabelas
    fread(&qtTables, sizeof(int), 1, tablesIndex);
}

// Encerra o sistema
void end() {
    // Pula para o começo do arquivo
    fseek(tablesIndex, 0, SEEK_SET);
    // Escreve o novo número de tabelas
    fwrite(&qtTables, sizeof(int), 1, tablesIndex);

    // Salva as listas de blocos vazios
    saveEmptyList(stringsEmptyList, &stringEBlocks);
    saveEmptyList(binariesEmptyList, &binaryEBlocks);

    // Fecha os arquivos de indexação e dados
    fclose(tablesIndex);
    fclose(stringsFile);
    fclose(stringsEmptyList);
    fclose(binariesFile);
    fclose(binariesEmptyList);

    freeResultTree(resultTree);
}