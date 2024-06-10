#include <stdio.h>
#include "file_read.h"
#include "../../../MyLibraries/headers/file_func.h"
#include "../differentiator/calculate.h"
#include <assert.h>
#include "../../../MyLibraries/headers/systemdata.h"
#include <string.h>
#include "../graph_gen/gen_graph_diff.h"
#include <string.h>
#include "../vector/my_vector.h"
#include "../bin_tree/bin_tree.h"
#include "../differentiator/derivative.h"

static int ReadNodeFile(TreeStruct *tree, TreeNode *node, Vector *vars, FileBuffer *buffer);
static int GetValue(TreeNode *node, Vector *vars, FileBuffer *buffer);
static int GetOperationNumber(TreeNode *node, FileBuffer *buffer);
static bool IsNewNode(FileBuffer *buffer);
static int EndNode(FileBuffer *buffer);
static int ReadVariable(TreeNode *node, FileBuffer *buffer, Vector *vars);

int ReadFileDiff(TreeStruct *tree, Vector *vars, const char *filename) {

    assert(tree);
    assert(vars);
    assert(filename);

    FileBuffer buffer = {};

    buffer.buf = readFileToBuffer(filename);
    if (!buffer.buf) {
        printf(RED "Buffer creation error" END_OF_COLOR "\n");
        return ERROR;
    }

    buffer.index = 0;

    if (IsNewNode(&buffer)) {
        if (!tree->root)
            return NO_MEMORY;
        if (ReadNodeFile(tree, tree->root, vars, &buffer) != SUCCESS) {
            printf(RED "Error when reading the tree" END_OF_COLOR "\n");
            return ERROR;
        }
    }

    free(buffer.buf);

    CountSubTreeSize(tree->root);

    return SUCCESS;
}

static int ReadNodeFile(TreeStruct *tree, TreeNode *node, Vector *vars, FileBuffer *buffer) {

    assert(node);
    assert(tree);
    assert(vars);
    assert(buffer);
    assert(buffer->buf);

    if (IsNewNode(buffer)) {
        node->left = TreeNodeNew(tree, {}, NULL, NULL);
        if (!node->left) return NO_MEMORY;
        if (ReadNodeFile(tree, node->left, vars, buffer) != SUCCESS)
            return ERROR;
    }

    if (GetValue(node, vars, buffer) != SUCCESS)
        return ERROR;

    if (IsNewNode(buffer)) {
        node->right = TreeNodeNew(tree, {}, NULL, NULL);
        if (!node->right) return NO_MEMORY;
        if (ReadNodeFile(tree, node->right, vars, buffer) != SUCCESS)
            return ERROR;
    }

    if (EndNode(buffer) != SUCCESS)
        return ERROR;

    return SUCCESS;
}

static int GetValue(TreeNode *node, Vector *vars, FileBuffer *buffer) {

    assert(node);
    assert(buffer);
    assert(buffer->buf);

    char *ptr = buffer->buf + buffer->index;

    int symbols_read = 0;
    double num = 0;
    if (sscanf(ptr, "%lf %n", &num, &symbols_read) == 1) {
        node->value = {NUMBER, {.number = num}};
    }
    else if (GetOperationNumber(node, buffer) == SUCCESS) {
        return SUCCESS;
    }
    else {
        if (ReadVariable(node, buffer, vars) != SUCCESS)
            return ERROR;
    }

    buffer->index += (size_t) symbols_read;

    return SUCCESS;
}

static int GetOperationNumber(TreeNode *node, FileBuffer *buffer) {

    assert(buffer);
    assert(buffer->buf);

    char *ptr = buffer->buf + buffer->index;

    #define DEF_OP(name, code, sym, ...)                                                \
        if (strncasecmp(ptr + i, sym, sizeof (sym) - 1) == 0) {                         \
            buffer->index += i + sizeof (sym) - 1;                                      \
            node->value = {OPERATION, {.operation = name}};                             \
            break;                                                                      \
        }

    for (size_t i = 0; ptr[i] != '\0'; i++) {

        if (ptr[i] == ' ')
            continue;

        #include "../data/operations.h"

        return ERROR;
    }

    #undef DEF_OP

    return SUCCESS;
}

static bool IsNewNode(FileBuffer *buffer) {

    assert(buffer);
    assert(buffer->buf);

    char *ptr = buffer->buf + buffer->index;

    for (size_t i = 0; ptr[i] != '\0'; i++) {
        if (ptr[i] == ' ')
            continue;
        if (ptr[i] == '(') {
            buffer->index += i + 1;
            return 1;
        }
        else if (ptr[i] == '_') {
            buffer->index += i + 1;
            return 0;
        }
        else {
            printf(RED "New node expected" END_OF_COLOR "\n");
            return 0;
        }
    }

    return 0;
}

static int EndNode(FileBuffer *buffer) {

    assert(buffer);
    assert(buffer->buf);

    char *ptr = buffer->buf + buffer->index;

    for (size_t i = 0; ptr[i] != '\0'; i++) {
        if (ptr[i] == ' ')
            continue;
        if (ptr[i] == ')') {
            buffer->index += i + 1;
            return SUCCESS;
        }
        else {
            printf(RED "End of node expected" END_OF_COLOR "\n");
            return ERROR;
        }
    }
    printf(RED "Incorrect syntax" END_OF_COLOR "\n");

    return ERROR;
}

static int ReadVariable(TreeNode *node, FileBuffer *buffer, Vector *vars) {

    assert(vars);
    assert(node);
    assert(buffer);
    assert(buffer->buf);

    int symbols_read = 0;
    char *str = (char *) calloc (MAX_VAR_NAME_LEN, sizeof (char));
    if (!str)
        return ERROR;

    char *ptr = buffer->buf + buffer->index;
    for (size_t i = 0; ptr[i] != '\0'; i++) {
        if (ptr[i] == ' ')
            continue;

        if (sscanf(ptr + i, "%" COL_SYM "[^ _] %n", str, &symbols_read) != 1)
            return ERROR;

        if (ptr[i + (size_t) symbols_read] != ' ' && ptr[i + (size_t) symbols_read] != '_') {
            printf(RED "Name of variable is too long" END_OF_COLOR "\n");
            return ERROR;
        }

        buffer->index += i + (size_t) symbols_read;
        break;
    }
    int var_index = GetIndexIfExist(vars, *str);

    if (var_index == -1) {
        if (PushBack(vars, {*str, 0}) != SUCCESS)
            return ERROR;
        node->value = {VARIABLE, {.nvar = vars->size - 1}};
    }
    else {
        node->value = {VARIABLE, {.nvar = (size_t) var_index}};
    }

    return SUCCESS;

}

int GetIndexIfExist(Vector *vars, char variable) {

    assert(vars);
    assert(variable);

    for (size_t i = 0; i < vars->size; i++) {
        if (vars->data[i].name == variable)
            return (int) i;
    }

    return -1;
}

int GetMainArgs(const int argc, const char *argv[], MainArgs *maindata) {

    assert(argv);
    assert(maindata);

    for (size_t i = 1; i < (size_t) argc; i++) {
        if (strcmp("--graph", argv[i]) == 0) {
            maindata->graph = true;
            continue;
        }
        if (strcmp("--calc", argv[i]) == 0) {
            maindata->calculate = true;
            continue;
        }
        if (strcmp("--latex", argv[i]) == 0) {
            maindata->latex = true;
            continue;
        }
        printf(RED "Incorrect args for main()" END_OF_COLOR "\n");
        return ERROR;
    }

    return SUCCESS;
}