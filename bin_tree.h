#ifndef BIN_TREE_FUNC
#define BIN_TREE_FUNC

#include "diff_values.h"
#include <stdio.h>

typedef NodeValue* Tree_t;
#define tree_output_id "%d"

struct TreeNode {
    Tree_t value;
    TreeNode *left;
    TreeNode *right;
};

struct TreeStruct {
    TreeNode *root;
    size_t size;
};

int TreeRootCtor(TreeStruct *tree);

TreeNode *TreeNodeNew(TreeStruct *tree, Tree_t value);

int TreeInsertNum(TreeStruct *tree, const Tree_t number);

int TreeRootDtor(TreeStruct *tree);

int NodeDtor(TreeStruct *tree, TreeNode *node);

int TreeInsertNum(TreeStruct *tree, const Tree_t number);

int TreeVerify(TreeStruct *tree);

int TreeNodeDtorDiff(TreeStruct *tree, TreeNode *node);

int TreeRootDtorDiff(TreeStruct *tree);

#endif