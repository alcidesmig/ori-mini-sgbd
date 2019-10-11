#include "hash.h"
#include "lista.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// http://www.cse.yorku.ca/~oz/hash.html
static int hashFunc(Chave key) {
	return key << 10 % TAM;
}

void initHash(Noh ** lista) {
	for (int i = 0; i < TAM; i++) {
		inicializaLista(&(lista[i]));
	}
}

void destroyHash(Noh ** lista) {
	for (int i = 0; i < TAM; i++) {
		freeLista(&(lista[i]));
	}
}

int insertHash(Noh ** lista, Item i) {
	int pos = hashFunc(i.key);
	return insereLista(&lista[pos], i);
}

Item *searchHash(Noh ** lista, Chave key) {
	int pos = hashFunc(key);
	Noh * no = pesquisaLista(&lista[pos], key);
	
	if (no == NULL) {
		Item *x = (Item *)malloc(sizeof(Item));
		x->key = -1;
		return x;
	}
	return &(no->item);
}

int updateHashItem(Noh ** lista, Item item) {
	int pos = hashFunc(item.key);
	return (pesquisaAtualizaLista(&lista[pos], item));
}

int removeHash(Noh ** lista, Chave key) {
	int pos = hashFunc(key);
	return removeLista(&lista[pos], key); 
}
