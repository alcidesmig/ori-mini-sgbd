#include "commandsTools.h"

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
    // Escreve a strign
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
        printf("%d\n", qt);
    }

    fseek(eListFile, 0, SEEK_SET);
    fwrite(&qt, sizeof(int), 1, eListFile);
}

long int addToExFile(char *str, FILE *dataFile, EmptyBlockList **list) {
    // Posição que a string foi escrita
    long int pos;
    // Tamanho da string
    int size = strlen(str);

    // Se a lista não está vazia e se o meior elemento é maior que a string
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