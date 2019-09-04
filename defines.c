#include "defines.h"

// Tipos
const char STR[4] = "STR";
const char BIN[4] = "BIN";
const char INT[4] = "INT";
const char FLT[4] = "FLT";

const char STR_REP = 'S';
const char BIN_REP = 'B';
const char INT_REP = 'I';
const char FLT_REP = 'F';

// Scanf Strings

char CMD_SCANF[16];
char TBL_NAME_SCANF[16];
char FIELD_NAME_SCANF[16] = "%[^\n]";
char PARAMETER_SCANF[16];
char FIELD_NAME_VALUE_SCANF[16] = "%[^:]%*c%[^\n]";
char TYPE_FIELD_SCANF[32] = "%[^:^;]%*c%[^;^:]%*c%[^\n]";
char VALUE_SCANF[16] = "%[^;]%*c%[^\n]";

// Tamanho de um campo binário
const long int BIN_SIZE = sizeof (long int);