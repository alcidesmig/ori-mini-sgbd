#include "commands.h"

// Erro global de execução de comandos
int EXEC_ERROR_CODE = NONE;
// Variável global utilizada para manipular arquivos
FILE * fp;

// Cria tabela
// table_name: Nome da tabela
// type_name_arr: Vetor de tipos
// field_name_arr: Vetor de campos(chave)
// size_arr: Tamanho dos vetores
void createTable(char *table_name, TypeArr type_name_arr, FieldArr field_name_arr, int size_arr) {
    printf("Criando tabela %s.\n", table_name);

    Table data;
    data.qt_fields = size_arr;
    strcpy(data.table_name, table_name);
    
    for (int i = 0; i < size_arr; i++) {
        for (int j = 0; j < TYPE_MAX && type_name_arr[i][j] != '\0'; j++) {
            printf("%c", type_name_arr[i][j]);
        }
        printf("aqui: <%s> %d", type_name_arr[i], type_name_arr[i] == "INT");
        if(strcmp(type_name_arr[i], STR) == 0) {
            data.types[i] = 'S';
        } else if(strcmp(type_name_arr[i], INT) == 0) {
            data.types[i] = 'I';
        } else if(strcmp(type_name_arr[i], FLT) == 0) {
            data.types[i] = 'F';
        } else if(strcmp(type_name_arr[i], BIN) == 0) {
            data.types[i] = 'B';
        }
        
        printf(" ");
        for (int j = 0; j < FIELD_MAX && field_name_arr[i][j] != '\0'; j++) {
            printf("%c", field_name_arr[i][j]);
        }
        strcpy(data.fields[i], field_name_arr[i]);
        strcat(data.fields[i], "\0");
        printf("\n");
    }

    
    // Abre arquivo com os metadados das tabelas
    fp = fopen("tables.bin", "ab+");

    // Coloca o ponteiro no início e tenta ler o número que representa a quantidade de tabelas, para acrescentar 1
    fseek(fp, 0, SEEK_SET);
    int qt_tables = 0;
    if (fread(&qt_tables, sizeof(int), 1, fp) != 0){
        fseek(fp, 0, SEEK_SET);
        qt_tables++;
        fwrite(&qt_tables, sizeof(int), 1, fp);
    } else {
        fseek(fp, 0, SEEK_SET);
        qt_tables = 1;
        fwrite(&qt_tables, sizeof(int), 1, fp); 
    }
    fseek(fp, 0, SEEK_END);

    // Grava os metadados da nova tabela
    fwrite(&data, sizeof(Table), 1, fp);
    fclose(fp);

    printf("Tabela criada!\n");
}
// Remove tabela
// table_name: Nome da tabela
void removeTable(char *table_name) {
    printf("Removendo a tabela %s.\n", table_name);
    printf("TODO\n");
}

// Apresenta tabela tabela
// table_name: Nome da tabela
void apTable(char *table_name) {
    printf("Mostrando a tabela %s.\n", table_name);
    printf("TODO\n");
}

// Lista tabela
void listTables() {
    printf("Listando tabelas.\n");

    Table data;
    int qt_tables;
    // Abre arquivo que guarda os metadados das tabelas
    fp = fopen("tables.bin", "rb");
    // Lê a quantidade de tabelas existentes
    fread(&qt_tables, sizeof(int), 1, fp);
    printf("Quantidade de tabelas: %d\n", qt_tables);
    // Lê os metadados de todas as tabelas
    fread(&data, sizeof(Table), qt_tables, fp);
    fclose(fp);
    // Imprime para o usuário
    for (int i = 0; i < qt_tables; i++) {
        printf("%s ", data.table_name);
        for(int j = 0; j < data.qt_fields; j++) {
            //printf("%s", data.types);
            printf("%s:%s ", data.types[i] == 'S' ? 
                                            STR : data.types[i] == 'F' ? 
                                            FLT : data.types[i] == 'B' ? 
                                            BIN : data.types[i] == 'I' ? 
                                            INT : "?", data.fields[i]);
        }
        printf("\n");
    }

}

// Inclui registro na tabela
// table_name: Nome da tabela
// value_arr: Vetor de valores
// size_arr: Tamanho do vetores
void includeReg(char *table_name, ValueArr value_arr, int size_arr) {
    printf("Novo registro na tabela %s.\n", table_name);

    for (int i = 0; i < size_arr; i++) {
        printf("%s\n", value_arr[i]);
    }

    printf("TODO\n");
}

// Busca registros na tabela, único
// table_name: Nome da tabela
// field_name: Nome do campo(chave)
// value: Nome do valor
void busRegU(char *table_name, char *field_name, char *value) {
    printf("Buscando %s igual à %s em %s.\n", table_name, field_name, value);
    printf("TODO\n");
}

// Busca registros na tabela, todos
// table_name: Nome da tabela
// field_name: Nome do campo(chave)
// value: Nome do valor
void busRegN(char *table_name, char *field_name, char *value) {
    printf("Buscando todos os %s igual à %s em %s.\n", table_name, field_name, value);
    printf("TODO\n");
}

// Apresenta registros pesquisados da tabela
// table_name: Nome da tabela
void apReg(char *table_name) {
    printf("Mostrando a pesquisa de %s.\n", table_name);
    printf("TODO\n");
}

// Remove registro da tabela
// table_name: Nome da tabela
void removeReg(char *table_name) {
    printf("Registros removidos de %s.\n", table_name);
    printf("TODO\n");
}

// Cria index da tabela, árvore
// table_name: Nome da tabela
// field_name: Nome da campo(chave) a ser usado
void createIndexA(char *table_name, char *field_name) {
    printf("Criado um índice com árvore para %s usando a coluna %s.\n", table_name, field_name);
    printf("TODO\n");
}

// Cria index da tabela, heap
// table_name: Nome da tabela
// field_name: Nome da campo(chave) a ser usado
void createIndexH(char *table_name, char *field_name) {
    printf("Criado um índice com heap para %s usando a coluna %s.\n", table_name, field_name);
    printf("TODO\n");
}

// Remove index da tabela
// table_name: Nome da tabela
// field_name: Nome da campo(chave) a ser usado
void removeIndex(char *table_name, char *field_name) {
    printf("Removendo o índice de %s referente a chave %s.\n", table_name, field_name);
    printf("TODO\n");
}

// Gera index
// table_name: Nome da tabela
// field_name: Nome da campo(chave) a ser usado
void genIndex(char *table_name, char *field_name) {
    printf("Gerando um índice com %s para %s usando a coluna %s.\n", "-recuperar-", table_name, field_name);
    printf("TODO\n");
}