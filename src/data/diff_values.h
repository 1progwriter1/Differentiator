#ifndef DIFF_VALUES
#define DIFF_VALUES

#define GRAPHVIZ_OUTPUT_FILE "Graphviz/diff.png"
#define GRAPH_FILE "Graphviz/diff_cpy.png"
#define GRAPHVIZ_INPUT_FILE "Graphviz/diff.dot"
#define DATA_DIFF "expression.txt"
#define PLOT_FILE "plot_file.txt"
#define LATEX_FILE "latex/latex.txt"
#define EXPR_FILE "long_expr.txt"

#define COL_SYM "5"

const size_t NUM_OF_VARS = 1;
const size_t MAX_VAR_NAME_LEN = 6;

struct MainArgs {
    bool graph;
    bool calculate;
    bool latex;
};

enum PoisonValue {
    NO_OPERATION,
    NO_NUMBER,
    NO_VAR,
};

enum ValueType {
    NUMBER    =  0,
    OPERATION =  1,
    VARIABLE  =  2,
};

#define DEF_OP(name, code, ...) name = code,

enum Operation {
    #include "operations.h"
};

#undef DEF_OP

struct NodeValue {
    enum ValueType type;
    union {
        enum Operation operation;
        double number;
        size_t nvar;
    };
    size_t subtree_size;
    char nick;
};

#endif