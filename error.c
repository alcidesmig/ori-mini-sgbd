#include "error.h"
#include "getline_custom.h"

void raiseError(Error error) {
	switch(error) {
		case NONE:
			break;
		case IN_ERROR:
			printf("Erro interno.\n");
			exit_term_close(0);
		case UNSUPORTED_PARAM:
			exit_term_close(0);
		case NO_COMMAND:
			printf("Comando não reconhecido.\n");
			return;
		case MALLOC_ERROR:
			printf("MALLOC_ERROR.\n");
			exit_term_close(0);
		case FOPEN_ERROR:
			printf("FOPEN_ERROR.\n");
			exit_term_close(0);
		case STRCAT_ERROR:
			printf("STRCAT_ERROR.\n");
			exit_term_close(0);
		case CANT_REMOVE_FILE:
			printf("O arquivo não pode ser removido.\n");
			exit_term_close(0);
		case TABLE_EXISTS:
			printf("Uma tabela com esse nome já existe.\n");
			exit_term_close(0);
		case UNSUPORTED_TYPE:
			printf("Tipo de dado não suportado.\n");
			exit_term_close(0);
		case CANT_FIND_TABLE:
			printf("Uma tabela com esse nome não existe.\n");
			exit_term_close(0);
		case DIFF_PARAM_NUMB:
			printf("A quantidade de parâmetros não bate.\n");
			exit_term_close(0);
		case WRONG_SINTAX:
			printf("Sintax do comando está errada.\n");
			exit_term_close(0);
		case WRONG_VALUE:
			printf("O tipo não valor não corresponde ao campo.\n");
			exit_term_close(0);
		case WRONG_PARAMETER:
			printf("Parâmero não foi reconhecido.\n");
			exit_term_close(0);
		case FIELD_NOT_FOUND:
			printf("Um campo com esse nome não existe.\n");
			exit_term_close(0);
		case NOT_INT:
			printf("O valor não é um int.\n");
			exit_term_close(0);
		case NOT_FLOAT:
			printf("O valor não é um float.\n");
			exit_term_close(0);
		case NO_SEARCH_TABLE:
			printf("Não há pesquisa para tabela.\n");
			exit_term_close(0);
		case TODO:
			printf("TODO.\n");
			break;
		case EXIT:
			printf("Saindo.\n");
			exit_term_close(0);
		default:
			printf("%x\n", error);
			exit_term_close(0);
	}
}