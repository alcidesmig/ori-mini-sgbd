#ifndef LISTA_H
#define LISTA_H

#include "btree.h"
#include "../defines.h"

typedef TableName Chave;

typedef struct {
	BTree * tree;
	TableName key;
} ItemBTree;

typedef struct no {
	ItemBTree item;
	struct no * prox;
} Noh;

extern Noh * lista_btree;

void inicializaLista(Noh ** raiz);
Noh * pesquisaLista(Noh ** raiz, TableName key);
void freeLista(Noh ** raiz);
int pesquisaAtualizaLista(Noh ** raiz, ItemBTree item);
int insereLista(Noh ** raiz, ItemBTree i);
int removeLista(Noh ** raiz, Chave key);

#endif