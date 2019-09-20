#ifndef DEFINES_H
#define DEFINES_H

#define NUMBER_COLUMNS_LIMIT 32

typedef char *TableName;
typedef char Type;
typedef Type TypeArr[NUMBER_COLUMNS_LIMIT];
typedef char *Field;
typedef Field FieldArr[NUMBER_COLUMNS_LIMIT];
typedef void *Value;
typedef Value ValueArr[NUMBER_COLUMNS_LIMIT];

typedef struct Table {
    int cols;
    TableName name;
    TypeArr types;
    FieldArr fields;
    int rows;
} Table;

typedef struct Row {
    int cols;
    TableName tableName;
    ValueArr values;
} Row;

typedef struct {
    char *command;
    union {
        Table table;
        Row row;
    } data;
} ParsedData;

extern const char INT[];
extern const char STR[];
extern const char FLT[];
extern const char BIN[];

extern int TYPE_LIMIT;

#endif /* DEFINES_H */