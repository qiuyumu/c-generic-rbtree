#include <stdio.h>
#include <stdlib.h>
#include "rbtree.h"

int cmp_int(const void* a, const void* b){
    int a_value = *(int*)a, b_value = *(int*)b;
    if(a_value < b_value)
        return -1;
    else if(a_value > b_value)
        return 1;
    return 0;
}
void print_int(const void* data){
    printf("%d ", *(int*)data);
}
int main(){
    int* data = (int*)malloc(sizeof(int) * 10);
    rbtree_t* tree = rbtree_init(sizeof(int), cmp_int);
    for(int i = 0; i < 10; ++i){
        data[i] = i;
        rbtree_insert(tree, &data[i]);
    }

    rbtree_traversal(tree, print_int);
    printf("\n");
    rbtree_destroy(tree);
    
    return 0;

}