

# C-Generic-RBtree

## Red–black tree

满足如下性质的二叉搜索树：

* 每个结点或者是红色的，或者是黑色的。
* 根结点是黑色的。
* 每个叶结点是黑色的。
* 如果一个结点是红色的，则它的两个子结点都是黑色的。
* 对于每个结点，从该结点到其所有的后代叶结点的简单路径上，均包含相同数目的黑色结点。

## API

| function                                  | description                                                  |
| ----------------------------------------- | ------------------------------------------------------------ |
| rbtree_init(data_size, cmp)               | 初始化一棵树红黑树，data_size为单个数据所占的空间，cmp为函数指针用于比较数据大小 |
| rbtree_search(tree, target)               | 检测target数据是否存在                                       |
| rbtree_insert(tree, new_data)             | 插入一条新数据                                               |
| rbtree_delete(tree, new_data)             | 删除一条数据                                                 |
| rbtree_size(tree)                         | 红黑树中所含的数据个数                                       |
| rbtree_traversal(tree, rbtree_data_print) | 红黑树的中序遍历，rbtree_data_print 为函数指针指定数据的输出方式 |
| rbtree_destroy(tree)                      | 红黑树销毁                                                   |

## Tips

红黑树的插入：

默认插入红色结点，若所插入结点的父结点为红色，则违反了性质4，此时需要对插入结点 current 进行 insert-fixup ，直到其父结点为黑色。在修正过程中 current 指针由子结点向父结点方向移动，且除了性质4外红黑树其他性质均被保持。insert-fixup 后新的红黑树的黑高可能加一或不变。



红黑树的删除：

明确三个结点的含义 deleted_node（包含的数据与要求删除的数据相等），real_deleted_node（对于红黑树来说减少了的一个红色结点或黑色结点），replacer（接替 real_deleted_node 的结点，即real_deleted_node的孩子结点）。

当 deleted_node 为叶结点时或仅有一个孩子结点时情况比较简单，real_deleted_node与deleted_node含义一致的，replacer 为 deleted_node 的孩子结点。当 deleted_node 有两个孩子时，real_deleted_node 为其右子树的最小结点。此时需要将 real_deleted_node （颜色为real_deleted_node_color）移动到 deleted_node 的位置（颜色改为 deleted_node 的颜色），real_deleted_node 的右孩子结点 replacer 接替 real_deleted_node 原本的位置。此时对于红黑树来说少了一个颜色为 real_deleted_node_color 的结点。若 real_deleted_node_color 为黑色则以 replacer 为根的树黑高相比原来减少了1，此时需要进行修正。current 指针在修正过程中均指向黑高减少了1的子树的根结点。current 结点若为红色直接将他涂黑补回了失去的一个黑高，否则不断向上移动直到根结点（此时新树的黑高与原来相比减少了1）。

## Reference

《算法导论》

[Red Black Tree: Delete(刪除資料)與Fixup(修正)](http://alrightchiu.github.io/SecondRound/red-black-tree-deleteshan-chu-zi-liao-yu-fixupxiu-zheng.html)

