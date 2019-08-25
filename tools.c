#include "tools.h"

int init() {
    // Index de tabelas
    fclose(fopen(TABLES_INDEX, "ab"));

    // Cria o diretório dos arquivos de tabelas
    mkdir(TABLES_DIR, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    return 1;
}

void *safe_malloc(size_t size) {
    void *p = malloc(size);

    if (p)
        return p;

    raiseError(MALLOC_ERROR);
}

FILE *safe_fopen(char *name, char *mode) {
    FILE *p = fopen(name, mode);

    if (p)
        return p;

    raiseError(FOPEN_ERROR);
}

char *safe_strcat(char *dest, char *src) {
    char *p = strcat(dest, src);
    
    if (p)
        return p;

    raiseError(STRCAT_ERROR);
}

// Lê a quantidade de tabelas
// tables_index: Arquivo de index das tabelas
int read_qt_tables(FILE *tables_index) {
    int qt_tables = 0;

    if(!fread(&qt_tables, sizeof(int), 1, tables_index)) {
        qt_tables = 0;
        fwrite(&qt_tables, sizeof(int), 1, tables_index);
    }

    return qt_tables;
}

// Escreve a quantidade de tabelas
// tables_index: Arquivo de index das tabelas
// qt_tables: Quantidade de tabelas
void write_qt_tables(FILE *tables_index, int qt_tables) {
    fseek(tables_index, 0, SEEK_SET);
    fwrite(&qt_tables, sizeof(int), 1, tables_index);
}

// Lê os nomes das tabelas
// tables_index: Arquivo de index das tabelas
// qt_tables: Quantidade de tabelas
TableName *read_tables_names(FILE *tables_index, int qt_tables) {
    TableName *names = safe_malloc(qt_tables * sizeof(TableName));

    fseek(tables_index, sizeof(int), SEEK_SET);
    fread(names, sizeof(TableName), qt_tables, tables_index);

    return names;
}

// Escreve os nomes das tabelas
// tables_index: Arquivo de index das tabelas
// names: Lista dos nomes
// qt_tables: Quantidade de tabelas
void write_tables_names(FILE *tables_index, TableName *names, int qt_tables) {
    fseek(tables_index, sizeof(int), SEEK_SET);
    fwrite(names, sizeof(TableName), qt_tables, tables_index);
}

// Lê os metadados de uma tabela, se ela existir
// tableName: Nome da tabela a ser lida
TableWRep *read_table_metadata(TableName tableName) {
    TablePath path = "";

    safe_strcat(path, TABLES_DIR);
    safe_strcat(path, tableName);
    safe_strcat(path, TABLE_EXTENSION);

    FILE *table_file = fopen(path, "rb+");
    if (!table_file) {
        return NULL;
    }

    TableWRep *table = safe_malloc(sizeof(TableWRep));
    fread(table, sizeof(TableWRep), 1, table_file);
    
    fclose(table_file);

    return table;
}

// Escreve os metadados de uma tabela no seu arquivo
// tables_index: Arquivo de index das tabelas
// table: tabela a ser gravada
// index: Posição da tabela
void write_table_metadata(FILE *tables_index, TableWRep *table, int index) {
    TablePath path = "";

    safe_strcat(path, TABLES_DIR);
    safe_strcat(path, table->name);
    safe_strcat(path, TABLE_EXTENSION);

    fclose(safe_fopen(path, "ab"));
    FILE *table_file = safe_fopen(path, "rb+");
    fwrite(table, sizeof(TableWRep), 1, table_file);
    fclose(table_file);

    fseek(tables_index, sizeof(int) + index * sizeof(TableName), SEEK_SET);
    fwrite(table->name, sizeof(TableName), 1, tables_index);
}

void safe_remove(char *path) {
    if (remove(path)) {
        raiseError(CANT_REMOVE_FILE);
    }
}

void toUpperCase(char *str) {
    int i = -1;
    while (str[++i] != '\0')
        if (str[i] >= 'a' && str[i] <= 'z') str[i] = str[i] - 32;
}

int replaceSpace(char *str, char c) {
    int flag = 0;

    for(int i = 0; str[i]; i++) {
        if (str[i] == ' ') {
            str[i] = c;
            flag = 1;
        }
    }

    return flag;
}

int glueChars(char *str, char c) {
    int flag = 0;

    int i = 0;
    for (; str[i]; i++);

    char *temp = safe_malloc(i*sizeof(char));
    int ti = 0;

    for (i = 0; str[i]; i++) {
        if (str[i] != c) {
            temp[ti++] = str[i];
        } else {
            flag = 1;
        }
    }

    for (i = 0; i < ti; i++) {
        str[i] = temp[i];
    }

    str[i] = '\0';

    return flag;
}

// Verifica a existência de uma tabela com o nome especificado
// name: Nome da tabela
int tableNameExists(TableName *names, char *name, int qt_tables) {
    for (int i = 0; i < qt_tables; i++) {
        if(strcmp(names[i], name) == 0) {
            return 1;
        }
    }

    return 0;
}

// Converte uma TableWType para uma TableWRep
int convertToRep(TableWRep *tableR, TableWType *tableT) {
    strncpy((*tableR).name, (*tableT).name, sizeof(TableName));
    (*tableR).cols = (*tableT).cols;
    (*tableR).row_bytes_size = 0;

    int s = (*tableT).cols;

    for (int i = 0; i < s; i++) {
        if(strcmp((*tableT).types[i], STR) == 0) {
            (*tableR).row_bytes_size += STR_SIZE;
            (*tableR).types[i] = STR_REP;
        } else if(strcmp((*tableT).types[i], INT) == 0) {
            (*tableR).row_bytes_size += sizeof(int);
            (*tableR).types[i] = INT_REP;
        } else if(strcmp((*tableT).types[i], FLT) == 0) {
            (*tableR).row_bytes_size += sizeof(float);
            (*tableR).types[i] = FLT_REP;
        } else if(strcmp((*tableT).types[i], BIN) == 0) {
            (*tableR).row_bytes_size += BIN_SIZE;
            (*tableR).types[i] = BIN_REP;
        } else {
            return 0;
        }

        strncpy((*tableR).fields[i], (*tableT).fields[i], sizeof(Field));
    }

    return 1;
}

// Converte uma TableWRep para uma TableWType
int convertToType(TableWType *tableT, TableWRep *tableR) {
    strncpy((*tableT).name, (*tableR).name, sizeof(TableName));
    (*tableT).cols = (*tableR).cols;
    
    int s = (*tableR).cols;

    for (int i = 0; i < s; i++) {
        if((*tableR).types[i] == STR_REP)
            strncpy((*tableT).types[i], STR, sizeof(Type));
        else if((*tableR).types[i] == INT_REP)
            strncpy((*tableT).types[i], INT, sizeof(Type));
        else if((*tableR).types[i] == FLT_REP)
            strncpy((*tableT).types[i], FLT, sizeof(Type));
        else if((*tableR).types[i] == BIN_REP)
            strncpy((*tableT).types[i], BIN, sizeof(Type));
        else
            return 0;

        strncpy((*tableT).fields[i], (*tableR).fields[i], sizeof(Field));
    }

    return 1;
}

void preline() {
    printf("SGDB>");
}
