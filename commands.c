#include "commands.h"

Error EXEC_ERROR_CODE = NONE; // Erro global de execução de comandos

FILE *tables_index = NULL; // Variável global utilizada para manipular arquivos

int qt_tables = 0;      // Quantidade de tabelas

// Cria tabela
// table: Struct com as informações para crira uma tabela
void createTable(TableWType table) {
    printf("Criando tabela.\n\n");

    tables_index = fopen(TABLES_INDEX, "rb+");

    if (!tables_index) {
        EXEC_ERROR_CODE = IN_ERROR; return;
    }

    if(!fread(&qt_tables, sizeof(int), 1, tables_index)) {
        qt_tables = 0;
        fwrite(&qt_tables, sizeof(int), 1, tables_index);
    }

    if (qt_tables) {
        TableName *names = malloc(qt_tables * sizeof(TableName));
        if (!names) {
            EXEC_ERROR_CODE = IN_ERROR; return;
        }

        fseek(tables_index, sizeof(int), SEEK_SET);
        for (int i = 0; i < qt_tables; i++) {
            fread(&names[i], sizeof(TableName), 1, tables_index);
            fseek(tables_index, sizeof(TableWRep) - sizeof(TableName), SEEK_CUR);
        }

        if (tableNameExists(names, table.name)) {
            EXEC_ERROR_CODE = CT_TBL_EXT; return;
        }

        safeFree(names);
    }

    TableWRep tableData;

    // Converte a tabela
    if(!convertToRep(&tableData, &table)) {
        EXEC_ERROR_CODE = CT_WRG_TYPE; return;
    }

    // Salva o novo número de tabelas
    qt_tables++;
    fseek(tables_index, 0, SEEK_SET);
    fwrite(&qt_tables, sizeof(int), 1, tables_index);

    // Escreve a tabela no arquivo
    fseek(tables_index, 0, SEEK_END);
    fwrite(&tableData, sizeof(TableWRep), 1, tables_index);

    fclose(tables_index);

    EXEC_ERROR_CODE = CT_SUCCESS;
}

// Remove tabela
// table_name: Nome da tabela
void removeTable(TableName table_name) {
    printf("Removendo a tabela %s.\n", table_name);
    EXEC_ERROR_CODE = TODO;
}

// Apresenta tabela tabela
// table_name: Nome da tabela
void apTable(TableName table_name) {
    tables_index = fopen(TABLES_INDEX, "rb+");

    if (!tables_index) {
        EXEC_ERROR_CODE = IN_ERROR; return;
    }

    if(!fread(&qt_tables, sizeof(int), 1, tables_index)) {
        qt_tables = 0;
        fwrite(&qt_tables, sizeof(int), 1, tables_index);
    }

    if (qt_tables) {
        TableWRep *tableData = malloc(sizeof(TableWRep));
        if (!tableData) {
            EXEC_ERROR_CODE = IN_ERROR; return;
        }

        TableWType *table = malloc(sizeof(TableWType));
        if (!table) {
            EXEC_ERROR_CODE = IN_ERROR; return;
        }

        fseek(tables_index, sizeof(int), SEEK_SET);

        int i = 0;
        while (i < qt_tables) {
            fread(tableData->name, sizeof(TableName), 1, tables_index);

            if (!strcmp(tableData->name, table_name)) {
                fseek(tables_index, -1*sizeof(TableName), SEEK_CUR);
                fread(tableData, sizeof(TableWRep), 1, tables_index);
                break;
            } else {
                fseek(tables_index, sizeof(TableWRep) - sizeof(TableName), SEEK_CUR);
            }

            i++;
        }

        if (i == qt_tables) {
            EXEC_ERROR_CODE = AT_NOT_FOUND; return;
        }

        printf("Mostrando a tabela %s.\n\n", table_name);

        convertToType(table, tableData);

        printf("%s\n", table->name);

        for (int j = 0; j < table->cols; j++) {
            printf("- %s:%s\n", table->types[j], table->fields[j]);
        }

        safeFree(tableData);
        safeFree(table);
    } else {
        EXEC_ERROR_CODE = AT_NOT_FOUND; return;
    }

    fclose(tables_index);

    EXEC_ERROR_CODE = AT_SUCCESS;
}

// Lista tabelas
void listTables() {
    tables_index = fopen(TABLES_INDEX, "rb+");

    if (!tables_index) {
        EXEC_ERROR_CODE = IN_ERROR; return;
    }

    if(!fread(&qt_tables, sizeof(int), 1, tables_index)) {
        qt_tables = 0;
        fwrite(&qt_tables, sizeof(int), 1, tables_index);
    }

    printf("Listando %d tabelas.\n\n", qt_tables);

    if (qt_tables) {
        TableName *tablesName = malloc(qt_tables * sizeof(TableName));
        if (!tablesName) {
            EXEC_ERROR_CODE = IN_ERROR; return;
        }

        fseek(tables_index, sizeof(int), SEEK_SET);
        for (int i = 0; i < qt_tables; i++) {
            fread(&tablesName[i], sizeof(TableName), 1, tables_index);
            fseek(tables_index, sizeof(TableWRep) - sizeof(TableName), SEEK_CUR);
        }

        for (int i = 0; i < qt_tables; i++) {
            printf("%s\n", tablesName[i]);
        }

        safeFree(tablesName);
    }

    fclose(tables_index);

    EXEC_ERROR_CODE = LT_SUCCESS;
}

// Inclui registro na tabela
// row: Struct com os valores de um registro
void includeReg(Row row) {
    printf("Novo registro na tabela %s.\n", row.table_name);

    for (int i = 0; i < row.size; i++) {
        printf("%s\n", row.values[i]);
    }

    EXEC_ERROR_CODE = TODO;
}

// Busca registros na tabela, único
// table_name: Nome da tabela
// field_name: Nome do campo(chave)
// value: Nome do valor
void busRegU(TableName table_name, Field field_name, Value value) {
    printf("Buscando %s igual à %s em %s.\n", table_name, field_name, value);

    EXEC_ERROR_CODE = TODO;
}

// Busca registros na tabela, todos
// table_name: Nome da tabela
// field_name: Nome do campo(chave)
// value: Nome do valor
void busRegN(TableName table_name, Field field_name, Value value) {
    printf("Buscando todos os %s igual à %s em %s.\n", table_name, field_name, value);

    EXEC_ERROR_CODE = TODO;
}

// Apresenta registros pesquisados da tabela
// table_name: Nome da tabela
void apReg(TableName table_name) {
    printf("Mostrando a pesquisa de %s.\n", table_name);

    EXEC_ERROR_CODE = TODO;
}

// Remove registro da tabela
// table_name: Nome da tabela
void removeReg(TableName table_name) {
    printf("Registros removidos de %s.\n", table_name);
    
    EXEC_ERROR_CODE = TODO;
}

// Cria index da tabela, árvore
// table_name: Nome da tabela
// field_name: Nome da campo(chave) a ser usado
void createIndexA(TableName table_name, Field field_name) {
    printf("Criado um índice com árvore para %s usando a coluna %s.\n", table_name, field_name);

    EXEC_ERROR_CODE = TODO;
}

// Cria index da tabela, heap
// table_name: Nome da tabela
// field_name: Nome da campo(chave) a ser usado
void createIndexH(TableName table_name, Field field_name) {
    printf("Criado um índice com heap para %s usando a coluna %s.\n", table_name, field_name);

    EXEC_ERROR_CODE = TODO;
}

// Remove index da tabela
// table_name: Nome da tabela
// field_name: Nome da campo(chave) a ser usado
void removeIndex(TableName table_name, Field field_name) {
    printf("Removendo o índice de %s referente a chave %s.\n", table_name, field_name);

    EXEC_ERROR_CODE = TODO;
}

// Gera index
// table_name: Nome da tabela
// field_name: Nome da campo(chave) a ser usado
void genIndex(TableName table_name, Field field_name) {
    printf("Gerando um índice com %s para %s usando a coluna %s.\n", "-recuperar-", table_name, field_name);

    EXEC_ERROR_CODE = TODO;
}