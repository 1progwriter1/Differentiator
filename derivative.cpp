#include <stdio.h>
#include "derivative.h"
#include "differentiator.h"
#include <assert.h>
#include "../MyLibraries/headers/systemdata.h"
#include "gen_graph_diff.h"
#include "../MyLibraries/headers/func.h"
#include <stdbool.h>

#define OP(operation) CreateNodeValue(OPERATION, operation, NO_NUMBER, NO_VAR)

#define NUM(num) CreateNodeValue(NUMBER, NO_OPERATION, num, NO_VAR)

#define NEW(val, left, right) /*Safe*/TreeNodeNew(&data->tree, val ,left, right)

#define D(node) FindDerivativeNode(data, node)

#define C(node) NodeCopy(data, node)

#define L node->left

#define R node->right

#define DTOR(node) TreeNodeDtorDiff(&data->tree, node);

#define IsVar(node) node->value->type == VARIABLE

enum Result {
    CHANGED,
    NOT_CHANGED,
};

// Console dump:
// Pass ...: nodes count = 2321
// Pass ...: nodes count = 1231231
//
static TreeNode* FindDerivativeNode(Differentiator *data, TreeNode *node);
static TreeNode* FirstSimplify(Differentiator *data, TreeNode *node, bool *is_changed); //BOOL???
static TreeNode* SecondSimplify(Differentiator *data, TreeNode *node, bool *is_chahged); //what??? pass
static bool IsEqualZero(TreeNode *node);
static bool IsStayEqual(TreeNode *node);

int FindDerivative(Differentiator *data, Differentiator *data_cpy) {

    assert(data);
    assert(data_cpy);

    if (DiffCtor(data_cpy) != SUCCESS)
        return ERROR;

    if (Simplify(data_cpy) != SUCCESS) {
        return ERROR;
    }

    data_cpy->tree.root = FindDerivativeNode(data_cpy, data_cpy->tree.root); // take derivative
    if (!data_cpy->tree.root) {
        return ERROR;
    }

    return SUCCESS;
}

static TreeNode* FindDerivativeNode(Differentiator *data, TreeNode *node) {

    assert(data);
    if (!node)
        return NULL;

    if (node->value->type == NUMBER) {
        node->value->value.number = 0;
        return node;
    }
    if (node->value->type == VARIABLE) {
        DTOR(node);
        return NEW(NUM(1), NULL, NULL);
    }

    TreeNode *ptr = NULL;

    #define DEF_OP(name, code, sym, args, diff, calc)   \
        case (code): {                                  \
            ptr = diff;                                 \
            if (L)                                      \
                DTOR(L);                                \
            if (R)                                      \
                DTOR(R);                                \
            break;                                      \
        }

    switch (node->value->value.operation) {
        #include "operations.h"
        case (NO_OPERATION): {
            printf(RED "Missing operation" END_OF_COLOR "\n");
            ptr = NULL;
            break;
        }
        default: {
            printf(RED "Missing operation" END_OF_COLOR "\n");
            ptr = NULL;
            break;
        }
    }

    #undef DEF_OP

    return ptr;
}

int Simplify(Differentiator *data) {

    assert(data);

    bool is_changed = false;

    data->tree.root = FirstSimplify(data, data->tree.root, &is_changed);
    data->tree.root = SecondSimplify(data, data->tree.root ,&is_changed);

    while (is_changed) {
        is_changed = false;
        data->tree.root = FirstSimplify(data, data->tree.root, &is_changed);
        if (!data->tree.root)
            return ERROR;
        if (!is_changed)
            return SUCCESS;
        data->tree.root = SecondSimplify(data, data->tree.root, &is_changed);
        if (!data->tree.root)
            return ERROR;
    } //scaling

    return SUCCESS;
}
//constexpr simplify
static TreeNode* FirstSimplify(Differentiator *data, TreeNode *node, bool *is_changed) {

    assert(node);

    if (node->right) {
        if (node->right->value->type != NUMBER) {
            node->right = FirstSimplify(data, node->right, is_changed);
            if (!node->right)
                return NULL;
        }
        if (node->left)
            if (node->left->value->type != NUMBER) {
                node->left = FirstSimplify(data, node->left, is_changed);
                if (!node->left)
                    return NULL;
            }

        if ((!node->left && node->right->value->type == NUMBER) || (node->left && node->left->value->type == NUMBER && node->right->value->type == NUMBER)) {
            bool error = 0;
            double ans = CalculateNode(data, node, &error);
            if (error)
                return NULL;
            TreeNodeDtorDiff(&data->tree, node);
            *is_changed = true;
            return NEW(NUM(ans), NULL, NULL);
        }
    }

    return node;
}

static TreeNode* SecondSimplify(Differentiator *data, TreeNode *node, bool *is_changed) {

    assert(data);
// visitor
    if (node->left)
        node->left = SecondSimplify(data, node->left, is_changed);

    if (node->right)
        node->right = SecondSimplify(data, node->right, is_changed);

    if (!node->left || !node->right)
        return node;

    if (IsEqualZero(node)) {
        TreeNodeDtorDiff(&data->tree, node);
        *is_changed = true;
        return NEW(NUM(0), NULL, NULL);
    }

    if (IsStayEqual(node)) {
        *is_changed = true;
        TreeNode *node_cpy = NULL;
        if (node->left->value->type == NUMBER)
            node_cpy = NodeCopy(data, node->left);
            if (!node_cpy)
                return NULL;
        else
            node_cpy = NodeCopy(data, node->right);
            if (!node_cpy)
                return NULL;
        TreeNodeDtorDiff(&data->tree, node);
        return node_cpy;
    }

    return node;
}

static bool IsEqualZero(TreeNode *node) { // compare trees

    assert(node);
    assert(node->left);
    assert(node->right);

    if (node->value->type == OPERATION)
        if (node->value->value.operation == MUL)
            return (node->right->value->type == NUMBER && IsEqual(node->right->value->value.number, 0)) || (node->left->value->type == NUMBER && IsEqual(node->left->value->value.number, 0));

    return false;
}

static bool IsStayEqual(TreeNode *node) {

    assert(node);
    assert(node->left);
    assert(node->right);

    if (node->value->type == OPERATION) {
        if (node->value->value.operation == MUL)
            return (node->right->value->type == NUMBER && IsEqual(node->right->value->value.number, 1)) ||
                   (node->left ->value->type == NUMBER && IsEqual(node->left ->value->value.number, 1));

        if (node->value->value.operation == ADD)
            return (node->right->value->type == NUMBER && IsEqual(node->right->value->value.number, 0)) || (node->left->value->type == NUMBER && IsEqual(node->left->value->value.number, 0));//FIXME:
    }

    return false;
}