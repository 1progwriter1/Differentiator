#include <stdio.h>
#include "file_read.h"
#include "../MyLibraries/headers/file_func.h"
#include "differentiator.h"
#include <assert.h>
#include "../MyLibraries/headers/systemdata.h"
#include <string.h>
#include "gen_graph_diff.h"
#include <string.h>

static int ReadNodeFile(Differentiator *data, TreeNode *node, FileBuffer *buffer);
static NodeValue *GetValue(Differentiator *data, FileBuffer *buffer);
static Operation GetOperationNumber(FileBuffer *buffer);
static bool IsNewNode(FileBuffer *buffer);
static int EndNode(FileBuffer *buffer);
static int ReadVariable(FileBuffer *buffer, Variable *var);

int ReadFileDiff(Differentiator *data, const char *filename) {

    assert(data);
    assert(filename);

    FileBuffer buffer = {};

    buffer.buf = readbuf(filename);
    if (!buffer.buf)
        return ERROR;

    buffer.index = 0;

    if (IsNewNode(&buffer)) {
        if (!data->tree.root)
            return NO_MEMORY;
        if (ReadNodeFile(data, data->tree.root, &buffer) != SUCCESS)
            return ERROR;
    }

    free(buffer.buf);

    return SUCCESS;
}

static int ReadNodeFile(Differentiator *data, TreeNode *node, FileBuffer *buffer) {

    assert(node);
    assert(data);
    assert(buffer);
    assert(buffer->buf);

    if (IsNewNode(buffer)) {
        node->left = TreeNodeNew(&data->tree, NULL);
        if (!node->left)
            return NO_MEMORY;
        if (ReadNodeFile(data, node->left, buffer) != SUCCESS)
            return ERROR;
    }

    node->value = GetValue(data, buffer);
    if (!node->value)
        return ERROR;

    if (IsNewNode(buffer)) {
        node->right = TreeNodeNew(&data->tree, NULL);
        if (!node->right)
            return NO_MEMORY;
        if (ReadNodeFile(data, node->right, buffer) != SUCCESS)
            return ERROR;
    }

    if (EndNode(buffer) != SUCCESS)
        return ERROR;

    return SUCCESS;
}

static NodeValue *GetValue(Differentiator *data, FileBuffer *buffer) {

    assert(data);
    assert(buffer);
    assert(buffer->buf);

    char *ptr = buffer->buf + buffer->index;

    NodeValue *node_ptr = NULL;
    int symbols_read = 0;
    double num = 0;
    Variable var = NO_VAR;
    if (sscanf(ptr, "%lf %n", &num, &symbols_read) == 1) {
        node_ptr = CreateNodeValue(NUMBER, NO_OPERATION, num);
        if (!node_ptr)
            return NULL;
    }
    else if (ReadVariable(buffer, &var) == SUCCESS) {
        node_ptr = CreateNodeValue(VARIABLE, NO_OPERATION, var);
        if (!node_ptr)
                return NULL;
    }
    else {
        Operation operation = GetOperationNumber(buffer);
        if (operation == NO_OPERATION)
            return NULL;
        node_ptr = CreateNodeValue(OPERATION, operation, NO_NUMBER);
    }

    buffer->index += (size_t) symbols_read;

    return node_ptr;
}

static Operation GetOperationNumber(FileBuffer *buffer) {

    assert(buffer);
    assert(buffer->buf);

    char *ptr = buffer->buf + buffer->index;

    for (size_t i = 0; ptr[i] != '\0'; i++) {

        if (ptr[i] == ' ')
            continue;

        if (ptr[i + 1] == ' ') {
            buffer->index += i + 1;
            switch (ptr[i]) {
                case ('+'): {
                    return ADD;
                }
                case ('-'): {
                    return SUB;
                }
                case ('*'): {
                    return MUL;
                }
                case ('/'): {
                    return DIV;
                }
                default: {
                    printf(RED "Invalid symbol" END_OF_COLOR "\n");
                    return NO_OPERATION;
                }
            }
        }
        else {
            if (strncasecmp("cos", ptr + i, 3) == 0) {
                buffer->index += i + 3;
                return COS;
            }
            if (strncasecmp("sin", ptr + i, 3) == 0) {
                buffer->index += i + 3;
                return SIN;
            }
            if (strncasecmp("sqrt", ptr + i, 4) == 0) {
                buffer->index += i + 4;
                return SQRT;
            }
            else {
                printf(RED "Invalid symbols" END_OF_COLOR "\n");
                return NO_OPERATION;
            }
        }
    }

    return NO_OPERATION;
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

static int ReadVariable(FileBuffer *buffer, Variable *var) {

    assert(var);
    assert(buffer);
    assert(buffer->buf);

    char *ptr = buffer->buf + buffer->index;
    for (size_t i = 0; ptr[i] != '\0'; i++) {
        if (ptr[i] == ' ')
            continue;
        if (ptr[i] != 'x') {
            return ERROR;
        }
        else {
            buffer->index += i + 1;
            *var = VAR_X;
            return SUCCESS;
        }
    }

    return ERROR;

}