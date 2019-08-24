#include "commands.h"

FILE *tables_index = NULL; // Variável global utilizada para manipular arquivos
int qt_tables = 0;      // Quantidade de tabelas

// Cria tabela
// table: Struct com as informações para crira uma tabela
void createTable(TableWType *table) {
    tables_index = safe_fopen(TABLES_INDEX, "rb+");
    qt_tables = read_qt_tables(tables_index);

    if (qt_tables) {
        TableName *names = read_tables_names(tables_index, qt_tables);

        if (tableNameExists(names, table->name, qt_tables)) {
            raiseError(CT_TABLE_EXISTS);
        }

        free(names);
    }

    TableWRep tableData;

    // Converte a tabela
    if(!convertToRep(&tableData, table)) {
        raiseError(CT_WRONG_TYPE);
    }

    // Salva o novo número de tabelas
    increase_qt_tables(tables_index, qt_tables);

    // Escreve a tabela no arquivo
    write_table_metadata(tables_index, &tableData);

    fclose(tables_index);

    printf("Criando tabela %s.\n\n", table->name);
}

// Remove tabela
// table_name: Nome da tabela
void removeTable(TableName table_name) {
    printf("Removendo a tabela %s.\n", table_name);
    raiseError(TODO);
}

// Apresenta tabela tabela
// table_name: Nome da tabela
void apTable(TableName table_name) {
    TableWRep *tableData = read_table_metadata(table_name);

    if (!tableData) {
        raiseError(AT_CANT_FIND_TABLE);
    }

    TableWType *table = safe_malloc(sizeof(TableWType));

    convertToType(table, tableData);

    printf("Mostrando a tabela %s.\n\n", table_name);

    printf("%s\n", table->name);

    for (int j = 0; j < table->cols; j++) {
        printf("- %s:%s\n", table->types[j], table->fields[j]);
    }

    free(tableData);
    free(table);
}

// Lista tabelas
void listTables() {
    tables_index = safe_fopen(TABLES_INDEX, "rb+");

    qt_tables = read_qt_tables(tables_index);

    if (!qt_tables) {
        printf("Nenhuma tabela no sistema.\n");
        fclose(tables_index);
        return;
    }

    printf("Listando %d tabelas.\n", qt_tables);

    if (qt_tables) {
        TableName *names = read_tables_names(tables_index, qt_tables);

        for (int i = 0; i < qt_tables; i++) {
            printf("- %s\n", names[i]);
        }

        free(names);
    }

    fclose(tables_index);

    printf("\n");
}

// Inclui registro na tabela
// row: Struct com os valores de um registro
void includeReg(Row *row) {
    printf("Novo registro na tabela %s.\n", row->table_name);

    for (int i = 0; i < row->size; i++) {
        printf("%s\n", row->values[i]);
    }

    raiseError(TODO);
}

// Busca registros na tabela, único
// table_name: Nome da tabela
// field_name: Nome do campo(chave)
// value: Nome do valor
void busRegU(TableName table_name, Field field_name, Value value) {
    printf("Buscando %s igual à %s em %s.\n", field_name, value, table_name);

    raiseError(TODO);
}

// Busca registros na tabela, todos
// table_name: Nome da tabela
// field_name: Nome do campo(chave)
// value: Nome do valor
void busRegN(TableName table_name, Field field_name, Value value) {
    printf("Buscando todos os %s igual à %s em %s.\n", table_name, field_name, value);

    raiseError(TODO);
}

// Apresenta registros pesquisados da tabela
// table_name: Nome da tabela
void apReg(TableName table_name) {
    printf("Mostrando a pesquisa de %s.\n", table_name);

    raiseError(TODO);
}

// Remove registro da tabela
// table_name: Nome da tabela
void removeReg(TableName table_name) {
    printf("Registros removidos de %s.\n", table_name);
    
    raiseError(TODO);
}

// Cria index da tabela, árvore
// table_name: Nome da tabela
// field_name: Nome da campo(chave) a ser usado
void createIndexA(TableName table_name, Field field_name) {
    printf("Criado um índice com árvore para %s usando a coluna %s.\n", table_name, field_name);

    raiseError(TODO);
}

// Cria index da tabela, heap
// table_name: Nome da tabela
// field_name: Nome da campo(chave) a ser usado
void createIndexH(TableName table_name, Field field_name) {
    printf("Criado um índice com heap para %s usando a coluna %s.\n", table_name, field_name);

    raiseError(TODO);
}

// Remove index da tabela
// table_name: Nome da tabela
// field_name: Nome da campo(chave) a ser usado
void removeIndex(TableName table_name, Field field_name) {
    printf("Removendo o índice de %s referente a chave %s.\n", table_name, field_name);

    raiseError(TODO);
}

// Gera index
// table_name: Nome da tabela
// field_name: Nome da campo(chave) a ser usado
void genIndex(TableName table_name, Field field_name) {
    printf("Gerando um índice com %s para %s usando a coluna %s.\n", "-recuperar-", table_name, field_name);

    raiseError(TODO);
}