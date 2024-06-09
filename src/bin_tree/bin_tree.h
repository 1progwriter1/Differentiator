#ifndef BIN_TREE_FUNC
#define BIN_TREE_FUNC

#include "diff_values.h"
#include <stdio.h>
#include "my_vector.h"

typedef NodeValue Tree_t;
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

TreeNode *TreeNodeNew(TreeStruct *tree, NodeValue value, TreeNode *left, TreeNode* rigth);

int TreeRootDtor(TreeStruct *tree);

int NodeDtor(TreeStruct *tree, TreeNode *node);

int TreeVerify(TreeStruct *tree);

TreeNode *TreeNodeNewSafe(TreeStruct *tree, NodeValue value, TreeNode *left, TreeNode *right);

int WriteInFile(TreeStruct *tree, Vector *vars, const char *filename);

#endif