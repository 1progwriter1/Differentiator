#include <stdio.h>
#include "bin_tree.h"
#include <assert.h>
#include "../MyLibraries/headers/systemdata.h"
#include "../MyLibraries/headers/func.h"
#include <string.h>

static int WalkTree(TreeNode *node, size_t *col_nodes);

int TreeRootCtor(TreeStruct *tree) {

    assert(tree);

    tree->root = TreeNodeNew(tree, {} ,NULL, NULL);
    if (!tree->root)
        return NO_MEMORY;

    return SUCCESS;
}

TreeNode *TreeNodeNew(TreeStruct *tree, NodeValue value, TreeNode *left, TreeNode *right) {

    TreeNode *node = (TreeNode *) calloc (1, sizeof (TreeNode));
    if (!node)
        return NULL;

    node->value = value;
    node->left = left;
    node->right = right;

    tree->size++;

    return node;
}

int TreeRootDtor(TreeStruct *tree) {

    assert(tree);
    assert(tree->root);

    if (tree->root->left || tree->root->right) {
        NodeDtor(tree, tree->root);
    }
    free(tree->root);
    tree->root = NULL;
    tree->size = 0;

    return SUCCESS;
}

int NodeDtor(TreeStruct *tree, TreeNode *node) {

    assert(tree);
    assert(node);

    if (node->left) {
        NodeDtor(tree, node->left);
        node->left = NULL;
    }
    if (node->right) {
        NodeDtor(tree, node->right);
        node->right = NULL;
    }

    free(node);

    tree->size--;

    return SUCCESS;
}

int TreeInsertNum(TreeStruct *tree, const Tree_t number) {

    assert(tree);

    if (TreeVerify(tree) != SUCCESS)
        return ERROR;

    if (!tree->root) {
        tree->root->value = number;
        return SUCCESS;
    }

    TreeNode *ptr = tree->root;
    while (1) {
        if (number < ptr->value)
            if (!ptr->left) {
                ptr->left = TreeNodeNew(tree, number, NULL, NULL);
                if (!ptr->left)
                    return NO_MEMORY;
                break;
            }
            else {
                ptr = ptr->left;
            }
        else
            if (!ptr->right) {
                ptr->right = TreeNodeNew(tree, number, NULL, NULL);
                if (!ptr->right)
                    return NO_MEMORY;
                break;
            }
            else {
                ptr = ptr->right;
            }
    }

    return SUCCESS;
}

int TreeVerify(TreeStruct *tree) {

    assert(tree);

    size_t col_nodes = 0;
    if (tree->root)
        col_nodes = 1;
    else {
        printf(RED "NUll Root" END_OF_COLOR "\n");
        return NULL_POINTER;
    }

    WalkTree(tree->root, &col_nodes);

    if (col_nodes != tree->size) {
        printf(RED "Incorrect size of the tree" END_OF_COLOR "\n");
        return ERROR;
    }

    return SUCCESS;
}

static int WalkTree(TreeNode *node, size_t *col_nodes) {

    assert(node);
    assert(col_nodes);

    if (node->left) {
        *col_nodes += 1;
        WalkTree(node->left, col_nodes);
    }
    if (node->right) {
        *col_nodes += 1;
        WalkTree(node->right, col_nodes);
    }

    return SUCCESS;
}

/* int TreeRootDtorDiff(TreeStruct *tree) {

    assert(tree);
    assert(tree->root);

    if (tree->root->left || tree->root->right) {
        TreeNodeDtorDiff(tree, tree->root);
    }

    tree->root = NULL;
    tree->size = 0;

    return SUCCESS;
}

int TreeNodeDtorDiff(TreeStruct *tree, TreeNode *node) {

    assert(tree);
    assert(node);


    if (node->left) {
        TreeNodeDtorDiff(tree, node->left);
        node->left = NULL;
    }
    if (node->right) {
        TreeNodeDtorDiff(tree, node->right);
        node->right = NULL;
    }
    free(node->value);
    node->value = NULL;
    free(node);

    tree->size--;

    return SUCCESS;
} */