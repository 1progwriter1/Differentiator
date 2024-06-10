#ifndef STRING_PARSE
#define STRING_PARSE

#include <stdio.h>
#include "../bin_tree/bin_tree.h"

#define INPUT_FILE "expr.txt"

enum ParseError {
    NO_ERROR,
    INCORRECT_ENDING,
    CL_PARENTHESIS_MISSED,
    INCORRECT_OPERATION,
    EMPTY_STRING,
    MISSING_NUMBER,
};

struct StringParseData {
    TreeStruct *tree;
    Vector *vars;
    const char *buf;
    size_t position;
    ParseError error;
};

int StringParse(TreeStruct *tree, Vector *vars, const char *filename);

TreeNode *GetExpression(StringParseData *data);

TreeNode *GetTerm(StringParseData *data);

TreeNode *GetPrimaryExpression(StringParseData *data);

TreeNode *GetNumber(StringParseData *data);

TreeNode *GetUnary(StringParseData *data);

#endif