#ifndef DEFINES_H
#define DEFINES_H

// Pelo menos >= sizeof(long int)
#define BLOCK_SIZE 8
#define NUMBER_COLUMNS_LIMIT 32
#define TABLE_NAME_LIMIT 32
#define FIELD_NAME_LIMIT 32
#define NUM_ORDEM_BTREE 1024

typedef char TableName[TABLE_NAME_LIMIT];
typedef char Type;
typedef Type TypeArr[NUMBER_COLUMNS_LIMIT];
typedef char Field[FIELD_NAME_LIMIT];
typedef Field FieldArr[NUMBER_COLUMNS_LIMIT];
typedef void *Value;
typedef Value ValueArr[NUMBER_COLUMNS_LIMIT];

typedef struct Table {
    int cols;
    int rows;
    int length;
    TableName name;
    TypeArr types;
    FieldArr fields;
} Table;

typedef struct Row {
    int cols;
    TableName tableName;
    ValueArr values;
} Row;

typedef struct Selection {
    char parameter;
    TableName tableName;
    Field field;
    Value value;
} Selection;

typedef struct {
    int key;
    int addr;
} pair_btree;

extern const char INT[];
extern const char STR[];
extern const char FLT[];
extern const char BIN[];

extern int TYPE_LIMIT;

extern const int zero;
extern const int valido;
extern const int invalido;

#endif /* DEFINES_H */