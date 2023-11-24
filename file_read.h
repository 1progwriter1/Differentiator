#ifndef FILE_READ_DIFF
#define FILE_READ_DIFF

#include "differentiator.h"

struct FileBuffer {
    char *buf;
    size_t index;
};

int ReadFileDiff(Differentiator *data, const char *filename);

#endif