#ifndef DIFF_VALUES
#define DIFF_VALUES

#define GRAPHVIZ_OUTPUT_FILE "Graphviz/diff.png"
#define GRAPH_FILE "Graphviz/diff_cpy.png"
#define GRAPHVIZ_INPUT_FILE "Graphviz/diff.dot"
#define DATA_DIFF "diff_data.txt"

const size_t NUM_OF_VARIABLES = 1; //??
const char variables[NUM_OF_VARIABLES][2] = {"x"}; //?

enum ValueType {
    NO_TYPE   = -2,
    NO_NUMBER = -1,
    NUMBER    =  0,
    OPERATION =  1,
    VARIABLE  =  2,
};

enum Variable {
    NO_VAR = -1,
    VAR_X  =  0,
};

#define DEF_OP(name, code, ...) name = code,

enum Operation {
    NO_OPERATION = -1,

    #include "operations.h"

};

#undef DEF_OP

struct NodeValue {
    enum ValueType type;
    union {
        enum Operation operation;
        double number;
        double *vars;
    };
};
// internal linkage
//const char array[][];
//
//const char* array[];

#endif