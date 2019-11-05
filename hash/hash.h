#ifndef HASH_H
#define HASH_H

//#include "lista.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#define MAX_REGS 63
#define TAM_LIXO 4

#define TAM_HASHTABLE 31

typedef struct Item {
    int chave;
    int valor;
} Item;

typedef struct Balde {
    int qtdRegsNoBalde;   // 32      = 32   +
    Item itens[MAX_REGS]; // 64 * 63 = 4032 +
    char lixo[TAM_LIXO];  // 8 * 4   = 32     = 4096 bits
} Balde;

void insereArquivoHash(FILE *arquivo, int chave, int valor);
void inicializaArquivoHash(FILE *arquivo);
int buscaEmArquivoHash(int chave, char * filename);

/*
void initHash(Noh ** lista);
void destroyHash(Noh ** lista);
int insertHash(Noh ** lista, Item i);
Item *searchHash(Noh ** lista, Chave key);
int updateHashItem(Noh ** lista, Item item);
int removeHash(Noh ** lista, Chave key);
*/

#endif
