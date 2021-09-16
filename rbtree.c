#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "rbtree.h"

#define err(str) fprintf(stderr, str)

static rbtree_node_t* rbtree_find(rbtree_t* tree, void* target);
static void rbtree_left_rotation(rbtree_t* tree, rbtree_node_t* node);
static void rbtree_right_rotation(rbtree_t* tree, rbtree_node_t* node);
static void rbtree_insert_fixup(rbtree_t* tree, rbtree_node_t *z);
static rbtree_node_t * rbtree_minimun_node(rbtree_t* tree, rbtree_node_t* x);
static void rbtree_delete_fixup(rbtree_t * tree, rbtree_node_t * current);
static void rbtree_transplant(rbtree_t* tree, rbtree_node_t* u, rbtree_node_t* v);


int rbtree_insert(rbtree_t* tree, void* data){
    if(rbtree_search(tree, data)){
        return 0;
    }
    rbtree_node_t* new_node = (rbtree_node_t*)malloc(sizeof(rbtree_node_t));
    if(new_node == NULL){
        err("rbtree_insert(): Could not allocate memory for red–black node\n");
        return -1;
    }
    new_node->data = malloc(tree->data_size);
    if(new_node->data == NULL){
        free(new_node);
        err("rbtree_insert(): Could not allocate memory for red–black node\n");       
        return -1;
    }
    memmove(new_node->data, data, tree->data_size);
    int (*cmp)(const void*, const void*) = (int (*)(const void*, const void*))tree->compare;
    rbtree_node_t *x = tree->root, *y = tree->nil;
    while(x != tree->nil){
        int cmp_result = cmp(x->data, data);
        y = x;
        if(cmp_result < 0)
            x = x->right;
        else
            x = x->left;
    }
    new_node->parent = y;
    if(y == tree->nil)
        tree->root = new_node;
    else{
        int cmp_reslut = cmp(data, y->data);
        if(cmp_reslut < 0)
            y->left = new_node;
        else
            y->right = new_node;
    }
    new_node->right = new_node->left = tree->nil;
    new_node->color = Red;
    tree->size++;
    rbtree_insert_fixup(tree, new_node);
    return 0;

}
static rbtree_node_t* rbtree_find(rbtree_t* tree, void* target){
    rbtree_node_t* node = tree->root;
    int (*cmp)(const void*, const void*) = (int (*)(const void*, const void*))tree->compare;
    while(node != tree->nil){
        int cmp_result = cmp(node->data, target);
        if(cmp_result < 0){
            node = node->right;
        }
        else if(cmp_result > 0){
            node = node->left;
        }
        else{
            break;
        }
    }

    return node;
}
bool rbtree_search(rbtree_t* tree, void* target){
    return rbtree_find(tree, target) != tree->nil;
}

static void rbtree_left_rotation(rbtree_t* tree, rbtree_node_t* node){
    rbtree_node_t* right_child = node->right;
    node->right = right_child->left;
    if(right_child->left != tree->nil)
        right_child->left->parent = node;
    right_child->parent = node->parent;
    if(node == tree->root)
        tree->root = right_child;
    else if(node == node->parent->left)
        node->parent->left = right_child;
    else
        node->parent->right = right_child;
    right_child->left = node;
    node->parent = right_child;
}

static void rbtree_right_rotation(rbtree_t* tree, rbtree_node_t* node){
    rbtree_node_t* left_child = node->left;
    node->left = left_child->right;
    if(left_child->right != tree->nil)
        left_child->right->parent = node;
    left_child->parent = node->parent;
    if(node == tree->root)
        tree->root = left_child;
    else if(node == node->parent->left)
        node->parent->left = left_child;
    else
        node->parent->right = left_child;
    left_child->right = node;
    node->parent = left_child;
}

void rbtree_insert_fixup(rbtree_t* tree, rbtree_node_t *current){

    while( current->parent->color == Red){                 // curr的父结点为红色，curr的爷爷结点必存在
        if(current->parent == current->parent->parent->left){
            rbtree_node_t * uncle = current->parent->parent->right; 
            if(uncle->color == Red){                    // Case 1: curr的叔叔结点为红色
                current->parent->color = Black;
                uncle->color = Black;
                current->parent->parent->color = Red;
                current = current->parent->parent;            // curr位置转移
            }
            else{
                if(current == current->parent->right){        // Case 2: curr的叔叔结点为黑色且curr为右孩子
                    current = current->parent;
                    rbtree_left_rotation(tree, current);
                }
                current->parent->color = Black;            // Case 3: curr的叔叔结点为黑色且curr为左孩子
                current->parent->parent->color = Red;
                rbtree_right_rotation(tree, current->parent->parent);
            }
        }
        else{                                           // 对称情况
            rbtree_node_t * uncle = current->parent->parent->left; 
            if(uncle->color == Red){
                current->parent->color = Black;
                uncle->color = Black;
                current->parent->parent->color = Red;
                current = current->parent->parent;  
            }
            else{
                if(current == current->parent->left){
                    current = current->parent;
                    rbtree_right_rotation(tree, current);
                }
                current->parent->color = Black;
                current->parent->parent->color = Red;
                rbtree_left_rotation(tree, current->parent->parent);
            }
        }

    }

    tree->root->color = Black;
}
static rbtree_node_t * rbtree_minimun_node(rbtree_t* tree, rbtree_node_t* x){
    while(x->left != tree->nil)
        x = x->left;
    return x;
}
static void rbtree_transplant(rbtree_t* tree, rbtree_node_t* u, rbtree_node_t* v){
    if(u->parent == tree->nil)
        tree->root = v;
    else if(u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    v->parent = u->parent;
}

void rbtree_delete_fixup(rbtree_t * tree, rbtree_node_t * current){
    while(current != tree->root && current->color == Black){
        if(current == current->parent->left){
            rbtree_node_t * sibling = current->parent->right;
            // Case 1:
            if(sibling->color == Red){                              
                sibling->color = Black;
                current->parent->color = Red;
                rbtree_left_rotation(tree, current->parent);
                sibling = current->parent->right;
                
            }
            // Case 2:
            if(sibling->left->color == Black && sibling->right->color == Black){
                sibling->color = Red;
                current = current->parent;
            }
            else{
                // Case 3:
                if(sibling->right->color == Black){
                    sibling->left->color = Black;
                    sibling->color = Red;
                    rbtree_right_rotation(tree, sibling);
                    sibling = current->parent->right;
                }
                // Case 4:
                sibling->color = current->parent->color;
                current->parent->color = Black;
                sibling->right->color = Black;
                rbtree_left_rotation(tree, current->parent);
                current = tree->root;

            }
        }
        else{
            rbtree_node_t * sibling = current->parent->left;
            if(sibling->color == Red){
                sibling->color = Black;
                current->parent->color = Red;
                rbtree_right_rotation(tree, current->parent);
                sibling = current->parent->left;
            }
            if(sibling->right->color == Black && sibling->left->color == Black){
                sibling->color = Red;
                current = current->parent;
            }
            else{
                if(sibling->left->color == Black){
                    sibling->right->color = Black;
                    sibling->color = Red;
                    rbtree_left_rotation(tree, sibling);
                    sibling = current->parent->left;
                }
                sibling->color = current->parent->color;
                current->parent->color = Black;
                sibling->left->color = Black;
                rbtree_right_rotation(tree, current->parent);
                current = tree->root;

            }
        }
    }

    current->color = Black;
}

void rbtree_delete(rbtree_t* tree, void* data){
    rbtree_node_t* deleted_node = rbtree_find(tree, data);
    if(deleted_node == tree->nil)
        return;
    rbtree_node_t * real_deleted_node = deleted_node, *replacer = NULL;
    rbtree_color_t real_deleted_node_color = real_deleted_node->color;
    if(deleted_node->left == tree->nil){                                        // 若被删除的结点deleted_node为叶结点
        replacer = deleted_node->right;                 
        rbtree_transplant(tree, deleted_node, deleted_node->right);
    }
    else if(deleted_node->right == tree->nil){
        replacer = deleted_node->left;
        rbtree_transplant(tree, deleted_node, deleted_node->left);
    }
    else{                                                                       // 若被删除的结点deleted_node不为叶结点
        real_deleted_node = rbtree_minimun_node(tree, deleted_node->right);
        real_deleted_node_color = real_deleted_node->color;
        replacer = real_deleted_node->right;

        if(real_deleted_node->parent == deleted_node)                           // real_deleted_node为deleted_node的直接孩子
            replacer->parent = real_deleted_node;                               // 保证replacer为nil结点时其父结点为real_deleted_node
        else{
            rbtree_transplant(tree, real_deleted_node, real_deleted_node->right);
            real_deleted_node->right = deleted_node->right;
            real_deleted_node->right->parent = real_deleted_node;
        }
        rbtree_transplant(tree, deleted_node, real_deleted_node);
        real_deleted_node->left = deleted_node->left;
        real_deleted_node->left->parent = real_deleted_node;
        real_deleted_node->color = deleted_node->color;

    }
    free(deleted_node->data);
    free(deleted_node);
    tree->size--;
    if(real_deleted_node_color == Black)                                        // 以replacer为根的树黑高与原来相比少了1
        rbtree_delete_fixup(tree, replacer);                                
    
}
rbtree_t* rbtree_init(size_t data_size, int (*cmp)(const void*, const void*)){
    rbtree_t* tree = (rbtree_t*)malloc(sizeof(rbtree_t));
    if(tree == NULL){
        err("rbtree_init(): Could not allocate memory for red–black tree\n");
        return NULL;
    }

    rbtree_node_t* nil = (rbtree_node_t*)malloc(sizeof(rbtree_node_t));
    if(nil == NULL){
        free(tree);
        err("rbtree_init(): Could not allocate memory for NIL node\n");
        return NULL;
    }
    tree->data_size = data_size;
    tree->size = 0;
    tree->compare = (void*)cmp;
    nil->color = Black;
    tree->nil = nil;
    tree->root = nil;
    return tree;
}
static void inorder_traversal(rbtree_node_t* node, rbtree_node_t* nil, void(*rbtree_data_print)(const void*)){
    if(node != nil){
        inorder_traversal(node->left, nil, rbtree_data_print);
        rbtree_data_print(node->data);
        inorder_traversal(node->right, nil, rbtree_data_print);
    }
}
void rbtree_traversal(rbtree_t* tree, void(*rbtree_data_print)(const void*)){
    inorder_traversal(tree->root, tree->nil, rbtree_data_print);
}



static void tree_destroy(rbtree_node_t* node, rbtree_node_t* nil){
    if(node != nil){
        tree_destroy(node->left, nil);
        tree_destroy(node->right, nil);
        free(node->data);
        free(node);
    }
}
void rbtree_destroy(rbtree_t* tree){
    tree_destroy(tree->root, tree->nil);
    free(tree->nil);
    free(tree);
}

size_t rbtree_size(rbtree_t* tree){
    return tree->size;
}
