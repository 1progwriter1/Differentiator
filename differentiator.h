#ifndef DIFFERENTIATOR
#define DIFFERENTIATOR

#include "bin_tree.h"
#include "value_buffer.h"

struct Differentiator {
    ValueBuffer buf;
    TreeStruct tree;
    double var;
};

int DiffCtor(Differentiator *data);

int DiffDtor(Differentiator *data);

Buf_t DiffPushValue(Differentiator *data, Buf_t value);

NodeValue *CreateNodeValue(ValueType type, Operation operation, double num);

int CalculateTree(Differentiator *data, double *answer);

double GetVarialble();

int FindDerivative(Differentiator *data);

#endif