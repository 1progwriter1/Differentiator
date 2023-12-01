#include <stdio.h>
#include "derivative.h"
#include "calculate.h"
#include <assert.h>
#include "../MyLibraries/headers/systemdata.h"
#include "gen_graph_diff.h"
#include "../MyLibraries/headers/func.h"
#include <stdbool.h>
#include "bin_tree.h"
#include "diff_dsl.h"
#include <math.h>

static TreeNode* TakeNodeDerivative(TreeStruct *tree, TreeNode *node, const size_t var_index);
static TreeNode* ConstEvaluate(TreeStruct *tree, TreeNode *node, bool *is_changed);
static TreeNode* RemovingNeutralElements(TreeStruct *tree, TreeNode *node, bool *is_chahged);
static TreeNode* AddSimplify(TreeStruct *tree, TreeNode *node);
static TreeNode* SubSimplify(TreeStruct *tree, TreeNode *node);
static TreeNode* MulSimplify(TreeStruct *tree, TreeNode *node);
static TreeNode* DivSimplify(TreeStruct *tree, TreeNode *node);
static TreeNode* PowSimplify(TreeStruct *tree, TreeNode *node);

int TakeTreeDerivative(TreeStruct *tree, TreeStruct *diff_tree, const size_t var_index) {

    assert(tree);
    assert(diff_tree);

    if (TreeVerify(tree) != SUCCESS)
        return ERROR;

    diff_tree->root = TakeNodeDerivative(diff_tree, tree->root, var_index);
    if (!diff_tree->root) {
        printf(RED "Differentiation error" END_OF_COLOR "\n");
        return NULL_POINTER;
    }

    CountSubTreeSize(diff_tree->root);

    return SUCCESS;
}

static TreeNode* TakeNodeDerivative(TreeStruct *tree, TreeNode *node, const size_t var_index) {

    assert(tree);
    if (!node)
        return NULL;

    if (IsNum(node)) {
        return NEW(NUM(0), NULL, NULL);
    }
    if (IsVar(node)) {
        if (node->value.nvar == var_index)
            return NEW(NUM(1), NULL, NULL);
        return NEW(NUM(0), NULL, NULL);
    }

    #define DEF_OP(name, code, sym, args, diff, calc, ...)  \
        case (code): {                                      \
            return diff;                                    \
        }

    switch (node->value.operation) {
        #include "operations.h"
        default: {
            printf(RED "Missing operation" END_OF_COLOR "\n");
            return NULL;
        }
    }

    #undef DEF_OP

    return NULL;
}

int SimplifyTree(TreeStruct *tree) {

    assert(tree);

    if (TreeVerify(tree) != SUCCESS)
        return ERROR;

    bool is_changed = false;

    while (1) {
        is_changed = false;
        tree->root = ConstEvaluate(tree, tree->root, &is_changed);
        if (!tree->root)
            return ERROR;
        tree->root = RemovingNeutralElements(tree, tree->root, &is_changed);
        if (!tree->root)
            return ERROR;
        if (!is_changed)
            break;
    }

    if (TreeVerify(tree) != SUCCESS) {
        printf(RED "Incorrect tree after simplifying" END_OF_COLOR "\n");
        return ERROR;
    }

    return SUCCESS;
}

static TreeNode* ConstEvaluate(TreeStruct *tree, TreeNode *node, bool *is_changed) {

    assert(node);
    assert(tree);
    assert(is_changed);

    if (node->right) {
        if (!(IsNum(node->right))) {
            node->right = ConstEvaluate(tree, node->right, is_changed);
            if (!node->right)
                return NULL;
        }
        if (node->left)
            if (!(IsNum(node->left))) {
                node->left = ConstEvaluate(tree, node->left, is_changed);
                if (!node->left)
                    return NULL;
            }

        if ((!node->left && IsNum(node->right)) || (node->left && IsNum(node->left) && IsNum(node->right))) {
            bool error = 0;
            double ans = CalculateNode(node, NULL ,&error);
            if (error)
                return NULL;
            NodeDtor(tree, node);
            *is_changed = true;
            return NEW(NUM(ans), NULL, NULL);
        }
    }

    return node;
}

static TreeNode* RemovingNeutralElements(TreeStruct *tree, TreeNode *node, bool *is_changed) {

    assert(tree);
    assert(node);
    assert(is_changed);

    if (node->left)
        node->left = RemovingNeutralElements(tree, node->left, is_changed);

    if (node->right)
        node->right = RemovingNeutralElements(tree, node->right, is_changed);

    if (!node->left || !node->right)
        return node;

    if (!(IsOp(node)))
        return node;

    if (OperationVerify(node) != SUCCESS)
        return NULL;

    Operation operation = node->value.operation;

    if (operation == ADD)
        return AddSimplify(tree, node);

    if (operation == SUB)
        return SubSimplify(tree, node);

    if (operation == MUL)
        return MulSimplify(tree, node);

    if (operation == DIV)
        return DivSimplify(tree, node);

    if (operation == POW)
        return PowSimplify(tree, node);

    return node;
}

static TreeNode* AddSimplify(TreeStruct *tree, TreeNode *node) {

    assert(tree);
    assert(node);

    if (IsZero(node->left)) {
        TreeNode *ptr = NodeCopy(tree, node->right);
        if (!ptr)  return NULL;

        NodeDtor(tree, node);
        return ptr;
    }
    if (IsZero(node->left)) {
        TreeNode *ptr = NodeCopy(tree, node->left);
        if (!ptr)  return NULL;

        NodeDtor(tree, node);
        return ptr;
    }

    return node;
}

static TreeNode* SubSimplify(TreeStruct *tree, TreeNode *node) {

    assert(tree);
    assert(node);

    if (IsNum(node->right) && IsZero(node->right)) {

        TreeNode *ptr = NodeCopy(tree, node->left);
        if (!ptr)  return NULL;

        NodeDtor(tree, node);
        return ptr;
    }

    return node;
}

static TreeNode* MulSimplify(TreeStruct *tree, TreeNode *node) {

    assert(tree);
    assert(node);

    if (IsZero(node->left) || IsZero(node->right)) {

        NodeDtor(tree, node);

        TreeNode *ptr = NEW(NUM(0), NULL, NULL);
        if (!ptr) return NULL;

        return ptr;
    }

    if (IsOne(node->left)) {

        TreeNode *ptr = NodeCopy(tree, node->right);
        if (!ptr) return NULL;

        NodeDtor(tree, node);
        return ptr;
    }

    if (IsOne(node->right)) {

        TreeNode *ptr = NodeCopy(tree, node->left);
        if (!ptr) return NULL;

        NodeDtor(tree, node);
        return ptr;
    }

    return node;
}

static TreeNode* DivSimplify(TreeStruct *tree, TreeNode *node) {

    assert(tree);
    assert(node);

    if (IsZero(node->left)) {

        NodeDtor(tree, node);
        TreeNode *ptr = NEW(NUM(0), NULL, NULL);
        if (!ptr)  return NULL;

        return ptr;
    }
    if (IsOne(node->right)) {

        TreeNode *ptr = NodeCopy(tree, node->left);
        if (!ptr)  return NULL;

        NodeDtor(tree, node);
        return ptr;
    }

    return node;
}

static TreeNode* PowSimplify(TreeStruct *tree, TreeNode *node) {

    assert(tree);
    assert(node);

    if (IsZero(node->left) || IsOne(node->right)) {

        TreeNode *ptr = NEW(NUM(node->left->value.number), NULL, NULL);
        NodeDtor(tree, node);
        if (!ptr) return NULL;

        return ptr;
    }
    if (IsZero(node->right)) {

        NodeDtor(tree, node);
        TreeNode *ptr = NEW(NUM(1), NULL, NULL);
        if (!ptr) return NULL;

        return ptr;
    }
    if (IsOne(node->right)) {

        TreeNode *ptr = NodeCopy(tree, node->left);
        if (!ptr)  return NULL;
        NodeDtor(tree, node);
        return ptr;
    }

    return node;
}

int CountSubTreeSize(TreeNode *node) {

    assert(node);

    node->value.subtree_size = 1;

    if (node->value.nick != 0) {
        node->value.subtree_size = 1;
        return SUCCESS;
    }

    if (node->left) {
        CountSubTreeSize(node->left);
        node->value.subtree_size += node->left->value.subtree_size;
    }
    if (node->right) {
        CountSubTreeSize(node->right);
        node->value.subtree_size += node->right->value.subtree_size;
    }

    return SUCCESS;
}