#include "commandsTools.h"


// Função para salvar todas as BTrees antes de fechar o programa
void salvaBTrees(Noh * lista_btree) {
    while(lista_btree) {
        salvarBTree(lista_btree->item.key);
        lista_btree = lista_btree->prox;
    }
}

// Função para salvar os pairs da BTree no arquivo de index antes de fechar o programa
void salvarBTree(TableName tableName) {
    // Path do arquivo da tabela
    char *path = glueString(2, TABLES_DIR, tableName);
    // Abre o arquivo da tabela
    FILE *tableFile = fopenSafe(path, "rb+");
    if(tableFile == NULL) {
        fprintf(stderr, "Erro ao salvar os dados de indexação de %s!\n", tableName);
    }
    // Lê os metadados
    Table table;
    fread(&table, sizeof(Table), 1, tableFile);
    // Fecha o arquivo
    fclose(tableFile);
    // Armazena a quantidade de registros
    int qtRegistros = table.rows;
    // Aloca espaço para leitura dos pares via DFS
    pair_btree * pairs = (pair_btree *) malloc(sizeof(pair_btree) * qtRegistros);
    // Encontra a BTree correspondente
    BTree * tree = encontraBTree(tableName);
    // DFS
    btree_dfs(tree, pairs);
    // Grava valores no arquivo da BTree
    char * filename = glueString(3, "tables_index/", tableName, "_tree.index"); 
    // Pega o Field indexado
    Field field_indexado;
    FILE * table_index = fopen(filename, "rb+");
    if(table_index == NULL) {
        fprintf(stderr, "Erro ao salvar os dados de indexação de %s!\n", tableName);
    }
    fread(&field_indexado, sizeof(Field), 1, table_index);
    fclose(table_index); // to do: mudar para append no arquivo anterior sobrescrevendo os dados dos pares e qtdRegistros
    // Reescreve o arquivo de index
    table_index = fopen(filename, "wb+");
    if(table_index == NULL) {
        fprintf(stderr, "Erro ao salvar os dados de indexação de %s!\n", tableName);
    }
    fwrite(&field_indexado, sizeof(Field), 1, table_index);
    // Grava qt de registros indexados
    fwrite(&qtRegistros, sizeof(int), 1, table_index);
    printf("Qtd registros salvos btree file: %d\n", qtRegistros);
    // Grava os valores (key, addr)
    fwrite(pairs, sizeof(pair_btree), qtRegistros, table_index); 
    // Fecha o arquivo
    fclose(table_index);
    return;
}

// Remove a BTree correspondente à tabela da lista de BTrees
void apagaBTree(TableName tableName) {
    // to do: free na BTree
    removeLista(&lista_btree, tableName);
    return;
}

// Retorna a BTree correspondente à tabela
BTree * encontraBTree(TableName tableName) {
    return pesquisaLista(&lista_btree, tableName)->item.tree;
}

// Carrega os dados da BTree de uma tabela caso eles ainda não tenha sido carregaods
void carregaBTree(TableName tableName) {
    if(haveIndexTree(tableName) && pesquisaLista(&lista_btree, tableName) == NULL) {
        // Arquivo da BTree
        char * filename = glueString(3, "tables_index/", tableName, "_tree.index"); 
        FILE * fp = fopen(filename, "r");
        // Pula o nome do campo indexado
        fseek(fp, sizeof(Field), SEEK_SET);
        // Lê a quantidade de itens indexados
        int qtdBTree;
        fread(&qtdBTree, sizeof(int), 1, fp);
        // Lê os itens indexados
        pair_btree * values = (pair_btree *) malloc(sizeof(pair_btree) * qtdBTree);
        fread(values, sizeof(pair_btree), qtdBTree, fp);
        // Cria, povoa a BTree e insere ela na lista de BTrees carregadas
        ItemBTree item_btree;
        strcpy(item_btree.key, tableName);
        item_btree.tree = btree_new(NUM_ORDEM_BTREE);
        
        for(int i = 0; i < qtdBTree; i++) {
            btree_insert(item_btree.tree, values[i].key, &values[i].addr);
            printf("Inserido item na BTree %d %d\n", values[i].key, values[i].addr);
        }
        insereLista(&lista_btree, item_btree);
    }
}

// Verifica se existe um índice hash para a tabela
int haveIndexHash(TableName tableName) {
    char * filename = glueString(3, "tables_index/", tableName, "_hash.index"); 
    return fileExist(filename);
}

// Verifica se existe um índice tree para a tabela
int haveIndexTree(TableName tableName) {
    char * filename = glueString(3, "tables_index/", tableName, "_tree.index"); 
    return fileExist(filename);
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