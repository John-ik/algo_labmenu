#include "rbtree.h"

#include <assert.h>
#include <stdio.h>

static size_t node_count_leaf(Node *node){
    if (node == NULL) return 0;
    if ( ! node->left && ! node->right) return 1; //  у узла нету детей
    return node_count_leaf(node->left) + node_count_leaf(node->right);
}

static size_t node_remove_leafs(RBTree* tree, Node* node){
    if (node == NULL) return 0;
    int removed_child = 0;
    removed_child += node_remove_leafs(tree, node->left);
    removed_child += node_remove_leafs(tree, node->right);
    if ( removed_child == 0 && (! node->left && ! node->right)){ //  у узла нету детей
        rbtree_remove(tree, node->key);
    }
}

static void rbtree_remove_leafs(RBTree *tree){
    node_remove_leafs(tree, tree->root);
}

size_t rbtree_count_leaf(RBTree *tree){
    Node *node = tree->root;
    return node_count_leaf(node);
}

int key_cmp(const void * lhs, const void * rhs){
    return *(int*)lhs - *(int*) rhs;
}

void printTREE(struct Node* tree);

int iteractive_tree(){
    RBTree* rbtree = rbtree_ctor(&key_cmp);
    
    while( ! feof(stdin)){
        int a;
        int n = scanf("%i", &a);
        if (n != 1) break;

        int was_inserted = rbtree_insert(rbtree, a, NULL);
    }
    printTREE(rbtree->root);
    printf("Листьев в дереве: %zu\n", rbtree_count_leaf(rbtree));

    rbtree_remove_leafs(rbtree);
    puts("Удалили листья");

    printTREE(rbtree->root);
    printf("Листьев в дереве: %zu\n", rbtree_count_leaf(rbtree));
}
