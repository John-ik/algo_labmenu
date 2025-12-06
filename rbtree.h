#pragma once
#include <stddef.h>

#include "queue/queue.h"

#define RBTree_key int

/**
 * @brief Красно-черное дерево
 * 
 * Свойства:
 * 1. Узел может быть либо красным, либо чёрным и имеет двух потомков.
 * 2. Все листья, не содержащие данных — чёрные.
 * 3. Оба потомка каждого красного узла — чёрные.
 * 4. Любой простой путь от узла-предка до листового узла-потомка содержит одинаковое число чёрных узлов.
 * 
 * Красное правило: 3.
 * Черное правило: 4.
 */

/// 

typedef struct RBTree RBTree;

typedef struct Node Node;
struct Node {
    RBTree_key key;
    char color;
    void *payload;
    Node *left, *right, *parent;
};


/**
@brief Тип-функции для сравнения узлов,
сохраняется в дереве и исп внутри реализации
*/
typedef int (*rbtree_cmp_t)(const void* lhs, const void* rhs);

struct RBTree {
    Node *root;
    rbtree_cmp_t cmp;
};

/**
 * @brief Конструктор красно-черного дерева
 * 
 * @param cmp_func функция которая будет исп для сравнения узлов
 * @return ссылка на дерево
 */
RBTree* rbtree_ctor(rbtree_cmp_t cmp_func);

void* rbtree_get(const RBTree *tree, const RBTree_key key);
void* rbtree_get_leftmost(const RBTree *tree);
void* rbtree_get_rightmost(const RBTree *tree);

/**
 * @brief Вставка в существующие красно-черное дерево с балансировкой
 * 
 * @param tree не должен быть null
 * @param key ключ вставляемого узла
 * @param payload груз узла
 *
 * @return 0 если узел с таким ключом уже есть, иначе 1
 */
int rbtree_insert(RBTree *tree, RBTree_key key, void* payload);

/**
 * @brief удаление узла из дерева, ничего если узла с таким ключом не существует
 * 
 * @param tree не должен быть null
 * @param key ключ удаляемого узла
 */
void rbtree_remove(RBTree *tree, RBTree_key key);
void rbtree_free(RBTree **tree);

/**
 * @brief функция вызываемая на каждом элементе
 * @return 0 если продолжается обход, не 0 для завершения
 */
typedef int (*rbtree_Apply_t)(RBTree_key, void*);

void rbtree_iter_wide(RBTree *tree, rbtree_Apply_t f);
void rbtree_iter_pre_deep(RBTree *tree, rbtree_Apply_t f);
void rbtree_iter_post_deep(RBTree *tree, rbtree_Apply_t f);
void rbtree_iter_center_deep(RBTree *tree, rbtree_Apply_t f);
