#include <ctime>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include "../rbtree.h"

using namespace std;
#define SIZE 100000

vector<int> traversal_result(SIZE);
int n = 0;
int data_set[SIZE];

int cmp(const void* a, const void* b){
    const int *pa = (const int*)a, *pb = (const int*) b;
    if(*pa < *pb)
        return -1;
    else if(*pa > *pb)
        return 1;
    return 0;
}
void store_data(const void* data){
    traversal_result[n++] = *(int*)data;
}

int main(){
    vector<int> delete_set(SIZE);
    unordered_set<int> temp;

    srand(time(NULL));
    
    rbtree_t* rbtree = rbtree_init(sizeof(int), cmp);
    for(int i = 0; i < SIZE; ++i){
        delete_set[i] = (rand() % 100000) - 50000;
        data_set[i] = (rand() % 50000) - 25000;
        temp.insert(data_set[i]);
        rbtree_insert(rbtree, &data_set[i]);
    }

    for(int i = 0; i < delete_set.size(); ++i){
        rbtree_delete(rbtree, &delete_set[i]);
        temp.erase(delete_set[i]);
    }
    
    vector<int> excepted_result(temp.begin(), temp.end());
    sort(excepted_result.begin(), excepted_result.end());
    rbtree_traversal(rbtree, store_data);
    size_t num = rbtree_size(rbtree);

    rbtree_destroy(rbtree);
    
    if(excepted_result.size() != num){
        cout << "Test Faild" << endl;
        return -1;
    }

    int i;
    for(i = 0; i < n; ++i){
        if(excepted_result[i] != traversal_result[i]){
            break;
        }
    }
    if(i < n){
        cout << "Test Faild" << endl;
        return -1;
    }
    else
        cout << "No errors" << endl;


    return 0;
}
