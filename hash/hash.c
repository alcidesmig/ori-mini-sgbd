#include "hash.h"

static int hashFunc(int chave) {
	return ((int)pow(chave, 2)) % NUM_BALDES;
}

void inicializaArquivoHash(char *filename) {

    Balde balde;
    balde.qtdRegsNoBalde = 0;
    balde.posContinuacao = -1;

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

    long int posBaldeInsercao = ftell(arquivo);

    Balde balde;
    fread(&balde, sizeof(Balde), 1, arquivo);

    while (balde.qtdRegsNoBalde == MAX_REGS_POR_BALDE) { // enquanto esta num balde cheio

        if (balde.posContinuacao == -1) {   // se esse balde nao tem continuacao

            fseek(arquivo, -sizeof(Balde), SEEK_CUR);   // volta pro comeco do balde lido
            long int posAnterior = ftell(arquivo);      // guarda onde ele comeca

            fseek(arquivo, 0, SEEK_END);                // vai pro fim do arquivo
            posBaldeInsercao     = ftell(arquivo);      // eh onde vai comecar o novo balde

            balde.posContinuacao = posBaldeInsercao;    // atualiza continuacao do balde lido

            fseek(arquivo, posAnterior, SEEK_SET);      // volta pro inicio do balde lido
            fwrite(&balde, sizeof(Balde), 1, arquivo);  // atualiza o balde lido

            balde.qtdRegsNoBalde =  0;  // dados do novo balde
            balde.posContinuacao = -1;      

        } else {    // se esse balde ja tem continuacao

            fseek(arquivo, balde.posContinuacao, SEEK_SET); // pula para a continuacao
            posBaldeInsercao = ftell(arquivo);              // guarda comeco do balde de continuacao

            fread(&balde, sizeof(Balde), 1, arquivo);       // le o balde de continuacao
        }
    }

    balde.itens[balde.qtdRegsNoBalde].chave = chave;
    balde.itens[balde.qtdRegsNoBalde].valor = valor;
    balde.qtdRegsNoBalde++;

    fseek(arquivo, posBaldeInsercao, SEEK_SET);
    fwrite(&balde, sizeof(Balde), 1, arquivo);

    fclose(arquivo);
}

void buscaEmArquivoHash(char * filename, int chave, int limit, ResultList **resultList) {

    FILE * arquivo = fopen(filename, "rb");
    int posicao = hashFunc(chave);

    fseek(arquivo, posicao*sizeof(Balde), SEEK_SET);

    int achou = 0;
    bool terminou = false;
    while (!terminou)
    {
        Balde balde;
        fread(&balde, sizeof(Balde), 1, arquivo);

        for (int i=0; i<balde.qtdRegsNoBalde && achou < limit; i++) {

            if (chave == balde.itens[i].chave)
            {
                achou++;
                addToResultList(resultList, balde.itens[i].valor);
            }
        }

        if (balde.posContinuacao == -1 || achou >= limit)
            terminou = true;
        else
            fseek(arquivo, balde.posContinuacao, SEEK_SET);
    }

    fclose(arquivo);
}
