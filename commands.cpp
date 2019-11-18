#include "commands.hpp"

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

        // Arquivo de strings
        char *auxPath = glueString(2, path, "_strings.bin");
        createFile(auxPath);
        free(auxPath);

        // Arquivo de binarios
        auxPath = glueString(2, path, "_binaries.bin");
        createFile(auxPath);
        free(auxPath);

        // Auxiliar
        long FLAG = -1;
        FILE *fp = NULL;

        // Arquivo de strings deletadas
        auxPath = glueString(2, path, "_strings.empty");
        createFile(auxPath);
        fp = fopenSafe(auxPath, "rb+");
        fwrite(&FLAG, sizeof(long), 1, fp);
        fclose(fp);
        free(auxPath);

        // Arquivo de binarios deletados
        auxPath = glueString(2, path, "_binaries.empty");
        createFile(auxPath);
        fp = fopenSafe(auxPath, "rb+");
        fwrite(&FLAG, sizeof(long), 1, fp);
        fclose(fp);
        free(auxPath);

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

        // Remoção dos arquivos

        // Arquivo da tabela
        char *path = glueString(2, TABLES_DIR, table->name);
        removeFile(path);
        free(path);

        // Arquivo de strings
        char *auxPath = glueString(2, path, "_strings.bin");
        removeFile(auxPath);
        free(auxPath);

        // Arquivo de binarios
        auxPath = glueString(2, path, "_binaries.bin");
        removeFile(auxPath);
        free(auxPath);

        // Arquivo de strings deletadas
        auxPath = glueString(2, path, "_strings.empty");
        removeFile(auxPath);
        free(auxPath);

        // Arquivo de binarios deletados
        auxPath = glueString(2, path, "_binaries.empty");
        removeFile(auxPath);
        free(auxPath);

        // Arquivo de blocos deletados
        auxPath = glueString(2, path, ".empty");
        removeFile(auxPath);
        free(auxPath);

        // Remove os índices da tabela, sem printar nada
        removerIndex(table->name, NULL, 0, 1);
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

        // Printa os índices existentes
        printf("Índices existentes:\n");

        int achouIndiceHash = 0;
        int achouIndiceTree = 0;

        for (int i=0; i<table->cols; i++) {
            if(tem_index_hash(table->name, table->fields[i])) {
                if (!achouIndiceHash)
                    printf("Indice Hash para os campos:\n");
                printf("  - %s\n", table->fields[i]);
                achouIndiceHash = 1;
            }
            if(tem_index_tree(table->name, table->fields[i])) {
                if (!achouIndiceTree)
                    printf("Indice Tree para os campos:");
                printf("  - %s\n", table->fields[i]);
                achouIndiceTree = 1;
            }
        }

        if (!achouIndiceHash) printf("\t> Hash: não existem índices\n");
        if (!achouIndiceTree) printf("\t> Árvore: não existem índices\n");

    } else {
        fprintf(stderr, "Tabela não encontrada!\n");
    }
}

// Lista as tabelas do banco
void listarTabela() {
    // Coloca o ponteiro no início do arquivo para uma nova chamada da função
    fseek(tablesIndex, sizeof(int), SEEK_SET);

    // Verifica se existem tabelas
    if (!qtTables) {
        printf("Não existem tabelas!\n");
        return;
    }

    if (qtTables == 1) {
        printf("Mostrando 1 tabela:\n");
    } else {
        printf("Mostrando %d tabelas:\n", qtTables);
    }

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

    // Carrega a btree da tabela utilizada caso ela ainda não tenha sido carregada
    // carregaBTree(row->tableName);

    // Verifica se a tabela existe
    int exists = tableExists(qtTables, row->tableName);

    // Se o marcador é válido
    if (exists) {
        // Path do arquivo da tabela
        char *path = glueString(2, TABLES_DIR, row->tableName);
        // Abre o arquivo da tabela
        FILE *tableFile = fopenSafe(path, "rb+");

        // Path do arquivo de strings da tabela
        char *pathString = glueString(2, path, "_strings.bin");
        FILE *stringsFile = NULL;

        // Path do arquivo de binarios da tabela
        char *pathBinary = glueString(2, path, "_binaries.bin");
        FILE *binariesFile = NULL;

        // Path do arquivo de strings deletadas da tabela
        char *pathStringEmpty = glueString(2, path, "_strings.empty");
        FILE *stringsFileEmpty = NULL;

        // Path do arquivo de binarios deletados da tabela
        char *pathBinaryEmpty = glueString(2, path, "_binaries.empty");
        FILE *binariesFileEmpty = NULL;

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
            
            // Auxiliares para indexação
            // Posição do registro no arquivo
            int posInsercaoRegistro = ftell(tableFile);

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
                    // Verifica se há indexação
                    if(tem_index_tree(row->tableName, table.fields[i])) {
                        // Encontra a Btree correspondente a tabela e ao campo
                        Btree * tree = new Btree(glueString(5, "tables_index/", row->tableName, "_", table.fields[i], "_tree.index"));
                        // Adiciona os valores na tree
                        pair_btree aux;
                        aux.addr = posInsercaoRegistro;
                        aux.key = numb;
                        tree->insert(aux);
                        // Delete na tree (necessário para chamar o construtor e manter a assinatura válida
                        delete tree;
                    }
                    if(tem_index_hash(row->tableName, table.fields[i])) {
                        //TODO: inserir no arquivo da hashtable o valor (posInsercaoRegistro) com chave (numb)
                        char * hashFilename  = glueString(5, "tables_index/", row->tableName, "_", table.fields[i], "_hash.index"); 
                        //hashFileInsert(hashFilename, valoresFieldsIndexados[i], posInsercaoRegistro);
                    }

                } else if (table.types[i] == 's') {
                    if (!stringsFile) {
                        stringsFile = fopenSafe(pathString, "rb+");
                        stringsFileEmpty = fopenSafe(pathStringEmpty, "rb+");
                    }

                    // Escreve a string no arquivo de strings
                    long int pos = addToExFile((char *)row->values[i], stringsFile, stringsFileEmpty);
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
                    if (!binariesFile) {
                        binariesFile = fopenSafe(pathBinary, "rb+");
                        binariesFileEmpty = fopenSafe(pathBinaryEmpty, "rb+");
                    }

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
                    long int pos = addToExFile(data, binariesFile, binariesFileEmpty);
                    // Escreve a posição da string no arquivo da tabela
                    fwrite(&pos, sizeof(long int), 1, tableFile);
                }
            }
            // Fecha arquivo de blocos deletados
            fclose(tableFileEmpty);

            // Fecha os arquivos de dados tabela
            if (stringsFile) {
                fclose(stringsFile);
                fclose(stringsFileEmpty);
            }
            if (binariesFile) {
                fclose(binariesFile);
                fclose(binariesFileEmpty);
            }
            free(pathString);
            free(pathBinary);
            free(pathStringEmpty);
            free(pathBinaryEmpty);
            
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
    // Carrega a btree da tabela utilizada caso ela ainda não tenha sido carregada
    // carregaBTree(selection->tableName);

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

        // Path do arquivo de strings da tabela
        char *pathString = glueString(2, path, "_strings.bin");
        FILE *stringsFile = NULL;

        // Verifica se tem indexação
        int temIndexTree = tem_index_tree(selection->tableName, selection->field);
        int temIndexHash = tem_index_hash(selection->tableName, selection->field);

        // se existir e for o critério de busca: realiza busca pelo índice, se não: busca sequencial
        if (temIndexHash) {

            int value;
            if(sscanf((char *) selection->value, "%d", &value) != 1) {
                fprintf(stderr, "Erro na busca (indexação)!\n");
                return;
            }
            //TODO: Busca pelo indice hash
            char * hashFilename = glueString(5, "tables_index/", selection->tableName, "_", selection->field, "_hash.index");
            int registerPos = buscaEmArquivoHash(value, hashFilename);

            return;
        } else if (temIndexTree) {
            printf("Buscando por indexação. Field indexado: %s\n", selection->field);

            Btree * tree = new Btree(glueString(5, "tables_index/", selection->tableName, "_", selection->field, "_tree.index"));

            int value;
            if(sscanf((char *) selection->value, "%d", &value) != 1) {
                fprintf(stderr, "Erro na busca (indexação)!\n");
                return;
            }
            // Busca o par (key, addr) na BTree
            pair_btree pair;
            pair.key = value;
            int search = tree->search(&pair);
            // Delete na tree (necessário para chamar o construtor e manter a assinatura válida
            delete tree;
            // Verifica se encontrou
            if(!search) {
                printf("Nenhum resultado para %s\n", selection->tableName);
                return;
            }
            
            int x = 0;
            // Pega a posição do registro no arquivo
            int * addr = (int*) malloc(sizeof(int));
            *addr = pair.addr;

            // Lista de resultados
            ResultList *resultList = NULL;
            if(addr != NULL) {
                addToResultList(&resultList, *addr);
            }
            if (resultList) {
                // Adiciona o resultado à arvore de resultados
                addToResultTree(&resultTree, resultList, selection->tableName);
            } else {
                printf("Nenhum resultado para %s\n", selection->tableName);
            }

        } else {
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
                        if (!stringsFile) {
                            stringsFile = fopenSafe(pathString, "rb+");
                        }

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

            // Fecha os arquivos
            fclose(tableFile);
            // Fecha os arquivos de dados tabela
            if (stringsFile) {
                fclose(stringsFile);
            }
            free(pathString);
        }
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

        // Path do arquivo de strings da tabela
        char *pathString = glueString(2, path, "_strings.bin");
        FILE *stringsFile = NULL;

        // Lê os metadados
        fread(&table, sizeof(Table), 1, tableFile);
        //checkpoint alcides
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
                    if (!stringsFile) {
                        stringsFile = fopenSafe(pathString, "rb+");
                    }

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

        // Fecha os arquivos
        fclose(tableFile);
        // Fecha os arquivos de dados tabela
        if (stringsFile) {
            fclose(stringsFile);
        }
        free(pathString);
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

    // Aloca a quantidade de valores a serem excluidos
    int cont_values = 0;
    ResultList *list_search = searchResultList(resultTree, selection->tableName); // Recupera a pesquisa
    ResultList * aux = list_search; 
    while(list_search) {
        cont_values++;
        list_search = list_search->next;
    }

    // Verifica se a tabela existe
    int exists = tableExists(qtTables, selection->tableName);

    // Se a tabela existe
    if (exists) {
        // Recupera a pesquisa
        ResultList *list = aux;

        if (list) {
            // Tabela em questão
            Table table;
            // Path do arquivo da tabela
            char *path = glueString(2, TABLES_DIR, selection->tableName);
            // Abre o arquivo da tabela
            FILE *tableFile = fopenSafe(path, "rb+");

            // Path do arquivo de strings da tabela
            char *pathString = glueString(2, path, "_strings.bin");
            FILE *stringsFile = NULL;

            // Path do arquivo de binarios da tabela
            char *pathBinary = glueString(2, path, "_binaries.bin");
            FILE *binariesFile = NULL;

            // Path do arquivo de strings deletadas da tabela
            char *pathStringEmpty = glueString(2, path, "_strings.empty");
            FILE *stringsFileEmpty = NULL;

            // Path do arquivo de binarios deletados da tabela
            char *pathBinaryEmpty = glueString(2, path, "_binaries.empty");
            FILE *binariesFileEmpty = NULL;

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

                printf("MEIO 1.1\n");

                // Posição da string ou binário
                long int pos;
                // Remove as strings e binários
                for (int i = 0; i < table.cols; i++) {
                    // Remove dos arquivos exteriores + pula offsets
                    if (table.types[i] == 'i') {
                        fseek(tableFile, sizeof(int), SEEK_CUR);
                    } else if (table.types[i] == 's') {
                        if (!stringsFile) {
                            stringsFile = fopenSafe(pathString, "rb+");
                            stringsFileEmpty = fopenSafe(pathStringEmpty, "rb+");
                        }

                        fread(&pos, sizeof(long int), 1, tableFile);
                        printf("pos %ld\n", pos);
                        removeFromExFile(pos, stringsFile, stringsFileEmpty);
                    } else if (table.types[i] == 'f') {
                        fseek(tableFile, sizeof(float), SEEK_CUR);
                    } else if (table.types[i] == 'b') {
                        if (!binariesFile) {
                            binariesFile = fopenSafe(pathBinary, "rb+");
                            binariesFileEmpty = fopenSafe(pathBinaryEmpty, "rb+");
                        }

                        fread(&pos, sizeof(long int), 1, tableFile);
                        removeFromExFile(pos, binariesFile, binariesFileEmpty);
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
            // Fecha os arquivos de dados tabela
            if (stringsFile) {
                fclose(stringsFile);
                fclose(stringsFileEmpty);
            }
            if (binariesFile) {
                fclose(binariesFile);
                fclose(binariesFileEmpty);
            }
            free(pathString);
            free(pathBinary);
            free(pathStringEmpty);
            free(pathBinaryEmpty);

            // Remove o registro nos índices

            // Verifica se já existe um index hash ou tree para a tabela
            // se existir: remove o valor do índice
            if(tem_index_hash(selection->tableName, selection->field)) {    
                //TODO: remoção na hash
            }
            if(tem_index_tree(selection->tableName, selection->field)) {    
                // Remoção na árvore
                // BTree * tree = encontraBTree(selection->tableName);
                // for(int i = 0; i < cont_values; i++) btree_remove(tree, valor_index[i]);
            }
        } else {
            fprintf(stderr, "Não existe pesquisa para essa tabela!\n");
        }
    } else {
        fprintf(stderr, "Tabela não encontrada!\n");
    }
}

void criarIndex(Selection *selection) {
    if(tableExists(qtTables, selection->tableName)) {
        //se tipo de indexacao eh hash
        if(selection->parameter == 'H') {
            //se o campo existe na tabela
            if(fieldExistInTable(selection->tableName, selection->field)){
                //se o campo eh inteiro
                if(getFieldType(selection->tableName, selection->field) != 'i') { 
                    fprintf(stderr, "O campo %s não é do tipo INT.\n", selection->field);
                    return;
                }
                //se o indice ja existe
                if(tem_index_hash(selection->tableName, selection->field)) {
                    fprintf(stderr, "O campo %s já é indexado na tabela %s.\n", selection->field, selection->tableName);
                    return;
                }

                char * hashFilename = glueString(5, "tables_index/", selection->tableName, "_", selection->field, "_h.i");
                FILE * arquivoHash  = fopen(hashFilename, "wb"); // cria o arquivo do index

                inicializaArquivoHash(arquivoHash);

                char *tableFilename = glueString(2, TABLES_DIR, selection->tableName);
                FILE *arquivoTable  = fopenSafe(tableFilename, "r+b");

                Table table;
                fread(&table, sizeof(Table), 1, arquivoTable);

                fseek(arquivoHash, 0, SEEK_SET);

                // pra cada registro, insere a pos dele na hashtable usando o valor do field indexado como chave
                for (int i=0; i<table.rows; i++) {

                    int posRegistro = ftell(arquivoTable);

                    int valido;

                    fread(&valido, sizeof(int), 1, arquivoTable);

                    if (!valido) fseek(arquivoTable, table.length, SEEK_CUR);
                    else {
                        //passa por cada field do registro
                        for (int i=0; i<table.cols; i++) {
                            //se achou o campo indexado
                            if (strcmp(table.fields[i], selection->field) == 0) {

                                int valorDoCampo;
                                //le o valor 
                                fread(&valorDoCampo, sizeof(int), 1, arquivoTable);
                                //insere na hash o par (valorDoCampo, posRegistro)
                                insereArquivoHash(arquivoHash, valorDoCampo, posRegistro);

                            } //se nao eh o campo indexado vai para o proximo campo
                            else if (table.types[i] == 's') fseek(arquivoTable, sizeof(long int), SEEK_CUR);
                            else if (table.types[i] == 'i') fseek(arquivoTable, sizeof(int), SEEK_CUR);
                            else if (table.types[i] == 'f') fseek(arquivoTable, sizeof(float), SEEK_CUR);
                            else if (table.types[i] == 'b') fseek(arquivoTable, sizeof(long int), SEEK_CUR);
                        }
                    }
                }

                fclose(arquivoTable);
                fclose(arquivoHash);

            } else {
                fprintf(stderr, "O campo %s não existe na tabela %s.\n", selection->field, selection->tableName);
            }
        } else if (selection->parameter == 'A') { // se for index do tipo árvore
            if(fieldExistInTable(selection->tableName, selection->field)){ // verifica se o campo a ser indexado existe na tabela
                
                if(getFieldType(selection->tableName, selection->field) != 'i') { // verifica se o campo a ser indexado é do tipo inteiro
                    fprintf(stderr, "O campo %s não é do tipo INT.\n", selection->field);
                    return;
                }

                if(tem_index_tree(selection->tableName, selection->field)) { // verifica se já existe um index tree para a tabela (=> arquivo já existe)
                    fprintf(stderr, "O campo %s já é indexado na tabela %s.\n", selection->field, selection->tableName);
                    return;
                }

                char * filename = glueString(5, "tables_index/", selection->tableName, "_", selection->field, "_tree.index"); // elabora o nome do arquivo: tables_index/<nome-da-tabela>_tree.index

                fclose(fopen(filename, "w"));
/*              # TODO Perguntar se é necessário gerar o index em sua criação
                Btree * btree = new Btree(filename);

                // Lê os valores do arquivo da tabela e insere os pares (key, ftell(key)) no arquivo para serem utilizados pela btree

                // Tabela em questão
                Table table;
                // Path do arquivo da tabela
                char *path = glueString(2, TABLES_DIR, selection->tableName);
                // Abre o arquivo da tabela
                FILE *tableFile = fopenSafe(path, "rb+");
                
                // Lê os metadados
                fread(&table, sizeof(Table), 1, tableFile);

                fclose(tableFile);

                int bit_validade;
                int tam_pular = 0, tam_row = 0;
                int j = 0;

                // Descobre qual a posição (offset) do field a ser indexado
                while(strcmp(table.fields[j], selection->field)) {
                    switch(table.types[j++]) {
                        case 'i':
                            tam_pular += sizeof(int);
                            break;
                        case 's':
                            tam_pular += sizeof(long int);
                            break;
                        case 'f':
                            tam_pular += sizeof(float);
                            break;
                        case 'b':
                            tam_pular += sizeof(long int);
                            break;
                        default:
                            break;
                    }
                }

                // Lê os valores do campo a ser indexado e a posição do seu registro no arquivo
                pair_btree * pair = (pair_btree *) malloc(sizeof(pair_btree));
                int i_valido = 0;
                for(int i = 0; i < table.rows; i++) {
                    // Lê o bit de validades
                    fread(&bit_validade, sizeof(int), 1, tableFile);

                    if(bit_validade) {
                        // Se for valido adiciona nos pairs
                        pair->addr = ftell(tableFile) - sizeof(int);
                        fseek(tableFile, tam_pular, SEEK_CUR); // to do: otimizar
                        fread(&(pair->key), sizeof(int), 1, tableFile);
                        fseek(tableFile, -(tam_pular + sizeof(int)), SEEK_CUR);
                        printf("Adicionando pair na BTree: (%d %d)\n", pair->key, pair->addr);
                        btree->insert(*pair);
                    }
                    // Pula para o próximo registro
                    fseek(tableFile, table.length, SEEK_CUR);
                }

                // Delete na tree (necessário para chamar o construtor e manter a assinatura válida
                delete btree;

                // Libera memória
                free(pair);
*/
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

void removerIndex(TableName tableName, Field field, int imprime, int all) { // recebe o nome da tabela e um booleano que indica a impressão dos logs para o usuário

    if(all) {
        Table table = readTable(tableName);
        for(int i = 0; i < table.cols; i++) {
            char *filename_tree = glueString(5, "tables_index/", tableName, "_", table.fields[i], "_tree.index"); // elabora o nome do arquivo: tables_index/<nome-da-tabela>_tree.index
            char * filename_hash = glueString(5, "tables_index/", tableName, "_", table.fields[i], "_h.index"); // elabora o nome do arquivo: tables_index/<nome-da-tabela>_hash.index

            if (fileExist(filename_tree)) { // remove o árquivo de índice (árvore), caso ele exista
                removeFile(filename_tree);
            }
            if(fileExist(filename_hash)) { // remove o árquivo de índice (hash), caso ele exista
                removeFile(filename_hash);
            }
        }
    }

    char * filename_tree = glueString(5, "tables_index/", tableName, "_", field, "_tree.index"); // elabora o nome do arquivo: tables_index/<nome-da-tabela>_tree.index
    char * filename_hash = glueString(3, "tables_index/", tableName, "_", field, "_h.index"); // elabora o nome do arquivo: tables_index/<nome-da-tabela>_hash.index


    if(fileExist(filename_tree)) { // remove o árquivo de índice (árvore), caso ele exista
        removeFile(filename_tree);
        if(imprime) printf("Índice (árvore) removido.\n");
    }
    if(fileExist(filename_hash)) { // remove o árquivo de índice (hash), caso ele exista
        removeFile(filename_hash);
        if(imprime) printf("Índice (hash) removido.\n");
    }
    return;
}

void gerarIndex(Selection *selection) {
    if (tem_index_tree(selection->tableName, selection->field))
    {
        char * filename = glueString(3, "tables_index/", selection->tableName, "_", selection->field, "_tree.index"); // elabora o nome do arquivo: tables_index/<nome-da-tabela>_tree.index
        Btree * btree = new Btree(filename);
        
        // Lê os valores do arquivo da tabela e insere os pares (key, ftell(key)) no arquivo para serem utilizados pela btree

        // Tabela em questão
        Table table;
        // Path do arquivo da tabela
        char *path = glueString(2, TABLES_DIR, selection->tableName);
        // Abre o arquivo da tabela
        FILE *tableFile = fopenSafe(path, "rb+");
        // Lê os metadados
        fread(&table, sizeof(Table), 1, tableFile);

        int bit_validade;
        int tam_pular = 0, tam_row = 0;
        int j = 0;

        // Descobre qual a posição (offset) do field a ser indexado
        while (strcmp(table.fields[j], selection->field))
        {
            switch (table.types[j])
            {
            case 'i':
                tam_pular += sizeof(int);
                break;
            case 's':
                tam_pular += sizeof(long int);
                break;
            case 'f':
                tam_pular += sizeof(float);
                break;
            case 'b':
                tam_pular += sizeof(long int);
                break;
            default:
                break;
            }
        }

        // Lê os valores do campo a ser indexado e a posição do seu registro no arquivo
        pair_btree *pair = (pair_btree *)malloc(sizeof(pair_btree));
        int i_valido = 0;
        for (int i = 0; i < table.rows; i++)
        {
            // Lê o bit de validades
            fread(&bit_validade, sizeof(int), 1, tableFile);
            if (bit_validade)
            {
                // Se for valido adiciona nos pairs
                pair->addr = ftell(tableFile) - sizeof(int);
                fseek(tableFile, tam_pular, SEEK_CUR); // to do: otimizar
                fread(&(pair->key), sizeof(int), 1, tableFile);
                fseek(tableFile, -(tam_pular + sizeof(int)), SEEK_CUR);
                printf("Adicionando pair na BTree: (%d %d)\n", pair->key, pair->addr);
                btree->insert(*pair);
            }
            // Pula para o próximo registro
            fseek(tableFile, table.length, SEEK_CUR);
        }

        // Libera memória
        free(pair);
    }
    if (tem_index_hash(selection->tableName, selection->field))
    {
        //TODO: gera indice hash
    }
}

// Prepara o sistema
void start() {
    // Cria o diretório dos arquivos das tabelas
    mkdir(TABLES_DIR, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    // Cria o arquivo de indexação
    createFile(TABLES_INDEX);

    // Abre o arquivo de indexação
    tablesIndex = fopenSafe(TABLES_INDEX, "rb+");

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

    // Fecha o arquivo de indexação
    fclose(tablesIndex);

    freeResultTree(resultTree);
}
