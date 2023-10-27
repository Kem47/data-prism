#ifndef UTILS_H
#define UTILS H

#include <stdio.h>
#include <stdlib.h>
#include <zlib.h>
#include "logging.h"


// these line length functions do not include the '\n' so if you use the values output from it and you need the line length including the '\n' then add 1.
int get_line_length(FILE *fp);
int gz_get_line_length(gzFile fp);

#endif /* UTILS_H */