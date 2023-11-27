#include <stdio.h>
#include "../MyLibraries/headers/systemdata.h"
#include "differentiator.h"
#include "gen_graph_diff.h"
#include "file_read.h"
#include "derivative.h"

int main() {

    Differentiator data = {};
    if (DiffCtor(&data) != SUCCESS)
        return ERROR;

    if (ReadFileDiff(&data, DATA_DIFF) != SUCCESS)
        return ERROR;

    if (GenGraphDiff(&data.tree ,GRAPHVIZ_OUTPUT_FILE) != SUCCESS)
        return ERROR;

    Differentiator data_cpy = {};
    if (FindDerivative(&data, &data_cpy) != SUCCESS)
        return ERROR;

    GenGraphDiff(&data_cpy.tree, GRAPH_FILE);
    /* if (FindDerivative(&data, &data_cpy) != SUCCESS) {
        return ERROR;
    } */

    /* if (GenGraphDiff(&data_cpy.tree, GRAPHVIZ_OUTPUT_FILE) != SUCCESS)
        return ERROR; */

    /* if (Simplify(&data_cpy) != SUCCESS)
        return ERROR;

    if (GenGraphDiff(&data_cpy.tree, GRAPH_FILE) != SUCCESS)
        return ERROR; */

    /* double answer = 0;
    if (CalculateTree(&data, &answer) != SUCCESS)
        return ERROR;

    printf("Answer: %lg", answer); */

    DiffDtor(&data);
    DiffDtor(&data_cpy);

    return SUCCESS;
}