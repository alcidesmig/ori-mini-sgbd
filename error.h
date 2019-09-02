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
#define CANT_REMOVE_FILE   0x0007
#define TODO               0x00f0
#define EXIT               0x00f1
#define CT_WRONG_SINTAX    0x0101
#define CT_WRONG_TYPE      0x0103 // Tipo da coluna não suportado
#define CT_TABLE_EXISTS    0x0104 // Tabela la existe
#define RT_WRONG_SINTAX    0x0201
#define RT_CANT_FIND_TABLE 0x0205 // Tabela não existe
#define AT_WRONG_SINTAX    0x0301
#define AT_CANT_FIND_TABLE 0x0305 // Tabela não existe
// LT 0x04
#define IR_WRONG_SINTAX    0x0501
#define IR_DIFF_PARAM_NUMB 0x0506
#define IR_WRONG_VALUE     0x0507
#define IR_WRONG_TABLE     0x0508
#define BR_WRONG_SINTAX    0x0601
#define BR_WRONG_PARAMETER 0x0602
#define BR_WRONG_TYPE      0x0603
#define BR_WRONG_VALUE     0x0607
#define BR_WRONG_TABLE     0x0608
#define BR_FIELD_NFOUND    0x0609
#define AR_WRONG_SINTAX    0x0701
#define AR_WRONG_TABLE     0x0708
#define RR_WRONG_SINTAX    0x0801
#define CI_WRONG_SINTAX    0x0901
#define CI_WRONG_PARAMETER 0x0902
#define RI_WRONG_SINTAX    0x0a01
#define GI_WRONG_SINTAX    0x0b01

void raiseError(Error error);

#endif /* ERROR_H */
