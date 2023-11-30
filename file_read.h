#ifndef FILE_READ_DIFF
#define FILE_READ_DIFF

#include "bin_tree.h"
#include "my_vector.h"

struct FileBuffer {
    char *buf;
    size_t index;
};

int ReadFileDiff(TreeStruct *tree, Vector *vars, const char *filename);

int GetMainArgs(const int argc, const char *argv[], MainArgs *maindata);

#endif