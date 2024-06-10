#ifndef DIFFERENTIATOR
#define DIFFERENTIATOR

#include "../bin_tree/bin_tree.h"

struct Differentiator {
    TreeStruct tree;
    double *var;
    bool is_get;
}; //???

//variables

// --`. function(variables, tree) --> evaluate
int DiffCtor(Differentiator *data);

int DiffDtor(Differentiator *data);

NodeValue *CreateNodeValue(ValueType type, Operation operation, double num, int nvar);

int CalculateTree(Differentiator *data, double *answer);

double GetVarialble();

int TreeCopy(Differentiator *data_src, Differentiator *data_dst);

TreeNode *NodeCopy(Differentiator *data, TreeNode *src);

double CalculateNode(Differentiator *data, TreeNode *node, bool *error);

#endif