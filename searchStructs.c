#include "searchStructs.h"

void addToResultList(ResultList **list, long int pos) {
	ResultList *elem = (ResultList *)mallocSafe(sizeof(ResultList));
	elem->next = *list;
	elem->pos = pos;
	*list = elem;
}

void freeResultList(ResultList *list) {
	if (list) {
		freeResultList(list->next);
		free(list);
	}	
}

void freeResultTree(ResultNode *root) {
	if (root) {
		freeResultTree(root->left);
		freeResultTree(root->right);
		freeResultList(root->result);
		free(root);
	}
}

void addToResultTree(ResultNode **root, ResultList *list, TableName tableName) {
	if (*root) {
		int comp = strcmp(tableName, (*root)->tableName);
		if (comp == 0) {
			freeResultList((*root)->result);
			(*root)->result = list;
		} else {
			if (comp < 0) {
				if ((*root)->left) {
					addToResultTree(&((*root)->left), list, tableName);
				} else {
					ResultNode *elem = (ResultNode *)mallocSafe(sizeof(ResultNode));
					elem->left = NULL;
					elem->right = NULL;
					elem->result = list;
					strncpy(elem->tableName, tableName, TABLE_NAME_LIMIT);

					(*root)->left = elem;
				}
			} else {
				if ((*root)->right) {
					addToResultTree(&((*root)->right), list, tableName);
				} else {
					ResultNode *elem = (ResultNode *)mallocSafe(sizeof(ResultNode));
					elem->left = NULL;
					elem->right = NULL;
					elem->result = list;
					strncpy(elem->tableName, &(tableName[0]), TABLE_NAME_LIMIT);

					(*root)->right = elem;
				}
			}
		}
	} else {
		ResultNode *elem = (ResultNode *)mallocSafe(sizeof(ResultNode));
		elem->left = NULL;
		elem->right = NULL;
		elem->result = list;
		strncpy(elem->tableName, tableName, TABLE_NAME_LIMIT);

		*root = elem;
	}
}

ResultList *searchResultList(ResultNode *root, TableName tableName) {
	if (root) {
		int comp = strcmp(tableName, root->tableName);
		if (comp == 0) {
			return root->result;
		} else if (comp < 0) {
			return searchResultList(root->left, tableName);
		} else {
			return searchResultList(root->right, tableName);
		}
	} else {
		return NULL;
	}
}

void showResultTree(ResultNode *root) {
	if (root) {
		showResultTree(root->left);
		printf("%s\n", root->tableName);
		showResultList(root->result);
		showResultTree(root->right);
	}
}

void showResultList(ResultList *list) {
	if (list) {
		showResultList(list->next);
		printf("\t%x\n", list->pos);
	}
}