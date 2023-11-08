#include "parse.h"

void gz_parser(Specs specs, gzFile fp_gz, char *input_filename) // I should probably pass in specs as a pointer??
{
    int rt_len = strlen(specs.first_rt->record_type);
    LOG("%s: rt_len is %i\n", __func__, rt_len);

    int length = gz_get_line_length(fp_gz)+1;
    if (gzseek(fp_gz, 0, SEEK_SET) != 0)
    {
        printf("ERROR: %s: unable to set gzip data file stream to 0", __func__);
        return;
    }

    char buffer[length+1];
    LOG("%s: line length is %i and buffer length is %li\n", __func__,  length, sizeof(buffer));
    int ffreq = fflush_frequency(length, specs.num_rt);
    LOG("%s: fflush_frequency is %i\n", __func__, ffreq);

    // PSEUDO CODE: START
    // READ LINE
    // get the record type
    // search for that RT's output file
    // if not found then
    // create one (write the column names as the first line of the file)
    // if found then check its file_length
    // if file_length is equal to or above MAX then 
    // close it and create a new outpuf file. add that output file to the link list of that RT
    // if file_length is below MAX then 
    // go through the RT's column's linked list cutting the line according to the length. skip BLANK columns
    // the cut chunks are processed to remove leading spaces and zeroes. 
    // processed cut chunk written into the RT's output file with a SEPARATOR
    // write '\n' into the output file
    // repeat the process - go back to READ LINE until EOF is reached
    // if EOF is reached then
    // go through RT linked list and close all the output files that are still open
    // PSEUDO CODE: END

    while (gzgets(fp_gz, buffer, length+1) != NULL)
    {
        buffer[length-1] = '\0';  // this overwrites the '\n' character at the end so that it is just the data from that line

        // LOG("%s[END OF LINE] Line size is %li\n", buffer, strlen(buffer));

        RecordTypeInfo *next = specs.first_rt;
        bool rt_spec_found = false;
        while (next != NULL)
        {
            if (strncmp(next->record_type, buffer, 2) == 0)
            {
                // LOG("%s: GOT HERE 1 with rt %s\n", __func__, next->record_type);
                // LOG("%s: output file pointer is %p\n", __func__, next->current_output_file);
                rt_spec_found = true;
                if (next->current_output_file == NULL || next->current_output_file->file_length >= MAX_WRITE_ROWS)
                {
                    // LOG("%s: GOT HERE 2\n", __func__);
                    open_new_output_file(next, input_filename); // TODO
                }
                write_row(next, buffer, ffreq);
                break;
            }
            else
            {
                next = next->next;
            }
        }
    }


    return;
}


void open_new_output_file(RecordTypeInfo *rt_specs, char *input_filename)
{
    LOG("%s: launched the function with rt %s\n", __func__, rt_specs->record_type);
    if (rt_specs->current_output_file != NULL)
    {
        fclose(rt_specs->current_output_file->fp);
        rt_specs->current_output_file->fp = NULL;
    }
    OutputFileInfo *new_file = malloc(sizeof(OutputFileInfo));
    new_file->previous_output_file = rt_specs->current_output_file;
    rt_specs->current_output_file = new_file;
    rt_specs->num_files++;

    new_file->file_length = 0;

    new_file->file_type = malloc(strlen(OUTPUT_FILETYPE) + 1);
    strcpy(new_file->file_type, OUTPUT_FILETYPE);
    
    char file_number[5]; // HARDCODED
    sprintf(file_number, "%d", rt_specs->num_files);
    
    new_file->file_name = malloc(strlen(input_filename) + strlen(OUTPUT_FILETYPE) + strlen(rt_specs->record_type) + strlen(file_number) + (strlen("_") * 2) + strlen(".") + 1);
    sprintf(new_file->file_name, "%s_%s_%s.%s", input_filename, rt_specs->record_type, file_number, OUTPUT_FILETYPE);
    LOG("%s: output filename is %s\n", __func__, new_file->file_name);

    FILE *ptr_output_file = fopen(new_file->file_name, "w");
    new_file->fp = ptr_output_file;

    ColumnInfo *next_column = rt_specs->first_column;
    char delimiter = DELIMITER;
    char newline = '\n';

    while (next_column != NULL)
    {
        if (strcmp(next_column->name, BLANK_COLUMN_NAME) == 0)
        {
            next_column = next_column->next;
            continue;
        }
        if (fwrite(next_column->name, strlen(next_column->name), 1, ptr_output_file) != 1)
        {
            printf("ERROR: %s: writing column name to new output file", __func__);
        }

        next_column = next_column->next;

        if (next_column != NULL)
        {
            fwrite(&delimiter, sizeof(delimiter), 1, ptr_output_file);
        }
        else
        {
            break;
        }
    }
    fwrite(&newline, sizeof(char), 1, ptr_output_file);
    fflush(ptr_output_file);
    LOG("%s: FINISHED\n", __func__);
}


void write_row(RecordTypeInfo *rt_specs, char *line, int ffreq)
{
    int start = 0;
    int len;
    char delimiter = DELIMITER;
    char newline = '\n';
    ColumnInfo *next_col = rt_specs->first_column;
    while (next_col != NULL)
    {
        if (strcmp(next_col->name, BLANK_COLUMN_NAME) == 0)
        {
            next_col = next_col->next;
            continue;
        }

        len = next_col->size;
        while (*(line+start) == '0' || *(line+start) == ' ')
        {
            start++;
            len--;
        }
        if (len > 0)
        {
            fwrite(line+start, len, 1, rt_specs->current_output_file->fp);  // some delicious pointer maths here ;)
        }
        start += len;

        next_col = next_col->next;
        if (next_col != NULL)
        {
            fwrite(&delimiter, sizeof(delimiter), 1, rt_specs->current_output_file->fp);
        }
        else
        {
            break;
        }
    }
    fwrite(&newline, sizeof(char), 1, rt_specs->current_output_file->fp);
    if (rt_specs->current_output_file->file_length % ffreq == 0)
    {
        fflush(rt_specs->current_output_file->fp);
    }
    
    rt_specs->current_output_file->file_length++;
}
