#ifndef DIFFERENTIATOR_DSL
#define DIFFERENTIATOR_DSL

#define CALC(node) CalculateNode(node, vars, error)

#define OP(oper)   {OPERATION, {.operation = oper}, 0}

#define NUM(num)   {NUMBER, {.number = num}, 0}

#define VAR(ind)   {VARIABLE, {.nvar = ind}, 0}

#define NEW(val, left, right)   TreeNodeNew(tree, val ,left, right)

#define NEW_S(val, left, right) TreeNodeNewSafe(tree, val ,left, right)

#define D(node) TakeNodeDerivative(tree, node, var_index)

#define C(node)  NodeCopy(tree, node)

#define L     node->left

#define R     node->right

#define LEFT  node->left

#define RIGHT node->right

#define IsVar(node)   node->value.type == VARIABLE

#define IsNum(node)   node->value.type == NUMBER

#define IsOp(node)    node->value.type == OPERATION

#define IsZero(node)  ( IsNum(node) && IsEqual(node->value.number, 0) )

#define IsOne(node)   ( IsNum(node) && IsEqual(node->value.number, 1) )

#define EQ(node, num) ( IsNum(node) && IsEqual(node->value.number, num) )

#define Print(node)   if (PrintNodeLatex(node, vars, fn) != SUCCESS) return ERROR

#define PrintSimple  if (PrintSimplifying(node, ptr, data->vars, data->fn) != SUCCESS)  return NULL;

#endif