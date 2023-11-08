#ifndef SPEC_H
#define SPEC_H

#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "configs.h"
#include "logging.h"


typedef struct ColumnInfo
{
    char *name;
    int size;
    int ref_count;
    struct ColumnInfo *next;
} ColumnInfo;

typedef struct OutputFileInfo
{
    FILE *fp;
    int file_length;
    char *file_name;
    char *file_type;
    struct OutputFileInfo *previous_output_file;
} OutputFileInfo;

typedef struct RecordTypeInfo
{
    char *record_type;
    int num_columns;
    ColumnInfo *first_column;
    struct RecordTypeInfo *next;
    int num_files;
    struct OutputFileInfo *current_output_file;
} RecordTypeInfo;

typedef struct Specs
{
    int num_rt;
    RecordTypeInfo *first_rt;
} Specs;


Specs get_specs(FILE *fp_spec, char (*ptr_rt_list)[3], uint8_t *ptr_rt_count);

void trim_whitespace(char *str);

void print_specs(Specs *specs);


#endif /* SPEC_H */