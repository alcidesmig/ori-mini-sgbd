#ifndef GETLINE_CUSTOM_H
#define GETLINE_CUSTOM_H

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>

extern struct termios TERM_CONFIG;
extern struct termios TERM_CONFIG_OLD;
extern int INSERT_MODE;
extern int TERM_READY;

// Move o cursor do terminal
void move_cursor_rigth(int i);

// Move o cursor do terminal
void move_cursor_left(int i);

// Prepara o terminal
void term_init();

// Resteura as configurações antigas do terminal
void term_close();

// Sai do programa e restaura o terminal
void exit_term_close(int n);

// Verifica se um char UTF8 é ASCCI
int is_ASCII_char(char c);

// Verifica se um char UTF8 é o primeiro de uma sequência
int is_marker_char(char c);

// Verifica se um char UTF8 é o payload de uma sequência
int is_payload_char(char c);

// Move à esquerda a posição corrente em uma string
int move_left(char *line, int pos);

// Move à direita a posição corrente em uma string
int move_right(char *line, int pos);

// Coloca um char de vários bytes em uma string
void put_multichar(char *line, char *sub, int size, int ssize, int place);

// Varifica o tamanho de um char
int get_multichar_length(char c);

// Remove um char de vários bytes de uma string
int remove_multichar(char *line, int size, int place);

// Troca um char de vários bytes por outro
int replace_multichar(char *line, char *sub, int size, int ssize, int place);

// Cuida de todos os inputs que o programa pode receber,
//   retorna strings como getline(),
//   dá funcionalidade ás seta, backspace, delete, insert,
//   não polui a entrada com outras teclas
int getline_custom(char **lineptr, ssize_t *n);

#endif /* GETLINE_CUSTOM_H */
