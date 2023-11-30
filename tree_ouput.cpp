#include <stdio.h>
#include "tree_output.h"
#include "../MyLibraries/headers/systemdata.h"
#include "../MyLibraries/headers/systemdata.h"
#include "bin_tree.h"
#include "../MyLibraries/headers/file_func.h"
#include "my_vector.h"
#include <assert.h>
#include "diff_dsl.h"
#include "calculate.h"
#include "../MyLibraries/headers/func.h"
#include "derivative.h"
#include "gen_graph_diff.h"
#include <ctype.h>

const size_t NUM_OF_PHRASES = 11;
const size_t DEPTH = 3;

const char *PHRASES[NUM_OF_PHRASES] = {
    "Несложно заметить, что следующая производная выглядит так:\n",
    "Ежу понятно, что при дальнейшем дифференцирвоании мы получим:\n",
    "Легко заметить, что далее мы получим:\n",
    "Мне уже надоело, но давайте ещё немного посчитаем:\n",
    "Следующие преобразования легко произветси в уме:\n",
    "Легко заметить, что ,продолжая вычисления, получим:\n",
    "Абсолютно очевидно, как найти следующую производную:\n",
    "Ещё разок продифференцируем:\n",
    "Лишняя производная ещё никому не мешала:\n",
    "В который раз дифференцируем:\n",
    "Мне, конечно, кажется, что уже хватит, но давайте ещё разок:\n"
};

static int PrintNodeLatex(TreeNode *node, Vector *vars, FILE *fn);
static size_t GetVarIndex(Vector *vars);

int PrintDifferentiation(TreeStruct *tree, Vector *vars, const char *filename) {

    assert(tree);
    assert(vars);
    assert(filename);

    if (TreeVerify(tree) != SUCCESS)
        return ERROR;

    if (VarsVerify(vars) != SUCCESS)
        return ERROR;

    size_t var_index = 0;
    if (vars->size > 1) {
        var_index = GetVarIndex(vars);
    }

    cleanfile(filename);

    FILE *fn = fileopen(filename, APPEND);
    if (!fn)
        return FILE_OPEN_ERROR;

    fprintf(fn, "Давайте начнем:\n");

    if (GenLatex(tree, vars, fn) != SUCCESS)
        return ERROR;


    TreeStruct diff_tree = {};
    for (size_t i = 0; i < DEPTH; i++) {
        fprintf(fn, "%s\n", PHRASES[randnum(0, NUM_OF_PHRASES - 1)]);

        if (TakeTreeDerivative(tree, &diff_tree, var_index) != SUCCESS)
            return ERROR;

        if (GenLatex(&diff_tree, vars, fn) != SUCCESS)
            return ERROR;

        TreeRootDtor(tree);

        if (TreeCopy(&diff_tree, tree) != SUCCESS)
            return ERROR;

        TreeRootDtor(&diff_tree);
    }

    return SUCCESS;
}

int GenLatex(TreeStruct *tree, Vector *vars, FILE *fn) {

    assert(tree);
    assert(vars);
    assert(fn);

    if (TreeVerify(tree) != SUCCESS)
        return ERROR;

    if (VarsVerify(vars) != SUCCESS)
        return ERROR;

    fprintf(fn, "$$");

    if (PrintNodeLatex(tree->root, vars, fn) != SUCCESS) {
        printf(RED "Error while latex file creating" END_OF_COLOR "\n");
        return ERROR;
    }

    fprintf(fn, "$$");

    return SUCCESS;
}

static int PrintNodeLatex(TreeNode *node, Vector *vars, FILE *fn) {

    assert(vars);
    assert(fn);

    if (!node) {
        fprintf(fn, " ");
        return SUCCESS;
    }

    switch (node->value.type) {
        case (NUMBER): {
            Print(L);
            fprintf(fn, "%lg", node->value.number);
            Print(R);
            break;
        }
        case (VARIABLE): {
            Print(L);
            fprintf(fn, "%s", vars->data[node->value.nvar].name);
            Print(R);
            break;
        }
        case (OPERATION): {
            if (OperationVerify(node) != SUCCESS)
                return ERROR;
            switch (node->value.operation) {
                case (ADD): {
                    Print(L);
                    fprintf(fn, "+");
                    Print(R);
                    break;
                }
                case (SUB): {
                    Print(L);
                    fprintf(fn, "-");
                    Print(R);
                    break;
                }
                case (MUL): {
                    Print(L);
                    fprintf(fn, "*");
                    Print(R);
                    break;
                }
                case (DIV): {
                    fprintf(fn, "\\frac{");
                    Print(L);
                    fprintf(fn, "}{");
                    Print(R);
                    fprintf(fn, "}");
                    break;
                }
                case (SIN): {
                    Print(L);
                    fprintf(fn, "\\sin(");
                    Print(R);
                    fprintf(fn, ")");
                    break;
                }
                case (COS): {
                    Print(L);
                    fprintf(fn, "\\cos(");
                    Print(R);
                    fprintf(fn, ")");
                    break;
                }
                case (LN): {
                    Print(L);
                    fprintf(fn, "\\ln(");
                    Print(R);
                    fprintf(fn, ")");
                    break;
                }
                case (POW): {
                    if (node->left->value.type == NUMBER || node->left->value.type == VARIABLE) {
                        Print(L);
                    }
                    else {
                        fprintf(fn, "(");
                        Print(L);
                        fprintf(fn, ")");
                    }
                    fprintf(fn, "^{");
                    Print(R);
                    fprintf(fn, "}");
                    break;
                }
                case (SQRT): {
                    Print(L);
                    fprintf(fn, "\\sqrt{");
                    Print(R);
                    fprintf(fn, "}");
                    break;
                }
                default: {
                    printf(RED "Incorrect operation (latex)" END_OF_COLOR "\n");
                    return ERROR;
                }
            }
            break;
        }
        default: {
            printf(RED "Incorrect value type (latex)" END_OF_COLOR "\n");
            break;
        }
    }

    return SUCCESS;
}

static size_t GetVarIndex(Vector *vars) {

    size_t var_index = 0;

    printf(MAGENTA "Enter the index of the variable by which you want to differentiate: " END_OF_COLOR);
    for (size_t i = 0; i < vars->size; i++)
        printf("%lu) %s, ", i, vars->data[i].name);
    printf("\b: ");

    bool correct = true;
    do {
        correct = true;
        if (scanf("%lu", &var_index) != 1) {
            printf(RED "Incorrect input. Try again: " END_OF_COLOR);
            buf_clear();
            correct = false;
        }
        if (var_index < 0 || vars->size <= var_index) {
            printf(RED "Variable with index %lu doesn't exist. Try again: " END_OF_COLOR, var_index);
            correct = false;
        }
    } while (!correct);

    return var_index;
}

int GenGraphDots(TreeStruct *tree, Vector *vars, const char *filename) {

    assert(tree);
    assert(vars);
    assert(filename);

    if (TreeVerify(tree) != SUCCESS)
        return ERROR;

    if (VarsVerify(vars) != SUCCESS)
        return  ERROR;

    if (vars->size > 1) {
        printf(RED "The function should depend on no more than one variable" END_OF_COLOR "\n");
        return ERROR;
    }

    double answer = 0;
    bool error = 0;
    FILE *fn = fileopen(filename, "w");

    if (vars->size == 0) {
        answer = CalculateNode(tree->root, vars, &error);
        if (error)
            return ERROR;
    }

    if (vars->size == 1) {
        for (int i = 0; i < 21; i++) {
            vars->data[0].value = (double) i;
            answer = CalculateNode(tree->root, vars, &error);
            if (error)
                return ERROR;

            fprintf(fn, "%lg %lg\n", vars->data[0].value, answer);
        }
    }

    fileclose(fn);

    return SUCCESS;
}