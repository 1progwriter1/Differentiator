#include <stdio.h>
#include "../MyLibraries/headers/file_func.h"
#include "../MyLibraries/headers/systemdata.h"
#include "gen_graph_diff.h"
#include "differentiator.h"
#include <assert.h>
#include "../MyLibraries/headers/systemdata.h"

static int GenDiffNodes(FILE *fn, const TreeNode *node, size_t *index);
static int PrintNodeTree(FILE *fn, const TreeNode *node, const size_t index);

int GenGraphDiff(const TreeStruct *tree) {

    assert(tree);

    FILE *fn = fileopen(GRAPHVIZ_INPUT_FILE, WRITE);
    if (!fn)
        return FILE_OPEN_ERROR;

    size_t index = 0;
    fprintf(fn, "digraph G {\n\t");
    fprintf(fn, "%d [shape = \"polygon\", label = \"size: %lu\\naddress: %p\", style = \"filled\", color = \"#FFFF99\"]\n\t-10->0 [weight = 1000, color = \"#FFFFFF\"]\n\t", -10, tree->size, tree->root);

    if (!tree->root)
        return NULL_POINTER;

    GenDiffNodes(fn, tree->root, &index);

    fprintf(fn, "\n");
    fprintf(fn, "}");

    fileclose(fn);

    system("dot " GRAPHVIZ_INPUT_FILE " -T png -o " GRAPHVIZ_OUTPUT_FILE);

    return SUCCESS;
}

static int GenDiffNodes(FILE *fn, const TreeNode *node, size_t *index) {

    assert(fn);
    assert(node);
    assert(index);

    PrintNodeTree(fn, node, *index);
    size_t main_index = *index;

    if (node->left) {
        *index += 1;
        fprintf(fn, "%lu->%lu\n\t", main_index, *index);
        GenDiffNodes(fn, node->left, index);
    }
    if (node->right) {
        *index += 1;
        fprintf(fn, "%lu->%lu\n\t", main_index, *index);
        GenDiffNodes(fn, node->right, index);
    }

    return SUCCESS;
}

static int PrintNodeTree(FILE *fn, const TreeNode *node, const size_t index) {

    assert(fn);
    assert(node);

    fprintf(fn, "%lu [shape = Mrecord, style = filled, fillcolor = \"", index);

    if (!node->value) {
        fprintf(fn, "#FF0000\", color = \"#331900\", label = \"NULL\"]\n\t");
        return SUCCESS;
    }

    switch (node->value->type) {
        case (OPERATION): {
            fprintf(fn, "#00FFFF\", color = \"#331900\", label = \"");
            switch (node->value->value.operation) {
                case (ADD): {
                    fprintf(fn, "+");
                    break;
                }
                case (SUB): {
                    fprintf(fn, "-");
                    break;
                }
                case (DIV): {
                    fprintf(fn, "/");
                    break;
                }
                case (MUL): {
                    fprintf(fn, "*");
                    break;
                }
                case (COS): {
                    fprintf(fn, "cos");
                    break;
                }
                case (SIN): {
                    fprintf(fn, "sin");
                    break;
                }
                case (SQRT): {
                    fprintf(fn, "sqrt");
                    break;
                }
                case (LN): {
                    fprintf(fn, "ln");
                }
                case (NO_OPERATION): {
                    break;
                }
                default: {
                    printf(RED "Incorrect operation number" END_OF_COLOR "\n");
                    return ERROR;
                }
            }
            break;
        }
        case (NUMBER): {
            fprintf(fn, "#66FF66\", color = \"#331900\", label = \"%lg", node->value->value.number);
            break;
        }
        case (VARIABLE): {
            fprintf(fn, "#66FF66\", color = \"#331900\", label = \"x");
        }
        case (NO_NUMBER): {
            break;
        }
        default: {
            printf(RED "Incoreect value type" END_OF_COLOR "\n");
            return ERROR;
        }
    }

    fprintf(fn, "\"]\n\t");

    return SUCCESS;
}