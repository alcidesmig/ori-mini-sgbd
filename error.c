#include "error.h"

void raiseError(Error error) {
	switch(error) {
		case NONE:
		case IN_ERROR:
			printf("Erro insterno.\n");
			exit(0);
		case UNSUPORTED_PARAM:
			exit(0);
		case NO_COMMAND:
			printf("Comando não reconhecido.\n");
			exit(0);
		case TODO:
			printf("TODO.\n");
			return;
		case EXIT:
			printf("Saindo.\n");
			exit(0);
		case MALLOC_ERROR:
			printf("MALLOC_ERROR.\n");
			exit(0);
		case FOPEN_ERROR:
			printf("FOPEN_ERROR.\n");
			exit(0);
		case STRCAT_ERROR:
			printf("STRCAT_ERROR.\n");
			exit(0);
		case CANT_REMOVE_FILE:
			printf("STRCAT_ERROR.\n");
			exit(0);
		case CT_WRONG_SINTAX:
			printf("Sintax errada.\n");
			exit(0);
		case CT_WRONG_TYPE:
			printf("Tipo não suportado.\n");
			exit(0);
		case CT_TABLE_EXISTS:
			printf("Nome já existe.\n");
			exit(0);
		case RT_WRONG_SINTAX:
			printf("Sintax errada.\n");
			exit(0);
		case RT_CANT_FIND_TABLE:
			printf("Tabela não existe.\n");
			exit(0);
		case AT_WRONG_SINTAX:
			printf("Sintax errada.\n");
			exit(0);
		case AT_CANT_FIND_TABLE:
			printf("Tabela não existe.\n");
			exit(0);
		case IR_WRONG_SINTAX:
			printf("Sintax errada.\n");
			exit(0);
		case BR_WRONG_SINTAX:
			printf("Sintax errada.\n");
			exit(0);
		case BR_WRONG_PARAMETER:
			printf("Parâmetro errado.\n");
			exit(0);
		case AR_WRONG_SINTAX:
			printf("Sintax errada.\n");
			exit(0);
		case RR_WRONG_SINTAX:
			printf("Sintax errada.\n");
			exit(0);
		case CI_WRONG_SINTAX:
			printf("Sintax errada.\n");
			exit(0);
		case CI_WRONG_PARAMETER:
			printf("Parâmetro errado.\n");
			exit(0);
		case RI_WRONG_SINTAX:
			printf("Sintax errada.\n");
			exit(0);
		case GI_WRONG_SINTAX:
			printf("Sintax errada.\n");
			exit(0);
		default:
			printf("%x\n", error);
			exit(0);
	}
}