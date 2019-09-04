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

// Tamanho máximo
#define CMD_MAX 3         // Tamanho máximo de um comando
#define PARAMETER_MAX 2   // Tamanho máximo de um paramêtro
#define TABLE_NAME_MAX 32 // Tamanho máximo do nome da tabela
#define TYPE_MAX  4       // Tamanho máximo dos tipos de dados
#define FIELD_MAX 32      // Tamanho máximo do nome de um campo(chave)
#define VALUE_MAX 32      // Tamanho máximo de um valor
#define NUMBER_COLUMNS 32 // Número máximo de colunas
#define STR_SIZE 256      // Número máximo de colunas
#define BIN_FILE_NAME_SIZE 256      // Número máximo de colunas


// Typedefs
typedef char TableName[TABLE_NAME_MAX];

typedef char Type[TYPE_MAX];
typedef Type TypeArr[NUMBER_COLUMNS];
typedef char TypeRepArr[NUMBER_COLUMNS];

typedef char Field[FIELD_MAX];
typedef Field FieldArr[NUMBER_COLUMNS];

typedef char Value[VALUE_MAX];
typedef Value ValueArr[NUMBER_COLUMNS];

typedef struct {
	TableName name;
	TypeArr types;
	FieldArr fields;
	int cols;
} TableWType;

typedef struct {
	int row_bytes_size;
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

// Arquivos
#define TABLES_INDEX "table_index.tbli" // Arquivo de indexação das colunas
#define TABLES_DIR "tables_dir/" // Diretório dos arquivos de colunas
#define TABLE_EXTENSION ".tbl" // Extenção de arquivo um arquivo de tabela
#define BINARY_TABLE "binary_data.bin"

// Tipos
extern const char STR[4];
extern const char BIN[4];
extern const char INT[4];
extern const char FLT[4];

extern const char STR_REP;
extern const char BIN_REP;
extern const char INT_REP;
extern const char FLT_REP;


// BIN_SIZE = tamanho de um long int -> Posição do valor no arquivo de binários
extern const long int BIN_SIZE;

// Scanf Strings

extern char CMD_SCANF[16];
extern char TBL_NAME_SCANF[16];
extern char FIELD_NAME_SCANF[16];
extern char PARAMETER_SCANF[16];
extern char FIELD_NAME_VALUE_SCANF[16];
extern char TYPE_FIELD_SCANF[32];
extern char VALUE_SCANF[16];

#endif /* DEFINES_H */