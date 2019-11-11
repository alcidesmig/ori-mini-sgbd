#include "hash.h"

static int hashFunc(int chave) {
	return ((int)pow(chave, 2)) % NUM_BALDES;
}

void inicializaArquivoHash(char *filename) {

    Balde balde;
    balde.qtdRegsNoBalde = 0;

    FILE *arquivo;
    arquivo = fopen(filename, "wb");

    for(int i=0; i<NUM_BALDES; i++)
    {
        fwrite(&balde, sizeof(Balde), 1, arquivo);
    }

    fclose(arquivo);
}

void insereArquivoHash(char * filename, int chave, int valor) {

    FILE *arquivo = fopen(filename, "r+b");
    int posicao = hashFunc(chave);
    
    fseek(arquivo, posicao*sizeof(Balde), SEEK_SET);

    Balde balde;
    fread(&balde, sizeof(Balde), 1, arquivo);

    balde.itens[balde.qtdRegsNoBalde].chave = chave;
    balde.itens[balde.qtdRegsNoBalde].valor = valor;
    balde.qtdRegsNoBalde++;

    fseek(arquivo, posicao*sizeof(Balde), SEEK_SET);
    fwrite(&balde, sizeof(Balde), 1, arquivo);

    fclose(arquivo);
}

void buscaEmArquivoHash(char * filename, int chave, int limit, ResultList **resultList) {

    FILE * arquivo = fopen(filename, "rb");
    int posicao = hashFunc(chave);

    fseek(arquivo, posicao*sizeof(Balde), SEEK_SET);

    Balde balde;
    fread(&balde, sizeof(Balde), 1, arquivo);

    int achou = 0;
    for (int i=0; i<balde.qtdRegsNoBalde && achou < limit; i++) {

        if (chave == balde.itens[i].chave)
        {
            achou++;
            addToResultList(resultList, balde.itens[i].valor);
        }
    }
}
