#ifndef ERROR_H
#define ERROR_H

#include <stdio.h>
#include <stdlib.h>

typedef int Error;

#define NONE               0x0000
#define IN_ERROR           0x0001
#define UNSUPORTED_PARAM   0x0002 // Parâmetros do programa incompletos
#define NO_COMMAND         0x0003 // Comando não suportado
#define MALLOC_ERROR       0x0004
#define FOPEN_ERROR        0x0005
#define STRCAT_ERROR       0x0006
#define TODO               0x00f0
#define EXIT               0x00f1
#define CT_WRONG_SINTAX    0x0101
#define CT_WRONG_TYPE      0x0103 // Tipo da coluna não suportado
#define CT_TABLE_EXISTS    0x0104 // Tabela la existe
#define RT_WRONG_SINTAX    0x0201
#define AT_WRONG_SINTAX    0x0301
#define AT_CANT_FIND_TABLE 0x0305 // Tabela não existe
// LT 0x04
#define IR_WRONG_SINTAX    0x0501
#define BR_WRONG_SINTAX    0x0601
#define BR_WRONG_PARAMETER 0x0602
#define AR_WRONG_SINTAX    0x0701
#define RR_WRONG_SINTAX    0x0801
#define CI_WRONG_SINTAX    0x0901
#define CI_WRONG_PARAMETER 0x0902
#define RI_WRONG_SINTAX    0x0a01
#define GI_WRONG_SINTAX    0x0b01

void raiseError(Error error);

#endif /* ERROR_H */