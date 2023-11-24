#ifndef DIFF_VALUES
#define DIFF_VALUES

#define GRAPHVIZ_OUTPUT_FILE "Graphviz/diff.png"
#define GRAPHVIZ_INPUT_FILE "Graphviz/diff.dot"
#define DATA_DIFF "diff_data.txt"

const size_t NUM_OF_VARIABLES = 1;
const char variables[NUM_OF_VARIABLES][2] = {"x"};

enum ValueType {
    NO_NUMBER = 0,
    NUMBER    = 1,
    OPERATION = 2,
    VARIABLE  = 3,
};

enum Variable {
    NO_VAR = -1,
    VAR_X  =  0,
};

enum Operation {
    NO_OPERATION = 0,
    ADD          = 1,
    SUB          = 2,
    DIV          = 3,
    MUL          = 4,
    SQRT         = 5,
    SIN          = 6,
    COS          = 7,
    LN           = 8,
};

struct Value {
    enum Operation operation;
    double number;
} ;


struct NodeValue {
    enum ValueType type;
    Value value;
};


#endif