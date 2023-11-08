#ifndef PARSE_H
#define PARSE_H

#include <stdio.h>
#include <stdlib.h>
#include <zlib.h>
#include "configs.h"
#include "logging.h"
#include "spec.h"
#include "utils.h"


void gz_parser(Specs specs, gzFile fp_gz, char *input_filename);

void open_new_output_file(RecordTypeInfo *rt_specs, char *input_filename);

void write_row(RecordTypeInfo *rt_specs, char *line, int ffreq);


#endif /* PRARSE_H */