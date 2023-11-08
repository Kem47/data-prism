#include "utils.h"


int get_line_length(FILE *fp)
{
    int length = 0;    
    int c;
    while ((c = fgetc(fp)) != EOF && c != '\n')
    {
        length++;
    }
    return length;
}

int gz_get_line_length(gzFile fp)
{
    LOG("%s started\n", __func__);
    int length = 0;
    int c;
    while ((c = gzgetc(fp)) != EOF && c != '\n')
    {
        length++;
    }
    return length;
}

void comp_specs_inputs(Specs *specs, char (*ptr_rt_list)[3], uint8_t *ptr_rt_count)
{
    RecordTypeInfo *next = specs->first_rt;
    bool found;
    while (next != NULL)
    {
        found = false;
        
        for (int i = 0; i < *ptr_rt_count; i++)
        {
            LOG("%s: rt in spec is %s and rt in input list is %s\n", __func__, next->record_type, ptr_rt_list[i]);
            if (strcmp(next->record_type, ptr_rt_list[i]) == 0)
            {
                found = true;
            }
        }

        if (!found)
        {
            printf("WARNING: your spec file did not have specs for record type %s\n", next->record_type);
        }

        next = next->next;
    }
}



int fflush_frequency(int line_len, int num_rt)
{
    // create an algorithm that determines how frequently to flush based on how large the lines are and how many output files there are
    // have some kind of a global buffer/memory limit that will determine
    // flush_frequency * line_length * num_output_files = GLOBAL_LIMIT

    // issue with using num_rt is that the number of record types in specs might not be the number of record types in the actual data file and thus not the right number of output files.
    // however at the point of calculations - before the cutting begings - I cannot know how many record types will be present in the data file.
    return MEMORY_LIMIT / line_len / num_rt;  // could do some floats and rounding but I don't think +-1 line will matter
}


void close_output_files(Specs *specs)
{
    //
    RecordTypeInfo *next = specs->first_rt;
    while (next != NULL)
    {
        fclose(next->current_output_file->fp);
    }
}


// RELEASE MEMORY

// ORIGINAL
void release_mem_fp(Specs *specs)
{
    // 
    RecordTypeInfo *rt = specs->first_rt;
    while (rt != NULL)
    {
        ColumnInfo *col = rt->first_column;
        while (col != NULL)
        {
            ColumnInfo *next_col = col->next;
            free(col->name);
            free(col);
            col = next_col;
        }

        OutputFileInfo *output_file = rt->current_output_file;
        while (output_file != NULL)
        {
            OutputFileInfo *next_output_file = output_file->previous_output_file;
            fclose(output_file->fp);
            free(output_file->file_name);
            free(output_file->file_type);
            free(output_file);
            output_file = next_output_file;
        }

        RecordTypeInfo *next_rt = rt->next;
        free(rt->record_type);
        free(rt);
        rt = next_rt;
    }

    free(specs);
}

// WITH RECURSION
void free_column_info(ColumnInfo *col)
{
    if (col == NULL) 
    {
        return;
    }

    free_column_info(col->next);
    if (col->name != NULL)
    {
        free(col->name);
        col->name = NULL;
    }
    if (col != NULL)
    {
        free(col);
        col = NULL;
    }
}

void free_output_file_info(OutputFileInfo *output_file)
{
    if (output_file == NULL) 
    {
        return;
    }

    free_output_file_info(output_file->previous_output_file);
    if (output_file->fp != NULL)
    {
        fclose(output_file->fp);
        output_file->fp = NULL;
    }
    if (output_file->file_name != NULL)
    {
        free(output_file->file_name);
        output_file->file_name = NULL;
    }
    if (output_file->file_type != NULL)
    {
        free(output_file->file_type);
        output_file->file_type = NULL;
    }
    if (output_file != NULL)
    {
        free(output_file);
        output_file = NULL;
    }
}

void free_record_type_info(RecordTypeInfo *rt)
{
    if (rt == NULL) 
    {
        return;
    }

    free_record_type_info(rt->next);
    if (rt->first_column->ref_count == 1)
    {
        free_column_info(rt->first_column);
    }
    else
    {
        rt->first_column->ref_count--;
    }
    free_output_file_info(rt->current_output_file);
    if (rt->record_type != NULL)
    {
        free(rt->record_type);
        rt->record_type = NULL;
    }
    if (rt != NULL)
    {
        free(rt);
        rt = NULL;
    }
}

void release_mem_fp_rec(Specs *specs)
{
    free_record_type_info(specs->first_rt);
}


// My get string
char* prompt_and_get_string(char* prompt) 
{
    printf("%s", prompt);
    char* line = NULL;
    size_t bufsize = 0;
    getline(&line, &bufsize, stdin);
    return line;
}



