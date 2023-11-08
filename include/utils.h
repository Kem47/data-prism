#ifndef UTILS_H
#define UTILS H

#include <stdio.h>
#include <stdlib.h>
#include <zlib.h>
#include "logging.h"
#include "spec.h"


// these line length functions do not include the '\n' so if you use the values output from it and you need the line length including the '\n' then add 1.
int get_line_length(FILE *fp);
int gz_get_line_length(gzFile fp);
void comp_specs_inputs(Specs *specs, char (*ptr_rt_list)[3], uint8_t *ptr_rt_count);
int fflush_frequency(int line_len, int num_rt);

void close_output_files(Specs *specs);
void release_mem_fp(Specs *specs);

void free_column_info(ColumnInfo *col);
void free_output_file_info(OutputFileInfo *output_file);
void free_record_type_info(RecordTypeInfo *rt);
void release_mem_fp_rec(Specs *specs);

char* prompt_and_get_string(char* prompt);


#endif /* UTILS_H */