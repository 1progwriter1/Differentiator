#include <stdio.h>
#include "../differentiator/differentiator.h"
#include "../../../MyLibraries/headers/systemdata.h"
#include "../bin_tree/bin_tree.h"
#include <assert.h>
#include <math.h>
#include "../../../MyLibraries/headers/func.h"
#include "../graph_gen/gen_graph_diff.h"

#undef CALC
#define CALC(node) CalculateNode(data, node, error)

const size_t NUM_OF_VAR = 1;

enum NumOfArgs {
    NO_ARGS = 0,
    ONE     = 1,
    TWO     = 2,
};

static int OperationVerify(const TreeNode *node);
static int GetNumOfArgs(const Operation operation);
int TreeCopy(Differentiator *data_src, Differentiator *data_dst);
static int NodeValueCopy(TreeNode *src, TreeNode *dst);

int DiffCtor(Differentiator *data) {

    assert(data);

    if (TreeRootCtor(&data->tree) != SUCCESS)
        return ERROR;

    data->var = (double *) calloc (NUM_OF_VAR, sizeof (double));
    if (!data->var)
        return NO_MEMORY;

    data->is_get = false;

    return SUCCESS;
}

int DiffDtor(Differentiator *data) {

    assert(data);

    TreeRootDtor(&data->tree);
    free(data->var);
    data->var = NULL;

    return SUCCESS;
}

NodeValue *CreateNodeValue(ValueType type, Operation operation, double num, int nvar) {

    NodeValue *ptr = (NodeValue *) calloc (1, sizeof (NodeValue));
    if (!ptr)
        return NULL;

    ptr->type = type;

    switch ((int) type) {
        case (OPERATION): {
            ptr->operation = operation;
            break;
        }
        case (NUMBER): {
            ptr->number = num;
            break;
        }
        case (VARIABLE): {
            ptr->nvar = (size_t) nvar;
            break;
        }
        default: {
            printf(RED "Incorrect value type" END_OF_COLOR "\n");
            return NULL;
        }
    }

    return ptr;
}

int CalculateTree(Differentiator *data, double *answer) {

    assert(data);
    assert(answer);

    if (TreeVerify(&data->tree) != SUCCESS)
        return ERROR;

    bool error = 0;
    *answer = CalculateNode(data, data->tree.root, &error);

    if (error)
        return ERROR;

    return SUCCESS;
}

double CalculateNode(Differentiator *data, TreeNode *node, bool *error) {

    assert(node);
    assert(error);

    if (*error) {
        return ERROR;
    }

    if (node->value.type == NUMBER) {
        return node->value.number;
    }

    if (node->value.type == VARIABLE) {
        if (!data->is_get) {
            data->is_get = true;
            return GetVarialble();
        }
        else
            return data->var[0];
    }

    if (OperationVerify(node) != SUCCESS) {
        *error = true;
    }

    #define DEF_OP(name, code, sym, args, diff, calc, ...) \
        case (code): {                                \
            return calc;                              \
        }

    switch (node->value.operation) {

        #include "../data/operations.h"

        default: {
            printf(RED "Incorrect operation" END_OF_COLOR "\n");
            *error = true;
            return ERROR;
        }
    }

    #undef DEF_OP

    return SUCCESS;
}

double GetVarialble() {

    double var = 0;

    bool correct = 1;
    do {
        correct = 1;
        printf(MAGENTA "Enter the value of the variable: " END_OF_COLOR);
        if (scanf("%lg", &var) != 1) {
            printf(RED "Incorrect input. Try again: " END_OF_COLOR);
            buf_clear();
            correct = 0;
        }
    } while (!correct);

    return var;
}

static int OperationVerify(const TreeNode *node) {

    assert(node);

    switch (GetNumOfArgs(node->value.operation)) {
        case (ONE): {
            if (!node->right) {
                printf(RED "Missing argument. A null pointer was received." END_OF_COLOR "\n");
                return ERROR;
            }
            if (node->left) {
                printf(RED "An unnecessary argument for a operation that requires a single argument" END_OF_COLOR "\n");
                return ERROR;
            }
            break;
        }
        case (TWO): {
            if (!node->left || !node->right) {
                printf(RED "Incorrect number of arguments for operation that requires two arguments" END_OF_COLOR "\n");
                return ERROR;
            }
            break;
        }
        default: {
            printf(RED "Invalid number of arguments" END_OF_COLOR "\n");
            return ERROR;
        }
    }
    Operation operation = node->value.operation;
    if (operation == DIV) {
        if (node->right->value.type == NUMBER && IsEqual(node->right->value.number, 0)) {
            printf(RED "Division by zero" END_OF_COLOR "\n");
            return ERROR;
        }
        return SUCCESS;
    }
    if (operation == SQRT) {
        if (node->right->value.type == NUMBER && node->right->value.number < 0) {
            printf(RED "The square root of a negative number" END_OF_COLOR "\n");
            return ERROR;
        }
        return SUCCESS;
    }
    if (operation == LN) {
        if (node->right->value.type == NUMBER && node->right->value.number <= 0) {
            printf(RED "The ln of a non-positive number" END_OF_COLOR "\n");
            return ERROR;
        }
        return SUCCESS;
    }

    return SUCCESS;
}

static int GetNumOfArgs(const Operation operation) {

    #define DEF_OP(name, code, sym, args, ...)  \
        case (code): {                          \
            num_of_args = args;                 \
            break;                              \
        }

    int num_of_args = NO_ARGS;

    switch (operation) {

        #include "../data/operations.h"

        default: {
            num_of_args = NO_ARGS;
            break;
        }
    }

    return num_of_args;
}

int TreeCopy(Differentiator *data_src, Differentiator *data_dst) {

    assert(data_src);
    assert(data_dst);

    if (DiffCtor(data_dst) != SUCCESS)
        return ERROR;

    NodeDtor(&data_dst->tree, data_dst->tree.root);
    data_dst->tree.root = NodeCopy(data_dst, data_src->tree.root);
    if (!data_dst->tree.root)
        return ERROR;

    return SUCCESS;
}

TreeNode* NodeCopy(Differentiator *data, TreeNode *src) {

    assert(data);
    assert(src);

    TreeNode *ptr = TreeNodeNew(&data->tree, {}, NULL, NULL);
    if (!ptr)
        return NULL;

    if (NodeValueCopy(src, ptr) != SUCCESS)
        return NULL;

    if (src->left) {
        ptr->left = NodeCopy(data, src->left);
        if (!ptr->left)
            return NULL;
    }

    if (src->right) {
        ptr->right = NodeCopy(data, src->right);
        if (!ptr->right)
            return NULL;
    }

    return ptr;
}

static int NodeValueCopy(TreeNode *src, TreeNode *dst) {

    assert(src);
    assert(dst);

    dst->value = src->value;

    return SUCCESS;
}