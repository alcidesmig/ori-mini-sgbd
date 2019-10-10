#include <stdio.h>
#include <stdlib.h>
#include "lista.h"
#include <string.h>

void inicializaLista(Noh ** raiz) {
	*raiz = NULL;
}

int insereLista(Noh ** raiz, Item i) {
	if(pesquisaLista(raiz, i.key) != NULL) {
		return 0;
	}

	Noh * p;
	
	p = (Noh *) malloc(sizeof(Noh));
	if (p == NULL) {
		printf("Out of memory\n");
		return -1;
	}
	
	p->item = i;
	p->prox = *raiz;
	*raiz = p;

	return 1;
}

Noh * pesquisaLista(Noh ** raiz, Chave key) {

	Noh * p;
	
	p = *raiz;
	while(p != NULL) {
		if (p->item.key == key)
			return p;
		p = p->prox;
	}
	
	return NULL;
}

void freeLista(Noh ** raiz) {

	Noh * p;
	
	p = *raiz;
	while(p != NULL) {
		Noh * save = p->prox;
		free(p);
		p = save;
	}

	return;
}

int pesquisaAtualizaLista(Noh ** raiz, Item item) {

	Noh * p;
	
	p = *raiz;
	while(p != NULL) {
		if (!strcmp(p->item.nome, item.nome)){
			p->item.telefone = item.telefone;
			return 1;
		}
		p = p->prox;
	}
	
	return 0;
}


int static removeListaPosicao(Noh ** raiz, Noh * p) {
	if (p == NULL)
		return 0;

	if (p == *raiz) {
		*raiz = (*raiz)->prox;
		free(p);
		return 1;
	}

	Noh * aux = *raiz;
	while (aux->prox != NULL && aux->prox != p) {
		aux = aux->prox;
	}
	
	aux->prox = p->prox;
	free(p);
	
	return 1;
}

int removeLista(Noh ** raiz, Chave key) {	
	Noh * p = pesquisaLista(raiz, key);	
	if(p == NULL) return 0;
	else removeListaPosicao(raiz, p);
	return 1;
}
