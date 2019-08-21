#ifndef PARSER_H
#define PARSER_H

// Identifica o comando
// command: String com a linha de comando em questão
// Lógica básica: Procura o comando e pula o ponteiro para após o mesmo, lê o nome da tabela, outro parâmetro até o ':' e outro até o ';' ou fim, repete
int parser(char *command);

#endif /* PARSER_H */