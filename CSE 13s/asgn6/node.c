#include "node.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

Node *node_create(uint8_t symbol, double weight) {
    Node *node = calloc(1, sizeof(Node));
    assert(node != NULL);

    node->symbol = symbol;
    node->weight = weight;
    node->code = 0;
    node->code_length = 0;
    node->left = NULL;
    node->right = NULL;

    return node;
}
//Problem occurs with Node_free
void node_free(Node **node) //create node_free
{
    if (*node == NULL) {
        return;
    }
    free(*node);
    *node = NULL;
}

void tree_free(Node **node) { //rename node_free to tree_free
    if (*node == NULL) {
        return;
    }

    node_free(&(*node)->left);
    node_free(&(*node)->right);

    free(*node);
    *node = NULL;
}

void node_print_tree(Node *tree, char ch, int indentation) {
    if (tree == NULL)
        return;
    node_print_tree(tree->right, '/', indentation + 3);
    printf("%*cweight = %.0f", indentation + 1, ch, tree->weight);
    if (tree->left == NULL && tree->right == NULL) {
        if (' ' <= tree->symbol && tree->symbol <= '~') {
            printf(", symbol = '%c'", tree->symbol);
        } else {
            printf(", symbol = 0x%02x", tree->symbol);
        }
    }
    printf("\n");
    node_print_tree(tree->left, '\\', indentation + 3);
}
