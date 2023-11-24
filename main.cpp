#include <stdio.h>
#include "../MyLibraries/headers/systemdata.h"
#include "differentiator.h"
#include "gen_graph_diff.h"
#include "file_read.h"

int main() {

    Differentiator data = {};
    if (DiffCtor(&data) != SUCCESS)
        return ERROR;

    if (ReadFileDiff(&data, DATA_DIFF) != SUCCESS)
        return ERROR;

    /* if (FindDerivative(&data) != SUCCESS)
        return ERROR; */

    GenGraphDiff(&data.tree);

    double answer = 0;
    CalculateTree(&data, &answer);
    printf("Answer: %lg", answer);

    TreeRootDtorDiff(&data.tree);

    return SUCCESS;
}