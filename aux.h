#ifndef AUX_H
#define AUX_H

//Função que recebe uma string e quebra ela em pedaços separados pelo splitter. Retorno: vetor de palavras
char ** split(char * str, char splitter);

// Função para transformar as letras de uma string em letras maíusculas
void toUpperCase(char * str);

// Procura em uma string e retorna NULL ou o ponteiro após a string procurada
// Inclui o limit, número de chars sem matching
// limit == -1 default, equivalente a limite infinito
char *findl(char *haystack, const char *needle, int limit);

char *find(char *haystack, const char *needle);

#endif /* AUX_H */