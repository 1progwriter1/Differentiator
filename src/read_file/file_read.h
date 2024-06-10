#ifndef FILE_READ_DIFF
#define FILE_READ_DIFF

#include "../vector/my_vector.h"
#include "../bin_tree/bin_tree.h"

struct FileBuffer {
    char *buf;
    size_t index;
};

int ReadFileDiff(TreeStruct *tree, Vector *vars, const char *filename);

int GetMainArgs(const int argc, const char *argv[], MainArgs *maindata);

int GetIndexIfExist(Vector *vars, const char *variable);

#endif