#ifndef UTILS_H
#define UTILS H

#include <stdio.h>
#include <stdlib.h>
#include <zlib.h>
#include "logging.h"

int get_line_length(FILE *fp);
int gz_get_line_length(gzFile fp);

#endif /* UTILS_H */