#include "commandsTools.hpp"

// Remove a BTree correspondente à tabela da lista de BTrees
void apagaBTree(TableName tableName, Field field) {
    removeLista(&lista_btree, tableName, field);
    return;
}

// Retorna a BTree correspondente à tabela
Btree * encontraBTree(TableName tableName, Field field) {
    return pesquisaLista(&lista_btree, tableName, field)->item.tree;
}

// Carrega uma Btree para a lista de Btrees
Btree * carregaBTree(TableName tableName, Field field) {
    if(pesquisaLista(&lista_btree, tableName, field) == NULL) {
        // Arquivo da BTree
        ItemBTree item_btree;
        strcpy(item_btree.field, field);
        strcpy(item_btree.tableName, tableName);
        item_btree.tree = new Btree(glueString(5, "tables_index/", tableName, "_", field, "_tree.index"));
        insereLista(&lista_btree, item_btree);
    }
    return (pesquisaLista(&lista_btree, tableName, field))->item.tree;
}

// Verifica se existe um índice hash para a tabela
int tem_index_hash(TableName tableName, Field field) {
    char * hashFilename = glueString(5, "tables_index/", tableName, "_", field, "_h.i");
    return fileExist(hashFilename);
}

// Verifica se existe um índice tree para a tabela
int tem_index_tree(TableName tableName, Field field) {
    char * treeFilename = glueString(5, "tables_index/", tableName, "_", field, "_tree.index");
    return fileExist(treeFilename);
}

// Antes de usar essa função, garanta que o campo exista na tablea utilizando a função fieldExistInTable(...)
Type getFieldType(TableName tableName, Field field) {
    // Path do arquivo da tabela
    char *path = glueString(2, TABLES_DIR, tableName);
    // Abre o arquivo da tabela
    FILE *tableFile = fopenSafe(path, "rb+");
    // Lê os metadados
    Table table;
    fread(&table, sizeof(Table), 1, tableFile);
    // Laço de repetição para encontrar a posição do campo desejado
    int i;
    for(i = 0; i < table.cols && (strcmp(table.fields[i], field)); i++);
    // Retorna o tipo do campo desejado baseado na sua posição
    return table.types[i];
}

int fieldExistInTable(char *name, Field field) {
    // Path do arquivo da tabela
    char *path = glueString(2, TABLES_DIR, name);
    // Abre o arquivo da tabela
    FILE *tableFile = fopenSafe(path, "rb+");
    // Lê os metadados
    Table table;
    fread(&table, sizeof(Table), 1, tableFile);
    // Variável que indicará se o campo existe na tabela
    int exist = 0;
    for(int i = 0; i < table.cols && !exist; i++) {
        exist = (strcmp(table.fields[i], field) == 0);
    }
    return exist;
}

int tableExists(int qtTables, char *name) {
    // Flag que representa a existência da tabela
    int exists = 0;

    // Pula para o começo dos nomes
    fseek(tablesIndex, sizeof(int), SEEK_SET);

    int i = 0;
    // Para cada tabela, enquanto é diferente
    while (exists == 0 && i < qtTables) {
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

            // Compara com o nome existente
            exists = (strcmp(buf, name) == 0);

            free(buf);

            // Incrementa só se a tabela é válida
            i++;
        } else {
            // Pula o espaço no caso de informações inválidas
            blocks *= -1;
            fseek(tablesIndex, blocks*BLOCK_SIZE, SEEK_CUR);
        }
    }

    return exists;
}

int tableNameIsUnique(int qtTables, char *name, long int *marker) {
    // Flag, novo nome é diferente
    int diff = 1;

    // Pula para o começo dos nomes
    fseek(tablesIndex, sizeof(int), SEEK_SET);

    int i = 0;
    // Para cada tabela, enquanto é diferente
    while (diff != 0 && i < qtTables) {
        // Número de blocos
        int blocks = 0;

        // Marcador da posição do igual ou última
        if (marker) {
            *marker = ftell(tablesIndex);
        }

        // Lê o número de blocos
        fread(&blocks, sizeof(int), 1, tablesIndex);

        // Se o espaço possuí informações válidas
        if (blocks > 0) {
            // Tamanho real do nome
            int size = blocks*BLOCK_SIZE;
            char *buf = (char *)mallocSafe(size);

            // Lê o nome
            fread(buf, size, 1, tablesIndex);

            // Compara com o novo nome
            diff = strcmp(buf, name);

            free(buf);

            // Incrementa só se a tabela é válida
            i++;
        } else {
            // Pula o espaço no caso de informações inválidas
            blocks *= -1;
            fseek(tablesIndex, blocks*BLOCK_SIZE, SEEK_CUR);
        }
    }

    return diff;
}

void addTableName(int qtTables, char *name) {
    qtTables++;

    // Pula para o começo do arquivo
    fseek(tablesIndex, 0, SEEK_SET);
    // Escreve o novo valor
    fwrite(&qtTables, sizeof(int), 1, tablesIndex);

    // Pula para o fim do arquivo
    fseek(tablesIndex, 0, SEEK_END);

    // Tamanho da string
    int length = strlen(name);
    // Quantidade de blocos
    int blocks = length/BLOCK_SIZE + (length%BLOCK_SIZE != 0 ? 1 : 0);

    // Escreve a quantidade de blocos
    fwrite(&blocks, sizeof(int), 1, tablesIndex);
    // Escreve a string
    fwrite(name, blocks*BLOCK_SIZE, 1, tablesIndex);
}

void loadEmptyList(FILE *eListFile, EmptyBlockList **list) {
    int qt = 0;

    fseek(eListFile, 0, SEEK_SET);
    fread(&qt, sizeof(int), 1, eListFile);

    EmptyBlock *block;

    for (int i = 0; i < qt; i++) {
        block = (EmptyBlock *)mallocSafe(sizeof(EmptyBlock));

        fread(block, sizeof(EmptyBlock), 1, eListFile);
        
        if (*list) {
            EmptyBlockList *temp = *list;
            
            while (temp->next) {
                temp = temp->next;
            }

            temp->next = (EmptyBlockList *)mallocSafe(sizeof(EmptyBlockList));
            temp = temp->next;
            temp->next = NULL;
            temp->data = block;
        } else {
            *list = (EmptyBlockList *)mallocSafe(sizeof(EmptyBlockList));
            (*list)->next = NULL;
            (*list)->data = block;
        }
    }
}

void saveEmptyList(FILE *eListFile, EmptyBlockList **list) {
    int qt = 0;
    fseek(eListFile, sizeof(int), SEEK_SET);

    while (*list) {
        fwrite((*list)->data, sizeof(EmptyBlock), 1, eListFile);
        free((*list)->data);
        EmptyBlockList *aux = *list;
        *list = (*list)->next;
        free(aux);
        qt++;
    }

    fseek(eListFile, 0, SEEK_SET);
    fwrite(&qt, sizeof(int), 1, eListFile);
}

long int addToExFile(char *str, FILE *dataFile, EmptyBlockList **list) {
    // Posição que a string foi escrita
    long int pos;
    // Tamanho da string
    int size = strlen(str);

    // Se a lista não está vazia e se o maior elemento é maior que a string
    if (*list && (*list)->data->size >= size) {
        // Pula para posição vazia
        fseek(dataFile, (*list)->data->pos, SEEK_SET);
        // Salva a posição da string
        pos = ftell(dataFile);
        // Escreve o tamanho da string
        fwrite(&size, sizeof(int), 1, dataFile);
        // Escreve a string
        fwrite(str, size, 1, dataFile);
        // Bloco removido
        EmptyBlockList *removed = *list;
        // Tira o primeiro bloco da lista
        *list = (*list)->next;
        // Atualiza o tamanho do bloco remanescente
        removed->data->size -= size;

        // Se o bloco ainda possuí espaço
        if (removed->data->size) {
            // Atualiza a posição
            removed->data->pos += size + sizeof(int);
            // Grava o novo tamanho, logo após a última string
            fwrite(&(removed->data->size), sizeof(int), 1, dataFile);
            // Coloca o bloco na lista
            addInOrderToEBList(list, removed->data->pos, removed->data->size);
        }
        // Libera o bloco de qualquer forma
        free(removed->data);
        free(removed);
    // Se a lista está vazia ou o maior bloco não é suficiente
    } else {
        // Pula para o fim
        fseek(dataFile, 0, SEEK_END);
        // Salva a posição da string
        pos = ftell(dataFile);
        // Escreve o tamanho da string
        fwrite(&size, sizeof(int), 1, dataFile);
        // Escreve a string
        fwrite(str, size, 1, dataFile);
    }

    return pos;
}

void removeFromExFile(long int pos, FILE *dataFile, EmptyBlockList **list) {
    // Tamanho do bloco
    int size;
    // Pula para o bloco que será removida
    fseek(dataFile, pos, SEEK_SET);
    // Salva o tamanho do bloco
    fread(&size, sizeof(int), 1, dataFile);

    // Coloca o bloco na lista
    addInOrderToEBList(list, pos, size);
}

void addInOrderToEBList(EmptyBlockList **list, long int pos, int size) {
    EmptyBlockList *newBlock = (EmptyBlockList *)mallocSafe(sizeof(EmptyBlockList));
    newBlock->data = (EmptyBlock *)mallocSafe(sizeof(EmptyBlock));
    newBlock->data->pos = pos;
    newBlock->data->size = size;

    if (*list) {
        EmptyBlockList *parent = NULL;
        EmptyBlockList *temp = *list;

        while (temp && temp->data->size > newBlock->data->size) {
            parent = temp;
            temp = temp->next;
        }

        if (parent) {
            newBlock->next = parent->next;
            parent->next = newBlock;
        } else {
            newBlock->next = temp;
            *list = newBlock;
        }
    } else {
        *list = newBlock;
    }
}
