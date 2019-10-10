#ifndef LISTA_H
#define LISTA_H

typedef int Chave;

typedef struct {
	Chave key;
	unsigned long int addr;
} Item;

typedef struct no {
	Item item;
	struct no * prox;
} Noh;

void inicializaLista(Noh ** raiz);
Noh * pesquisaLista(Noh ** raiz, Chave key);
void freeLista(Noh ** raiz);
int pesquisaAtualizaLista(Noh ** raiz, Item item);
int insereLista(Noh ** raiz, Item i);
int removeLista(Noh ** raiz, Chave key);

#endif