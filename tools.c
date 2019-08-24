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

// Lê os nomes das tabelas
// tables_index: Arquivo de index das tabelas
// qt_tables: Quantidade de tabelas
TableName *read_tables_names(FILE *tables_index, int qt_tables) {
    TableName *names = safe_malloc(qt_tables * sizeof(TableName));

    fseek(tables_index, sizeof(int), SEEK_SET);
    fread(names, sizeof(TableName), qt_tables, tables_index);

    return names;
}

// Aumenta a quantidade de tabelas em 1
// tables_index: Arquivo de index das tabelas
// qt_tables: Quantidade de tabelas
void increase_qt_tables(FILE *tables_index, int qt_tables) {
    qt_tables++;
    fseek(tables_index, 0, SEEK_SET);
    fwrite(&qt_tables, sizeof(int), 1, tables_index);
}

// Diminui a quantidade de tabelas em 1
// tables_index: Arquivo de index das tabelas
// qt_tables: Quantidade de tabelas
void reduce_qt_tables(FILE *tables_index, int qt_tables) {
    qt_tables--;
    fseek(tables_index, 0, SEEK_SET);
    fwrite(&qt_tables, sizeof(int), 1, tables_index);
}

// Escreve os metadados de uma tabela no seu arquivo
// tables_index: Arquivo de index das tabelas
// table: tabela a ser gravada
void write_table_metadata(FILE *tables_index, TableWRep *table) {
    TablePath path = "";

    safe_strcat(path, TABLES_DIR);
    safe_strcat(path, table->name);
    safe_strcat(path, TABLE_EXTENSION);

    fclose(safe_fopen(path, "ab"));
    FILE *table_file = safe_fopen(path, "rb+");
    fwrite(table, sizeof(TableWRep), 1, table_file);
    fclose(table_file);

    fseek(tables_index, 0, SEEK_END);
    fwrite(table->name, sizeof(TableName), 1, tables_index);
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

void toUpperCase(char *str) {
    int i = -1;
    while (str[++i] != '\0')
        if (str[i] >= 'a' && str[i] <= 'z') str[i] = str[i] - 32;
}

// Conta a quantidade de espaços em uma string
int countSpaces(char *str) {
    int cont = 0;
    for (int i = 0; i < strlen(str); i++) {
        if(*(str + i) == ' ') cont++;
    }
    return cont;
}

// Retira espaços indesejados no meio dos tipos e substitui os espaços dos campos por underline -> Para CT
int fixingCommandCT(char *command) {
    if(countSpaces(command) < 2) return 0;
    char * beginStruct = strstr((strstr(command, " ") + 1), " ") + 1;
    void bringBack(char * str, int qt) {
        for(int i = 0; i < strlen(str); i++) {
            str[i] = str[i+qt];
        }
    }
    int inType = 0;
    int cont_ = 0, contSpace = 0;
    for(int i = 0; beginStruct[i] != '\0'; i++){
        if(beginStruct[i] == ':') inType = 1;
        else if(beginStruct[i] == ';') inType = 0;
        if(beginStruct[i] == ' '){
            if(!inType) {
                bringBack(&beginStruct[i], 1);
            } else {
                cont_++;
                beginStruct[i] = '_';
            }
        }
    }
    if(cont_) {
        printf("%d espaços nos nomes dos campos foram substituídos por \"_\" para melhor representação dos dados.\n", cont_);
    }
    if(contSpace) {
        printf("%d espaços nos tipos dos campos foram removidos para obedecer a tipação disponível.\n", contSpace);
    }
    return 1;
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
    strcpy((*tableR).name, (*tableT).name);
    (*tableR).cols = (*tableT).cols;
    (*tableR).row_bytes_size = 0;

    int s = (*tableT).cols;

    for (int i = 0; i < s; i++) {
        if(strcmp((*tableT).types[i], STR) == 0) {
            (*tableR).row_bytes_size += STR_SIZE;
            (*tableR).types[i] = STR_REP;
        } else if(strcmp((*tableT).types[i], INT) == 0) {
            (*tableR).row_bytes_size += INT_SIZE;
            (*tableR).types[i] = INT_REP;
        } else if(strcmp((*tableT).types[i], FLT) == 0) {
            (*tableR).row_bytes_size += FLT_SIZE;
            (*tableR).types[i] = FLT_REP;
        } else if(strcmp((*tableT).types[i], BIN) == 0) {
            (*tableR).row_bytes_size += BIN_SIZE;
            (*tableR).types[i] = BIN_REP;
        } else {
            return 0;
        }

        strcpy((*tableR).fields[i], (*tableT).fields[i]);
    }

    return 1;
}

// Converte uma TableWRep para uma TableWType
int convertToType(TableWType *tableT, TableWRep *tableR) {
    strcpy((*tableT).name, (*tableR).name);
    (*tableT).cols = (*tableR).cols;
    
    int s = (*tableR).cols;

    for (int i = 0; i < s; i++) {
        if((*tableR).types[i] == STR_REP)
            strcpy((*tableT).types[i], STR);
        else if((*tableR).types[i] == INT_REP)
            strcpy((*tableT).types[i], INT);
        else if((*tableR).types[i] == FLT_REP)
            strcpy((*tableT).types[i], FLT);
        else if((*tableR).types[i] == BIN_REP)
            strcpy((*tableT).types[i], BIN);
        else
            return 0;

        strcpy((*tableT).fields[i], (*tableR).fields[i]);
    }

    return 1;
}

void preline() {
    printf("SGDB>");
}
