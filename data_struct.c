#include "data_struct.h"

// Adiciona os dados de uma row no início da lista
// result: Início de uma lista de resultados
// row_raw: dados adicionados à lista
// return: Novo início da lista
Result *addResult(Result *result, char *row_raw) {
	Result *r = malloc(sizeof(Result));
	
	if (!r) return NULL;

	r->next = result;
	r->row_raw = row_raw;

	return r;
}

void freeResultList(Result *result) {
	if (result) {
		freeResultList(result->next);
		free(result->row_raw);
		free(result);
	}
}

// Adiciona uma lista de resultados à uma árvore de resultados
// root: Raiz da árvore
// name: Nome a ser associado ao resultado
// row_length: tamanho em bytes da row
// result: lista de resultados
// return: Raiz da nova lista
Dnode *addDnode(Dnode *root, TableWRep *meta, Result *result) {
	if (root == NULL) {
		Dnode *n = malloc(sizeof(Dnode));
		if (!n)
			return n;

		n->left = NULL;
		n->right = NULL;
		n->meta = meta;
		n->result_list = result;

		return n;
	} else {
		if (!strcmp(root->meta->name, meta->name)) {
			freeResultList(root->result_list);
			root->result_list = result;
		} else if (strOrder(root->meta->name, meta->name)) {
			root->left = addDnode(root->left, meta, result);
		} else {
			root->right = addDnode(root->right, meta, result);
		}

		return root;
	}
}

// Acha uma lista de resultados
// root: Árvore de resultados
// name: Nome associado à lista
// return: Ponteiro para lista
Dnode *findResultList(Dnode *root, char *name) {
	// Retorna NULL se o nome não é encontrado
	if (!root) {
		return NULL;
	}

	if (!strcmp(root->meta->name, name)) {
		return root;
	} else if (strOrder(root->meta->name, name)) {
		return findResultList(root->left, name);
	} else {
		return findResultList(root->right, name);
	}
}

void freeDnodeTree(Dnode *root) {
	if (!root) return;
	freeDnodeTree(root->left);
	freeDnodeTree(root->right);
	freeResultList(root->result_list);
	free(root->meta);
	free(root);
}

void printTree(Dnode *root) {
	if (!root) return;
	printTree(root->left);
	printf("%s\n", root->meta->name);
	printTree(root->right);
}
