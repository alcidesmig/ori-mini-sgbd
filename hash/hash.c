#include "hash.h"

static int hashFunc(int chave) {
	return ((int)pow(chave, 2)) % TAM_HASHTABLE;
}

void inicializaArquivoHash(FILE * arquivo) {

    Balde balde;
    balde.qtdRegsNoBalde = 0;

    for(int i=0; i<TAM_HASHTABLE; i++)
    {
        fwrite(&balde, sizeof(Balde), 1, arquivo);
    }
}

void insereArquivoHash(FILE *arquivo, int chave, int valor) {

    int posicao = hashFunc(chave);
    
    Balde balde;

    fseek(arquivo, posicao*sizeof(Balde), SEEK_SET);
    fread(&balde, sizeof(Balde), 1, arquivo);

    balde.itens[balde.qtdRegsNoBalde].chave = chave;
    balde.itens[balde.qtdRegsNoBalde].valor = valor;
    balde.qtdRegsNoBalde++;

    fwrite(&balde, sizeof(Balde), 1, arquivo);
}

int buscaEmArquivoHash(int chave, char * filename) {

    FILE * arquivo = fopen(filename, "rb");

    int posicao = hashFunc(chave);

    Balde balde;

    fseek(arquivo, posicao*sizeof(Balde), SEEK_SET);
    fread(&balde, sizeof(Balde), 1, arquivo);

    for (int i=0; i<balde.qtdRegsNoBalde; i++)
    {
        if (chave == balde.itens[i].chave)
            return balde.itens[i].valor;
    }

    return 0;
}

// http://www.cse.yorku.ca/~oz/hash.html

/*
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
*/
