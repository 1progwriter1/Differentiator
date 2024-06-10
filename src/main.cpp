#include <stdio.h>
#include "../../MyLibraries/headers/systemdata.h"
#include "differentiator/calculate.h"
#include "graph_gen/gen_graph_diff.h"
#include "read_file/file_read.h"
#include "differentiator/derivative.h"
#include "vector/my_vector.h"
#include "gen_latex/tree_output.h"
#include "parse/parse.h"

int main(const int argc, const char *argv[]) {

    MainArgs maindata = {false, false, false};
    if (GetMainArgs(argc, argv, &maindata) != SUCCESS)
        return ERROR;

    TreeStruct tree = {};
    Vector vars = {};

    if (TreeRootCtor(&tree) != SUCCESS)
        return ERROR;

    if (VectorCtor(&vars, NUM_OF_VARS) != SUCCESS)
        return ERROR;

    if (StringParse(&tree, &vars, DATA_DIFF) != SUCCESS)
        return ERROR;

    if (maindata.graph)
        if (GenGraphDots(&tree, &vars, PLOT_FILE) != SUCCESS) {
            printf(RED "Dots generation failed" END_OF_COLOR "\n");
            return ERROR;
        }

    if (maindata.latex) {
        if (GenGraphDiff(&tree, &vars, GRAPHVIZ_OUTPUT_FILE) != SUCCESS)
            return ERROR;
        if (PrintDifferentiation(&tree, &vars, LATEX_FILE) != SUCCESS) {
            printf(RED "latex error" END_OF_COLOR "\n");
            return ERROR;
        }
    }

    if (maindata.calculate) {
        double answer = 0;
        if (CalculateTree(&tree, &vars, &answer) != SUCCESS)
            return ERROR;
        printf(BRIGHT_GREEN "Answer: " END_OF_COLOR "%lg\n", answer);
    }

    TreeRootDtor(&tree);
    VectorDtor(&vars);

    return SUCCESS;
}