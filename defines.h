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
extern const char STR[4];
extern const char BIN[4];
extern const char INT[4];
extern const char FLT[4];

extern const char STR_REP;
extern const char BIN_REP;
extern const char INT_REP;
extern const char FLT_REP;

// Parâmetros do programa
extern const char PFILE[3];
extern const char PFILEX[7];

extern const char PHELP[3];
extern const char PHELPX[7];

// Tamanho máximo
#define TABLE_NAME_MAX 32  // Tamanho máximo do nome da tabela
#define TYPE_MAX  4   // Tamanho máximo dos tipos de dados
#define FIELD_MAX 32  // Tamanho máximo do nome de um campo(chave)
#define VALUE_MAX 32  // Tamanho máximo de um valor
#define NUMBER_COLUMNS 32  // Número máximo de colunas

// Arquivos
#define TABLES_INDEX "table_index.tbli" // Arquivo de indexação das colunas
#define TABLES_DIR "tables_dir/" // Diretório dos arquivos de colunas
#define TABLE_FILE_EX ".tbl" // Extenção de arquivo um arquivo de tabela

// Erros
#define NONE        0x00

#define IN_ERROR    0x01 // Erro interno
#define NO_CMD      0x02 // Comando não encontrado

#define PRE_MISS_FL 0x02 // Nenhum arquivo especificado
#define PRE_WRG_PRM 0x02 // Parâmetro não reconhecido
#define PRE_MANY_FL 0x02 // Muitos arquivos

#define EXIT        0x03
#define TODO        0x0F

// SUCCESS      0x0
// WS           0x1 Wrong Sintax
// WS_USC       0x2 Wrong Sintax or Useless Semicolon
// USC          0x3 Useless Semicolon
// MP           0x4 Missing Parameter
// TBL_EXISTENT 0xE Table already exists
// FAILED       0xF

#define CT_SUCCESS   0x10
#define CT_WS        0x11
#define CT_WS_USC    0x12
#define CT_WRG_TYPE  0x1D
#define CT_TBL_EXT   0x1E
#define CT_FAILED    0x1F
#define RT_WS        0x21
#define AT_SUCCESS   0x30
#define AT_WS        0x31
#define AT_NOT_FOUND 0x3C
#define LT_SUCCESS   0x40
#define LT_FAILED    0x4f
#define IR_WS        0x51
#define IR_USC       0x53
#define BR_WS        0x61
#define BR_MP        0x64
#define AR_WS        0x71
#define RR_WS        0x81
#define CI_WS        0x91
#define CI_MP        0x94
#define RI_WS        0xA1
#define GI_WS        0xB1

//Typedefs
typedef char TableName[TABLE_NAME_MAX];

typedef char Type[TYPE_MAX];
typedef Type TypeArr[NUMBER_COLUMNS];
typedef char TypeRepArr[NUMBER_COLUMNS];

typedef char Field[FIELD_MAX];
typedef Field FieldArr[NUMBER_COLUMNS];

typedef char Value[VALUE_MAX];
typedef Value ValueArr[NUMBER_COLUMNS];

typedef int Error;

typedef struct {
	TableName name;
	TypeArr types;
	FieldArr fields;
	int cols;
} TableWType;

typedef struct {
	TableName name;
	TypeRepArr types;
	FieldArr fields;
	int cols;
} TableWRep;

typedef struct {
	TableName table_name;
	ValueArr values;
	int size;
} Row;

#endif /* DEFINES_H */