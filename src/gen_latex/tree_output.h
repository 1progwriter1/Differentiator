#ifndef GEN_LATEX_DIFF
#define GEN_LATEX_DIFF

#include "../bin_tree/bin_tree.h"

int GenLatex(TreeStruct *tree, Vector *vars, FILE *fn);

int PrintDifferentiation(TreeStruct *tree, Vector *vars, const char *filename);

int GenGraphDots(TreeStruct *tree, Vector *vars, const char *filename);

#endif