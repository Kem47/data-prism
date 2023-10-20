#ifndef PARSE_H
#define PARSE_H

#include <stdio.h>
#include <stdlib.h>
#include <zlib.h>
#include "spec.h"
#include "configs.h"
#include "logging.h"
#include "utils.h"

void gz_parser(Specs specs, gzFile fp_gz);

#endif /* PRARSE_H */