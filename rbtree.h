#ifndef RBTREE_H
#define RBTREE_H
#include <stdbool.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef enum Color{
    Red, Black
}rbtree_color_t;

typedef struct rbtree_node_t{
    struct rbtree_node_t * parent, *left, *right;
    void* data;
    rbtree_color_t color;
}rbtree_node_t;

typedef struct rbtree_t{
    size_t size; 
    size_t data_size;
    void *compare;
    struct rbtree_node_t * root, *nil;
}rbtree_t;

int rbtree_insert(rbtree_t* tree, void* new_data);
bool rbtree_search(rbtree_t* tree, void* target);
void rbtree_delete(rbtree_t* tree, void* data);
rbtree_t* rbtree_init(size_t data_size, int (*cmp)(const void*, const void*));
void rbtree_traversal(rbtree_t* tree, void(*rbtree_data_print)(const void*));
void rbtree_destroy(rbtree_t* tree);
size_t rbtree_size(rbtree_t* tree);
#ifdef __cplusplus
}
#endif

#endif