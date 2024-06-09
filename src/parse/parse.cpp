#include "parse.h"
#include <assert.h>
#include <math.h>
#include "../MyLibraries/headers/systemdata.h"
#include <string.h>
#include "../MyLibraries/headers/file_func.h"
#include "diff_dsl.h"
#include "file_read.h"

#define ERR_ASSERT(error) if (error != NO_ERROR) return NULL;
#define PAR_ASSERT(str)   if (str != ')') { printf(RED "Syntax error: " END_OF_COLOR "\nrecieved %s\nexpexted: )\n", data->buf + data->position);   \
                                            data->error = CL_PARENTHESIS_MISSED;                                                                    \
                                            return NULL;}                                                                                           \

int StringParse(TreeStruct *tree, Vector *vars, const char *filename) {

    assert(filename);

    char *buf = readbuf(filename);
    if (!buf) return NO_MEMORY;

    StringParseData data = {tree, vars, buf, 0, NO_ERROR};
    TreeRootDtor(tree);

    size_t start_position = data.position;

    tree->root = GetExpression(&data);

    if (data.buf[data.position] != '\0') {
        printf(RED "Syntax error: " END_OF_COLOR "\nrecieved: %s\nexpected: \\0\n", data.buf + data.position);
        return ERROR;
    }

    if (data.error != NO_ERROR)
        return ERROR;

    if (start_position == data.position) {
        data.error = EMPTY_STRING;
        printf(MAGENTA "warning: " END_OF_COLOR "empty expression\n");
    }

    return SUCCESS;
}

TreeNode *GetExpression(StringParseData *data) {

    assert(data);
    assert(data->tree);
    assert(data->buf);

    ERR_ASSERT(data->error);

    TreeStruct *tree = data->tree;

    TreeNode *ptr_fst = GetTerm(data);
    if (!ptr_fst) return NULL;

    while (data->buf[data->position] == '+' || data->buf[data->position] == '-') {
        char oper = data->buf[data->position];
        data->position += 1;

        TreeNode *ptr_snd = GetTerm(data);
        if (!ptr_snd) return NULL;

        switch (oper) {
            case ('+'): {
                return NEW_S(OP(ADD), ptr_fst, ptr_snd);
            }
            case ('-'): {
                return NEW_S(OP(ADD), ptr_fst, ptr_snd);
            }
            default: {
                printf(RED "Syntax error: " END_OF_COLOR "\nrecieved: %s\nexpected: +, -\n", data->buf + data->position);
                return NULL;
            }
        }
    }
    return ptr_fst;
}

TreeNode *GetTerm(StringParseData *data) {

    assert(data);
    assert(data->buf);
    assert(data->tree);

    ERR_ASSERT(data->error);

    TreeStruct *tree = data->tree;

    TreeNode *ptr_fst = GetPrimaryExpression(data);
    if (!ptr_fst) return NULL;

    ERR_ASSERT(data->error);

    while (data->buf[data->position] == '*' || data->buf[data->position] == '/' || data->buf[data->position] == '^') {

        char oper = data->buf[data->position];
        data->position += 1;

        TreeNode *ptr_snd = GetPrimaryExpression(data);
        switch (oper) {
            case ('*'): {
                return NEW_S(OP(MUL), ptr_fst, ptr_snd);
            }
            case ('/'): {
                return NEW_S(OP(DIV), ptr_fst, ptr_snd);
            }
            case ('^'): {
                return NEW_S(OP(POW), ptr_fst, ptr_snd);
            }
            default: {
                printf(RED "Syntax error: " END_OF_COLOR "\nrecieved: %s\nexpected: *, /, ^\n", data->buf + data->position);
                return NULL;
            }
        }
    }
    return ptr_fst;
}

TreeNode *GetPrimaryExpression(StringParseData *data) {

    assert(data);
    assert(data->buf);
    assert(data->tree);

    ERR_ASSERT(data->error);

    if (data->buf[data->position] == '(') {

        data->position += 1;
        TreeNode *ptr = GetExpression(data);
        if (!ptr) return NULL;

        PAR_ASSERT(data->buf[data->position])
        data->position += 1;

        return ptr;
    }
    else {
        return GetUnary(data);
    }
}

TreeNode *GetUnary(StringParseData *data) {

    assert(data);
    assert(data->buf);
    assert(data->tree);

    ERR_ASSERT(data->error);

    TreeStruct *tree = data->tree;

    TreeNode *ptr = NULL;

    if (strncmp(data->buf + data->position, "cos(", sizeof ("cos(") - 1) == 0) {
        data->position += sizeof ("cos(") - 1;
        ptr = NEW_S(OP(COS), NULL, GetExpression(data));

        PAR_ASSERT(data->buf[data->position])
        data->position += 1;

        return ptr;
    }
    if (strncmp(data->buf + data->position, "sin(", sizeof ("sin(") - 1) == 0) {
        data->position += sizeof ("sin(") - 1;

        ptr = NEW_S(OP(SIN), NULL, GetExpression(data));

        PAR_ASSERT(data->buf[data->position])
        data->position += 1;

        return ptr;
    }
    if (strncmp(data->buf + data->position, "ln(", sizeof ("ln(") - 1) == 0) {
        data->position += sizeof ("ln(") - 1;

        ptr = NEW_S(OP(LN), NULL, GetExpression(data));

        PAR_ASSERT(data->buf[data->position])
        data->position += 1;

        return ptr;
    }

    return GetNumber(data);
}

TreeNode *GetNumber(StringParseData *data) {

    assert(data);
    assert(data->buf);

    ERR_ASSERT(data->error);

    TreeStruct *tree = data->tree;

    double val = 0;
    size_t old_pos = data->position;

    while ('0' <= data->buf[data->position] && data->buf[data->position] <= '9') {
        val = val * 10 + data->buf[data->position] - '0';
        data->position += 1;
    }
    if (data->buf[data->position] == '.') {
        int ten_pow = 0;
        data->position += 1;
        while ('0' <= data->buf[data->position] && data->buf[data->position] <= '9') {
            ten_pow++;
            val = val * 10 + data->buf[data->position] - '0';
            data->position += 1;
        }
        val /= pow(10, ten_pow);
    }

    if ('x' <= data->buf[data->position] && data->buf[data->position] <= 'z') {

        int var_index = GetIndexIfExist(data->vars, data->buf[data->position]);
        if (var_index == -1) {
            if (PushBack(data->vars, {data->buf[data->position], 0}) != SUCCESS)
                return NULL;
            data->position += 1;
            return NEW(VAR(data->vars->size - 1), NULL, NULL);
        }
        else {
            data->position += 1;
            return NEW(VAR((size_t) var_index), NULL, NULL);
        }

    }

    if (data->position == old_pos) {
        data->error = MISSING_NUMBER;
        printf(RED "Syntax error: " END_OF_COLOR "\nrecieved: %s\nexpected: 0 - 9\n", data->buf + data->position);
        return NULL;
    }

    return NEW(NUM((double) val), NULL, NULL);
}