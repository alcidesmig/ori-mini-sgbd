#ifndef DEFINES_H
#define DEFINES_H

// Comandos
#define CT "CT"
#define RT "RT"
#define AT "AT"
#define LT "LT"
#define IR "IR"
#define BR "BR"
#define AR "AR"
#define RR "RR"
#define CI "CI"
#define RI "RI"
#define GI "GI"
#define EB "EB"

// Parâmetros do BR
#define U "U"
#define N "N"
#define A "A"
#define H "H"

// Tipos
#define STR "STR"
#define BIN "BIN"
#define INT "INT"
#define FLT "FLT"

// Tamanho máximo
#define CMD_MAX   256 // Tamanho máximo da linha de comando
#define TABLE_NAME_MAX 32  // Tamanho máximo do nome da tabela
#define TYPE_MAX  4   // Tamanho máximo dos tipos de dados
#define FIELD_MAX 32  // Tamanho máximo do nome de um campo(chave)
#define VALUE_MAX 32  // Tamanho máximo de um valor
#define NUMBER_COLUMNS 32  // Número máximo de colunas

// Marcadores
#define TABLE_NAME 0x10

// Arquivos
#define TABLES_INDEX "table_index.tbli" // Arquivo de indexação das colunas
#define TABLE_FILE_EX ".tbl" // Extenção de arquivo um arquivo de tabela

// Erros
// WS  0x01 Wrong Sintax
// USC 0x02 Useless Semicolon
// MP  0x04 Missing Parameter
#define NONE      0x00
#define IN_ERROR  0x10
#define NO_CMD    0x20
#define CT_WS     0x31
#define CT_WS_USC 0x33
#define RT_WS     0x41
#define AT_WS     0x51
#define IR_WS     0x61
#define IR_USC    0x62
#define BR_WS     0x71
#define BR_MP     0x74
#define AR_WS     0x81
#define RR_WS     0x91
#define CI_WS     0xb1
#define CI_MP     0xa4
#define RI_WS     0xc1
#define GI_WS     0xd1
#define EXIT      0xe0

// Tipos
typedef char TableName[TABLE_NAME_MAX];
typedef char Type[TYPE_MAX];
typedef char Field[FIELD_MAX];
typedef char Value[VALUE_MAX];

typedef Type TypeArr[NUMBER_COLUMNS];
typedef Field FieldArr[NUMBER_COLUMNS];
typedef Value ValueArr[NUMBER_COLUMNS];

#endif /* DEFINES_H */