#include "tools.h"

// Função auxilidar para auto-compleate dos comandos (uso interno da linenoise)
// prefix: string a ser completada
// lc: vetor de possibilidades
void completionHook (char const* prefix, linenoiseCompletions* lc) {
    int i = 0;

    while (i < NUM_COMMANDS) {
        char *prefixU = strdup(prefix);
        toUpperCase(prefixU);
        if (!strncmp(prefixU, commands[i], strlen(prefix))) {
            linenoiseAddCompletion(lc, commands[i]);
        }
        i++;
        free(prefixU);
    }
}

// Verifica um tipo e o converte para representação interna
// type: tipo escolhido pelo usuário
// return: char respectivo ao tipo
char validateType(char *type) {
    if (!strncmp(type, INT, TYPE_LIMIT)) {
        return 'i';
    } else if (!strncmp(type, STR, TYPE_LIMIT)) {
        return 's';
    } else if (!strncmp(type, FLT,TYPE_LIMIT)) {
        return 'f';
    } else if (!strncmp(type, BIN, TYPE_LIMIT)) {
        return 'b';
    } else {
        return '\0';
    }
}

// int init() {
//     // Index de tabelas
//     fclose(fopen(TABLES_INDEX, "ab"));

//     // Arquivo de valores binários
//     fclose(fopen(BINARY_TABLE, "ab"));

//     // Cria o diretório dos arquivos de tabelas
//     mkdir(TABLES_DIR, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

//     // Cria as strings para os scanf's do parser, com relação ao tamanho dos paramêtros
//     sprintf(CMD_SCANF, "%%%ds %%[^\n]", CMD_MAX-1);
//     sprintf(TBL_NAME_SCANF, "%%%ds %%[^\n]", TABLE_NAME_MAX-1);
//     sprintf(PARAMETER_SCANF, "%%%ds %%[^\n]", PARAMETER_MAX-1);

//     return 1;
// }

// void *safe_malloc(size_t size) {
//     void *p = malloc(size);

//     if (p)
//         return p;

//     raiseError(MALLOC_ERROR);
// }

// FILE *safe_fopen(char *name, char *mode) {
//     FILE *p = fopen(name, mode);

//     if (p)
//         return p;

//     raiseError(FOPEN_ERROR);
// }

// char *safe_strcat(char *dest, char *src) {
//     char *p = strcat(dest, src);
    
//     if (p)
//         return p;

//     raiseError(STRCAT_ERROR);
// }

// // Lê a quantidade de tabelas
// // tables_index: Arquivo de index das tabelas
// // return: Quantidade de tabelas
// int read_qt_tables(FILE *tables_index) {
//     int qt_tables = 0;

//     // Vai para o começo do arquivo
//     fseek(tables_index, 0, SEEK_SET);

//     // Lê um int
//     if(!fread(&qt_tables, sizeof(int), 1, tables_index)) {
//         // No caso de erro ao ler, significa que o arquivo estava vazio, então zero é gravado
//         fwrite(&qt_tables, sizeof(int), 1, tables_index);
//     }

//     return qt_tables;
// }

// // Escreve a quantidade de tabelas
// // tables_index: Arquivo de index das tabelas
// // qt_tables: Quantidade de tabelas
// void write_qt_tables(FILE *tables_index, int qt_tables) {
//     fseek(tables_index, 0, SEEK_SET);
//     fwrite(&qt_tables, sizeof(int), 1, tables_index);
// }

// // Lê os nomes das tabelas
// // tables_index: Arquivo de index das tabelas
// // qt_tables: Quantidade de tabelas
// // return: Vetor de TableName
// TableName *read_tables_names(FILE *tables_index, int qt_tables) {
//     TableName *names = safe_malloc(qt_tables * sizeof(TableName));

//     // Pula o número de tabelas
//     fseek(tables_index, sizeof(int), SEEK_SET);

//     // Lê os nomes
//     fread(names, sizeof(TableName), qt_tables, tables_index);

//     return names;
// }

// // Escreve os nomes das tabelas
// // tables_index: Arquivo de index das tabelas
// // names: Lista dos nomes
// // qt_tables: Quantidade de tabelas
// void write_tables_names(FILE *tables_index, TableName *names, int qt_tables) {
//     // Pula o número de tabelas
//     fseek(tables_index, sizeof(int), SEEK_SET);
//     fwrite(names, sizeof(TableName), qt_tables, tables_index);
// }

// // Lê os metadados de uma tabela, se ela existir
// // tableName: Nome da tabela a ser lida
// // return: Ponteiro para TableWRep lida
// TableWRep *read_table_metadata(TableName tableName) {
//     char *path = glueString(3, TABLES_DIR, tableName, TABLE_EXTENSION);

//     FILE *table_file = fopen(path, "rb+");
//     if (!table_file) {
//         return NULL;
//     }

//     TableWRep *table = safe_malloc(sizeof(TableWRep));
//     fread(table, sizeof(TableWRep), 1, table_file);
    
//     fclose(table_file);

//     return table;
// }

// // Escreve os metadados de uma tabela no seu arquivo
// // tables_index: Arquivo de index das tabelas
// // table: tabela a ser gravada
// // index: Posição da tabela
// void write_table_metadata(FILE *tables_index, TableWRep *table, int index) {
//     char *path = glueString(3, TABLES_DIR, table->name, TABLE_EXTENSION);

//     fclose(safe_fopen(path, "ab"));
//     FILE *table_file = safe_fopen(path, "rb+");
//     // Escreve a tabela
//     fwrite(table, sizeof(TableWRep), 1, table_file);
//     // Escreve o número de rows
//     fwrite((int[]){0}, sizeof(int), 1, table_file);
//     fclose(table_file);

//     // Adiciona o nome no arquivo de index
//     fseek(tables_index, sizeof(int) + index * sizeof(TableName), SEEK_SET);
//     fwrite(table->name, sizeof(TableName), 1, tables_index);
// }

// void safe_remove(char *path) {
//     if (remove(path)) {
//         raiseError(CANT_REMOVE_FILE);
//     }
// }

// void toUpperCase(char *str) {
//     int i = -1;
//     while (str[++i] != '\0')
//         if (str[i] >= 'a' && str[i] <= 'z') str[i] = str[i] - 32;
// }

// // return: Flag indicando se algum espaço foi substituido
// int replaceSpace(char *str, char c) {
//     int flag = 0;

//     for(int i = 0; str[i]; i++) {
//         if (str[i] == ' ') {
//             str[i] = c;
//             flag = 1;
//         }
//     }

//     return flag;
// }

// // return: Flag indicando se algum char foi substituido
// int glueChars(char *str, char c) {
//     int flag = 0;

//     int i = 0;
//     for (; str[i]; i++);

//     char *temp = safe_malloc(i*sizeof(char));
//     int ti = 0;

//     for (i = 0; str[i]; i++) {
//         if (str[i] != c) {
//             temp[ti++] = str[i];
//         } else {
//             flag = 1;
//         }
//     }

//     for (i = 0; i < ti; i++) {
//         str[i] = temp[i];
//     }

//     str[i] = '\0';

//     return flag;
// }

// // Verifica a existência de uma tabela com o nome especificado
// // name: Nome da tabela
// // return: 1 se a tabela existe
// int tableNameExists(TableName *names, char *name, int qt_tables) {
//     int i = 0;

//     while (i < qt_tables) {
//         if(!strcmp(names[i], name)) {
//             return 1;
//         }

//         i++;
//     }

//     return 0;
// }

// // Converte uma TableWType para uma TableWRep
// // tableR: Ponteiro para tabela resultado
// // tableT: Ponteiro para tabela de entrada
// // return: 1 em sucesso
// int convertToRep(TableWRep *tableR, TableWType *tableT) {
//     // Quantidade de colunas da tabela de entrada
//     int t_cols = tableT->cols;

//     // Copia o nome
//     strncpy(tableR->name, tableT->name, sizeof(TableName));
//     // Atribui a quantidade de colunas
//     tableR->cols = t_cols;
//     // Define o tamanho estático da row
//     tableR->row_bytes_size = 0;

//     // Faz a converçãos dos tipos
//     for (int i = 0; i < t_cols; i++) {
//         if(strcmp((*tableT).types[i], STR) == 0) {
//             (*tableR).row_bytes_size += STR_SIZE;
//             (*tableR).types[i] = STR_REP;
//         } else if(strcmp((*tableT).types[i], INT) == 0) {
//             (*tableR).row_bytes_size += sizeof(int);
//             (*tableR).types[i] = INT_REP;
//         } else if(strcmp((*tableT).types[i], FLT) == 0) {
//             (*tableR).row_bytes_size += sizeof(float);
//             (*tableR).types[i] = FLT_REP;
//         } else if(strcmp((*tableT).types[i], BIN) == 0) {
//             (*tableR).row_bytes_size += BIN_SIZE;
//             (*tableR).types[i] = BIN_REP;
//         } else {
//             return 0;
//         }

//         strncpy((*tableR).fields[i], (*tableT).fields[i], sizeof(Field));
//     }

//     return 1;
// }

// // Converte uma TableWRep para uma TableWType
// // return: 1 em sucesso
// int convertToType(TableWType *tableT, TableWRep *tableR) {
//     strncpy((*tableT).name, (*tableR).name, sizeof(TableName));
//     (*tableT).cols = (*tableR).cols;
    
//     int s = (*tableR).cols;

//     for (int i = 0; i < s; i++) {
//         if((*tableR).types[i] == STR_REP)
//             strncpy((*tableT).types[i], STR, sizeof(Type));
//         else if((*tableR).types[i] == INT_REP)
//             strncpy((*tableT).types[i], INT, sizeof(Type));
//         else if((*tableR).types[i] == FLT_REP)
//             strncpy((*tableT).types[i], FLT, sizeof(Type));
//         else if((*tableR).types[i] == BIN_REP)
//             strncpy((*tableT).types[i], BIN, sizeof(Type));
//         else
//             return 0;

//         strncpy((*tableT).fields[i], (*tableR).fields[i], sizeof(Field));
//     }

//     return 1;
// }

// // Junta várias strings em uma
// // return: Nova string
// char *glueString(int n_args, ...) {
//     char **args = safe_malloc(n_args * sizeof(char*));
//     int size = 0;

//     va_list ap;
//     va_start(ap, n_args);

//     for (int i = 0; i < n_args; i++) {
//         args[i] = va_arg(ap, char *);
//         for (int j = 0; args[i][j]; j++) {
//             size++;
//         }
//     }
    
//     va_end(ap);
    
//     size++;
//     char *r = safe_malloc(size * sizeof(char));

//     int k = 0;
//     for (int i = 0; i < n_args; i++) {
//         for (int j = 0; args[i][j]; j++) {
//             r[k++] = args[i][j];
//         }
//     }
//     r[k] = '\0';

//     free(args);
//     return r;
// }

// void preline() {
//     printf("SGDB>");
//     fflush(stdout);
// }

// // Compara duas strings, 'a' e 'b'
// // retorno: 1 se 'a' é 'maior'
// int strOrder(char *a, char *b) {
//     int i = 0;
//     while (a[i] && b[i] && a[i] == b[i]) {
//         i++;
//     }

//     return a[i] > b[i];
// }

// // Calcula o offset de uma campo dentro dos dados de uma row e o tamanho do campo
// // meta: ponteiro para os metadados de uma tabela
// // field: nome do campo
// // return: vetor com o offset, o tamanho e o tipo do campo em bytes
// int *getOffset(TableWRep *meta, Field field) {
//     int *info = safe_malloc(3 * sizeof(int));
//     int offset = 0;
//     int i = 0;

//     while (i < meta->cols && strcmp(meta->fields[i], field)) {
//         if (meta->types[i] == STR_REP) {
//             offset += STR_SIZE;
//         } else if (meta->types[i] == INT_REP) {
//             offset += sizeof(int);
//         } else if (meta->types[i] == FLT_REP) {
//             offset += sizeof(float);
//         } else if (meta->types[i] == BIN_REP) {
//             offset += BIN_SIZE;
//         }

//         i++;
//     }

//     // Campo não encontrado
//     if (i == meta->cols) {
//         info[0] = -1;
//         return info;
//     }

//     // Offset e tamanho
//     info[0] = offset;
//     if (meta->types[i] == STR_REP) {
//         info[1] = STR_SIZE;
//     } else if (meta->types[i] == INT_REP) {
//         info[1] = sizeof(int);
//     } else if (meta->types[i] == FLT_REP) {
//         info[1] = sizeof(float);
//     } else if (meta->types[i] == BIN_REP) {
//         info[1] = BIN_SIZE;
//     }
//     info[2] = meta->types[i];

//     return info;
// }
