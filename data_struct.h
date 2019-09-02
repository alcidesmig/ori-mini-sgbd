#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include <string.h>

#include "tools.h"
#include "defines.h"

// Lista de resultados
struct result {
	struct result *next;
	char *row_raw;
};

typedef struct result Result;

// Árvore dicionário para armazenamento dos resultados
struct dnode {
	struct dnode *left;
	struct dnode *right;
	TableWRep *meta;
	Result *result_list;
};

typedef struct dnode Dnode;

// Adiciona os dados de uma row no início da lista
// result: Início de uma lista de resultados
// row_raw: dados adicionados à lista
// return: Novo início da lista
Result *addResult(Result *result, char *row_raw);

void freeResultList(Result *result);

// Adiciona uma lista de resultados à uma árvore de resultados
// root: Raiz da árvore
// name: Nome a ser associado ao resultado
// row_length: tamanho em bytes da row
// result: lista de resultados
// return: Raiz da nova lista
Dnode *addDnode(Dnode *root, TableWRep *meta, Result *result);

// Acha uma lista de resultados
// root: Árvore de resultados
// name: Nome associado à lista
// return: Ponteiro para lista
Dnode *findResultList(Dnode *root, char *name);

void freeDnodeTree(Dnode *root);

void printTree(Dnode *root);

#endif /* DATA_STRUCTURES_H */
