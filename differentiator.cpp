#include <stdio.h>
#include "differentiator.h"
#include "../MyLibraries/headers/systemdata.h"
#include "bin_tree.h"
#include "value_buffer.h"
#include <assert.h>
#include <math.h>
#include "../MyLibraries/headers/func.h"

const double PI = 3.1415926535;

static int DiffVerify(Differentiator *data);
static double CalculateNode(TreeNode *node);
static int FindDerivativeNode(TreeNode *node);

int DiffCtor(Differentiator *data) {

    assert(data);

    if (ValueBufCtor(&data->buf) != SUCCESS)
        return ERROR;

    if (TreeRootCtor(&data->tree) != SUCCESS) {
        ValueBufDtor(&data->buf);
        return ERROR;
    }
    data->var = 0;

    return SUCCESS;
}

int DiffDtor(Differentiator *data) {

    assert(data);

    ValueBufDtor(&data->buf);
    TreeRootDtorDiff(&data->tree);

    return SUCCESS;
}

Buf_t DiffPushValue(Differentiator *data, Buf_t value) {

    assert(data);

    if (DiffVerify(data) != SUCCESS)
        return NULL;

    if (PushValue(&data->buf, value) != SUCCESS)
        return NULL;

    return value;
}

NodeValue *CreateNodeValue(ValueType type, Operation operation, double num) {

    NodeValue *ptr = (NodeValue *) calloc (1, sizeof (NodeValue));
    if (!ptr)
        return NULL;

    ptr->type = type;

    switch ((int) type) {
        case (OPERATION): {
            ptr->value.operation = operation;
            break;
        }
        case (NUMBER): {
            ptr->value.number = num;
            break;
        }
        case (VARIABLE): {
            ptr->value.number = num;
            break;
        }
        default: {
            printf(RED "Incoreect value type" END_OF_COLOR "\n");
            return NULL;
        }
    }

    return ptr;
}

static int DiffVerify(Differentiator *data) {

    assert(data);

    if (ValueBufVerify(&data->buf) != SUCCESS)
        return ERROR;

    if (TreeVerify(&data->tree) != SUCCESS)
        return ERROR;

    return SUCCESS;
}

int CalculateTree(Differentiator *data, double *answer) {

    assert(data);
    assert(answer);

    *answer = CalculateNode(data->tree.root);

    return SUCCESS;
}

static double CalculateNode(TreeNode *node) {

    assert(node);

    if (node->value->type == NUMBER) {
        return node->value->value.number;
    }
    if (node->value->type == VARIABLE) {
        return GetVarialble();
    }
    switch (node->value->value.operation) {
        case (ADD): {
            return CalculateNode(node->left) + CalculateNode(node->right);
        }
        case (SUB): {
            return CalculateNode(node->left) - CalculateNode(node->right);
        }
        case (MUL): {
            return CalculateNode(node->left) * CalculateNode(node->right);
        }
        case (DIV): {
            return CalculateNode(node->left) / CalculateNode(node->right);
        }
        case (SQRT): {
            return sqrt(CalculateNode(node->right));
        }
        case (COS): {
            return cos(CalculateNode(node->right) * PI / 180);
        }
        case (SIN): {
            return sin(CalculateNode(node->right) * PI / 180);
        }
        case (LN): {
            return log(CalculateNode(node->right) * PI / 180);
        }
        case (NO_OPERATION): {
            printf(RED "Missing operation" END_OF_COLOR "\n");
            return ERROR;
        }
        default: {
            printf(RED "Incorrect operation" END_OF_COLOR "\n");
            return ERROR;
        }
    }

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

int FindDerivative(Differentiator *data) {

    assert(data);

    if (FindDerivativeNode(data->tree.root) != SUCCESS)
        return ERROR;

    return SUCCESS;
}

static int FindDerivativeNode(TreeNode *node) {

    assert(node);

    if (node->value->type == NUMBER) {
        node->value->value.number = 0;
        return SUCCESS;
    }
    if (node->value->type == VARIABLE) {
        node->value->type = NUMBER;
        node->value->value.number = 1;
        return SUCCESS;
    }
    if (node->value->type == OPERATION) {
        switch (node->value->type) {
            case (ADD): {
                if (FindDerivativeNode(node->left) != SUCCESS)
                    return ERROR;
                if (FindDerivativeNode(node->right) != SUCCESS)
                    return ERROR;
                break;
            }
            case (SUB): {
                if (FindDerivativeNode(node->left) != SUCCESS)
                    return ERROR;
                if (FindDerivativeNode(node->right) != SUCCESS)
                    return ERROR;
                break;
            }
            default:
                break;
        }
    }

    return SUCCESS;
}
