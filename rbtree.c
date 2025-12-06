#include "rbtree.h"

#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/select.h>
#include <sys/types.h>

enum {
    RED = 0,
    BLACK = 1
};

enum Direction {
    LEFT = -1,
    RIGHT = 1
};

typedef struct Node Node;

// struct RBTree {
//     Node *root;
//     rbtree_cmp_t cmp;
// };

RBTree* rbtree_ctor(rbtree_cmp_t cmp_func){
    RBTree *t = malloc(sizeof(RBTree));
    t->root = NULL;
    t->cmp = cmp_func;
    return t;
}

// ==================== СЛУЖЕБНОЕ ====================

// struct Node {
//     RBTree_key key;
//     char color;
//     void *payload;
//     Node *left, *right, *parent;
// };

/**
 * @brief Аллоцируем красный элемент, зануляем указатели в нём.
 * 
 * @return Node* 
 */
static Node* node(){
    Node* tree = malloc(sizeof(Node));
    tree->left = NULL; tree->parent = NULL; tree->right = NULL;
    tree->color = RED;
    return tree;
}

static Node* _grandpa(Node *tree){
    if (tree != NULL && tree->parent != NULL)
        return tree->parent->parent;
    else
        return NULL;
}

static Node* _uncle(Node *tree){
    Node *g = _grandpa(tree);
    if (g == NULL) return NULL;
    if (g->left == tree->parent)
        return g->right;
    else
        return g->left;
}

static Node* _sibling(Node *n){
    if (n == NULL || n->parent == NULL) return NULL;
	if (n == n->parent->left)
		return n->parent->right;
	else
		return n->parent->left;
}

/// a уйдет влево вниз, на его место встанет a->left
static void rot_left(Node *a){
    assert(a != NULL);
    Node* b = a->right;
    assert(b != NULL);
    Node* parent = a->parent;

    if (parent != NULL){
        assert(parent->left || parent->right);
        if (parent->left == a)
            parent->left = b;
        else
            parent->right = b;
    }
    a->right = b->left;
    if(b->left) b->left->parent = a;
    b->left = a; a->parent = b;
    b->parent = parent;
}

/// a уйдет вправо вниз, на его место встанет a->left
static void rot_right(Node *a){
    assert(a != NULL);
    Node* b = a->left;
    assert(b != NULL);
    Node* parent = a->parent;

    if (parent != NULL){
        assert(parent->left || parent->right);
        if (parent->left == a)
            parent->left = b;
        else
            parent->right = b;
    }
    a->left = b->right;
    if (b->right) b->right->parent = a;
    b->right = a;       a->parent = b;
    b->parent = parent;
}

/// узел черный если черный или null
static int _is_black(Node *n){
    return n == NULL || n->color == BLACK;
}


// ==================== ПОИСК ====================

static Node* _leftmost(Node *node){
    if (node == NULL) return NULL;
    while(node->left != NULL){
        node = node->left;
    }
    return node;
}

static Node* _rightmost(Node *node){
    if (node == NULL) return NULL;
    while(node->right != NULL){
        node = node->right;
    }
    return node;
}

void* rbtree_get_leftmost(const RBTree *tree){
    return _leftmost(tree->root)->payload;
}

void* rbtree_get_rightmost(const RBTree *tree){
    return _rightmost(tree->root)->payload;
}

static Node* _next(const Node* node){
    assert(node != NULL);
    Node* r = _leftmost(node->right);
    assert(r != NULL);
    return r;
}

static Node* _get(Node* node, const rbtree_cmp_t cmp, const RBTree_key key){
    assert(cmp != NULL);
    while (node != NULL){
        if(cmp(&node->key, &key) < 0)
            node = node->right;
        else if (cmp(&node->key, &key) > 0)
            node = node->left;
        else
            return node;
    }
    return NULL;
}

void* rbtree_get(const RBTree *tree, const RBTree_key key){
    assert(tree != NULL);
    Node* node = tree->root;
    node = _get(node, tree->cmp, key);
    return node ? node->payload : NULL;
}

// ==================== ВСТАВКА ====================

int rbtree_insert(RBTree *tree, const RBTree_key key, void* payload){
    assert(tree != NULL);
    Node* n = tree->root;
    Node* parent = NULL;
    enum Direction dir;
    while (n != NULL){
        parent = n;

        if(tree->cmp(&n->key, &key) < 0){ // текущий меньше искомого
            n = n->right;
            dir = RIGHT;
        } else if (tree->cmp(&n->key, &key) > 0){ // текущий больше искомого
            n = n->left;
            dir = LEFT;
        } else
            return 0;
    }
    n = node();
    n->key = key;
    n->payload = payload;
    n->parent = parent;

    if(tree->root == NULL){
        tree->root = n;
        return 1;
    }else{
        assert(dir == RIGHT || dir == LEFT);
        if (dir == RIGHT)
            parent->right = n;
        else
            parent->left = n;
    }

    // проверяем свойства и балансируем
    for(;;){
        parent = n->parent;

        // инварианты цикла
        assert(n->color == RED); // текущий узел красный

        if (parent == NULL){ // n - корень
            return 1;
        }
        if (_is_black(parent)){ // родитель черный - всё норм
            return 1;
        }
        Node* gp = parent->parent;
        if (gp == NULL){ // родитель корень
            // можно было бы проверить на красный, но я просто установлю черный
            // если корень красный, то нарушается красное правило
            parent->color = BLACK;
            return 1;
        }

        Node* uncle = _uncle(n);
        // по св-вам дерева если родитель красный, то дедушка черный
        assert(parent->color == RED && gp->color == BLACK);

        if (_is_black(uncle)){ // если дядя черный (в том числе null)
            /* N = n (текущий), P = родитель, G = дедушка, a = просто какой-то узел
            * черн       G         G
            *           /           \
            * красн    P      или    P
            *         / \           / \
            * красн  a   N         N   a
            */
            if (n == parent->right && parent == gp->left){
                /*
                * например, повернём влево по N--P =>
                *        G
                *       /
                *      N
                *     / \
                *    P  (правое дитя N)
                *   / \
                *  a  (левое дитя N)
                */
                rot_left(parent);
                n = parent;
                parent = parent->parent;
                // привели к следующему
            }else if(n == parent->left && parent == gp->right){
                rot_right(parent);
                n = parent;
                parent = parent->parent;
                // привели к следующему
            }

            // следующий случай
            assert(
                (n == parent->left && parent == gp->left)
                || (n == parent->right && parent == gp->right)
            );

            if (n == parent->left && parent == gp->left){
                rot_right(gp); // поворачиваем направо по P--G
                /*
                *        G                    P (новый корень, если у P нет родителя)
                *       / \                 /  \
                *      P   d              N      G
                *     / \      ==>       / \    / \
                *    N   c              a   b  c   d
                *   / \
                *  a   b
                */
            }else{
                rot_left(gp);
            }
            // обновить корень если у P нет родителя
            if (parent->parent == NULL){
                tree->root = parent;
            }
            // перекрашиваем, после поворота, 
            parent->color = BLACK;
            gp->color = RED;
            return 1;
        }

        // и дядя и родитель красные
        parent->color = BLACK;
        uncle->color = BLACK;
        gp->color = RED; // теперь надо проверить дедушку, вдруг у него родитель был красный

        n = gp;
    }
    assert(0); // не должен достигаться
}

// ==================== ОБХОДЫ ====================

void rbtree_iter_wide(RBTree *tree, rbtree_Apply_t f){
    Node *node = tree->root;
    Queue *q = queue();
    queue_push(q, node);
    while( ! queue_empty(q)){
        node = queue_peek(q);
        queue_pop(q);
        f(node->key, node->payload);
        if (node->left)
            queue_push(q, node->left);
        if (node->right)
            queue_push(q, node->right);
    }
    queue_free(&q);
}


static void _iter_pre_deep(Node *node, rbtree_Apply_t f){
    if (node == NULL) return;
    int r = f(node->key, node->payload);
    if (r) return;
    _iter_pre_deep(node->left, f);
    _iter_pre_deep(node->right, f);
}
void rbtree_iter_pre_deep(RBTree *tree, rbtree_Apply_t f){
    Node *node = tree->root;
    _iter_pre_deep(node, f);
}

static void _iter_post_deep(Node *node, rbtree_Apply_t f){
    if (node == NULL) return;
    _iter_post_deep(node->left, f);
    _iter_post_deep(node->right, f);
    int r = f(node->key, node->payload);
    if (r) return;
}
void rbtree_iter_post_deep(RBTree *tree, rbtree_Apply_t f){
    Node *node = tree->root;
    _iter_post_deep(node, f);
}

static void _iter_center_deep(Node *node, rbtree_Apply_t f){
    if (node == NULL) return;
    _iter_center_deep(node->left, f);
    int r = f(node->key, node->payload);
    if (r) return;
    _iter_center_deep(node->right, f);
}
void rbtree_iter_center_deep(RBTree *tree, rbtree_Apply_t f){
    Node *node = tree->root;
    _iter_center_deep(node, f);
}

// ==================== УДАЛЕНИЕ ====================

void _free(Node * node){
    assert(node);
    Node* parent = node->parent;
    if (parent){
        if (node == parent->left){
            parent->left = NULL;
        }else{
            parent->right = NULL;
        }
    }
    free(node);
}

void _replace(Node* dest, Node* src){
    dest->key = src->key;
    dest->payload = src->payload;
}

void rbtree_remove(RBTree *tree, int key){
    assert(tree != NULL);
    
    Node* node = _get(tree->root, tree->cmp, key);
    if (node == NULL) return;
    
    int childrens = ( !! node->left) + ( !! node->right);

    // два ребенка
    if (childrens == 2){
        // меняем со следующим по порядку узлом
        Node* successor = _next(node);
        _replace(node, successor);
        node = successor; // и считаем, что удаляем его
        childrens = ( !! node->left) + ( !! node->right);
        assert(childrens <= 1);
    }
    if (childrens == 1){
        Node* child = node->left ? node->left : node->right;
        assert(child != NULL);
        assert(child->color == RED); // единственное дитя обязано быть красным
        assert(node->color  == BLACK); // т.к. дитё красное. то родитель черный
        _replace(node, child);
        node->color = BLACK;
        _free(node);
        return;
    }
    if (childrens == 0){
        if(node->parent == NULL){
            _free(node); // дерево становиться пустым
            return;
        }
        if(node->color == RED){
            _free(node);
            return;
        }

        assert(node->color == BLACK);
        // удаление черного требует перебалансировки
        for(;;){
            Node* parent = node->parent;

            if(parent == NULL){ // корень
                _free(node);
                return;
            }

            enum Direction dir = node == parent->left ? LEFT : RIGHT;
            Node *sibling, *close, *distant;
            if(dir == LEFT){
                sibling = parent->right;
                close = sibling->left;
                distant = sibling->right;
            }else{
                sibling = parent->left;
                close = sibling->right;
                distant = sibling->left;
            }

            if (sibling->color == RED){
                assert(_is_black(close) && _is_black(distant)); // дети красного черные
                assert(parent->color == BLACK); // родитель красного черный
                if (dir == LEFT){ // S правое дитё
                    rot_left(parent);
                }else{
                    rot_right(parent);
                }
                if (parent->parent == NULL){ // уронили Parent, теперь sibling может быть корнем
                    tree->root = sibling;
                }

                parent->color = RED;
                sibling->color = BLACK;
                continue;
            }

            if (parent->color == RED){
                parent->color = BLACK;
                sibling->color = RED;
                _free(node);
                return;
            }

            if (close && close->color == RED){
                if (dir == LEFT){ // close левоё дитё брата N
                    rot_right(sibling);
                }else{
                    rot_left(sibling);
                }
                close->color = BLACK;
                sibling->color = RED;
                continue;
            }

            if (distant && distant->color == RED){
                if (dir == LEFT){ // N левое дитя P
                    rot_left(parent);
                }else{
                    rot_right(parent);
                }

                if (parent->parent == NULL){ // уронили Parent, теперь sibling может быть корнем
                    tree->root = sibling;
                }

                sibling->color = parent->color;
                parent->color = BLACK;
                distant->color = BLACK;
                _free(node);
                return;
            }

            assert(parent->color == BLACK && _is_black(sibling)
                    && _is_black(close) && _is_black(distant));
            sibling->color = RED;
            node = parent;
        }
    }
}

// ==================== УДАЛЕНИЕ ВСЕГО ДЕРЕВА ====================

static void _iter_post_deep_4free(Node *node){
    if (node == NULL) return;
    _iter_post_deep_4free(node->left);
    _iter_post_deep_4free(node->right);
    free(node);
}
void rbtree_free(RBTree **tree){
    if (tree == NULL || *tree == NULL) return;
    _iter_post_deep_4free((*tree)->root);
    free((*tree));
    (*tree) = NULL;
}
