#ifndef SEARCH_STRUCTS_H
#define SEARCH_STRUCTS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "defines.h"
#include "utils.h"

typedef struct ResultList {
	struct ResultList *next;
	long int pos;
	void * key;
} ResultList;

typedef struct ResultNode {
	struct ResultNode *left;
	struct ResultNode *right;
	ResultList *result;
	TableName tableName;
} ResultNode;

void addToResultList(ResultList **list, long int pos, void * key);
void freeResultList(ResultList *list);
void freeResultTree(ResultNode *root);
void addToResultTree (ResultNode **root, ResultList *list, TableName tableName);
ResultList *searchResultList(ResultNode *root, TableName tableName);
void showResultTree(ResultNode *root);
void showResultList(ResultList *list);

#endif /* SEARCH_STRUCTS_H */