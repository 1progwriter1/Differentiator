#include <stdio.h>
#include "bin_tree.h"
#include <assert.h>
#include "../../../MyLibraries/headers/systemdata.h"
#include "../../../MyLibraries/headers/func.h"
#include <string.h>
#include "../differentiator/calculate.h"
#include "../vector/my_vector.h"
#include  "../../../MyLibraries/headers/file_func.h"

static int WalkTree(TreeNode *node, size_t *col_nodes);
static int WriteNodeInFile(TreeNode *node, FILE *file, Vector *vars);

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

TreeNode *TreeNodeNewSafe(TreeStruct *tree, NodeValue value, TreeNode *left, TreeNode *right) {

    if (!right)
        return NULL;

    if (value.type == OPERATION && GetNumOfArgs(value.operation) == 2)
        if (!left)
            return NULL;

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

    if (!tree->root)
        return SUCCESS;

    if (tree->root->left || tree->root->right) {
        NodeDtor(tree, tree->root);
    }

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

int WriteInFile(TreeStruct *tree, Vector *vars, const char *filename) {

    assert(tree);
    assert(vars);
    assert(filename);

    if (VarsVerify(vars) != SUCCESS)
        return ERROR;

    FILE *fn = openFile(filename, WRITE);
    if (!fn)
        return FILE_OPEN_ERROR;

    if (WriteNodeInFile(tree->root, fn, vars) != SUCCESS) {
        printf(RED "Error while printing tree" END_OF_COLOR "\n");
        return ERROR;
    }

    return SUCCESS;
}

static int WriteNodeInFile(TreeNode *node, FILE *file, Vector *vars) {

    assert(file);
    assert(vars);

    if (!node) {
        fprintf(file, "_");
        return SUCCESS;
    }

    fprintf(file, "(");

    if (WriteNodeInFile(node->left, file, vars) != SUCCESS)
        return ERROR;

    #define DEF_OP(name, code, sym, ...)\
        case (code): {                  \
            fprintf(file, sym);         \
            break;                      \
        }

    switch (node->value.type) {
        case (NUMBER): {
            fprintf(file, "%lg", node->value.number);
            break;
        }
        case (OPERATION): {
            switch (node->value.operation) {
                #include "../data/operations.h"
                default: {
                    printf(RED "Incorrect operation (printing tree)" END_OF_COLOR "\n");
                    return ERROR;
                }
            }
            break;
        }
        case (VARIABLE): {
            fprintf(file, "%c", vars->data[node->value.nvar].name);
            break;
        }
        default: {
            printf(RED "Incorrect value type (printing tree)" END_OF_COLOR "\n");
            return ERROR;
        }
    }

    #undef DEF_OP

    if (WriteNodeInFile(node->right, file, vars) != SUCCESS)
        return ERROR;

    fprintf(file, ")");

    return SUCCESS;

}