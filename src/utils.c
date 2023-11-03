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
    while (next != NULL)
    {
        for (int i = 0; i < *ptr_rt_count; i++)
        {
            if (strcmp(next->record_type, ptr_rt_list[i]) != 0)
            {
                printf("WARNING: your spec file did not have specs for record type %s\n", next->record_type);
            }
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

void release_specs_mem(Specs *specs)
{
    // 
    
}