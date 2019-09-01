#include "data_struct.h"

// Lista de resultados
struct result {
	struct result *next;
	Row *row;
};

typedef struct result Result;

// Adiciona uma row no início da lista
// result: Início de uma lista de resultados
// row: row a ser adicionada à lista
// return: Novo início da lista
Result *addResult(Result *result, Row *row) {
	Result *r = malloc(sizeof(Result));
	
	if (!r) return NULL;

	r->next = result;
	r->row = row;

	return r;
}

// Árvore dicionário para armazenamento dos resultados
struct dnode {
	struct dnode *left;
	struct dnode *right;
	char *name;
	Result *resultList;
};

typedef struct dnode Dnode;

// Adiciona uma lista de resultados à uma árvore de resultados
// root: Raiz da árvore
// name: Nome a ser associado ao resultado
// result: lista de resultados
// return: Raiz da nova lista
Node *addNode(Node *root, char *name, Result *result) {
	if (root == NULL) {
		Node *n = malloc(sizeof(Node));
		if (!n)
			return n;

		n->left = NULL;
		n->right = NULL;
		n->name = name;
		n->resultList = result;

		return n;
	} else {
		if (strBigger(root->name, name)) {
			root->left = addNode(root->left, name, result);
		} else {
			root->right = addNode(root->right, name, result);
		}

		return root;
	}
}

// Acha uma lista de resultados
// root: Árvore de resultados
// name: Nome associado à lista
// return: Ponteiro para lista
Node *findResultList(Node *root, char *name) {
	// Retorna NULL se o nome não é encontrado
	if (!root) {
		return NULL;
	}

	if (!strcmp(root->name, name)) {
		return root;
	} else if (strBigger(root->name, name)) {
		return findResultList(root->left, name);
	} else {
		return findResultList(root->right, name);
	}
}
