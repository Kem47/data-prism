#include "parse.h"

void gz_parser(Specs specs, gzFile fp_gz, char *input_filename)
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
    
    // if (gzgets(fp_gz, buffer, length+1) != NULL)
    // {
    //     // buffer[length] = '\0';
    //     LOG("%s[END]\n", buffer);
    // }


    // for (int i = 1, limit = 400; i < limit; i++)
    // {
    //     char c;
    //     c = gzgetc(fp_gz);
    //     LOG("Character %i is \"%c\"\n", i, c);
    // }

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
        buffer[length-1] = '\0';
        LOG("%s[END OF LINE] Line size is %li\n", buffer, strlen(buffer));
        // char input_record_type[rt_len+1];
        // for (int i = 0, limit = rt_len + 1; i < limit; i++)
        // {
        //     input_record_type[i] = buffer[i];
        // }
        // input_record_type[rt_len] = '\n';

        RecordTypeInfo *next = specs.first_rt;
        bool rt_spec_found = false;
        while (next != NULL)
        {
            if (strncmp(next->record_type, buffer, 2) == 0)
            {
                LOG("%s: GOT HERE 1 with rt %s\n", __func__, next->record_type);
                LOG("%s: output file pointer is %p\n", __func__, next->current_output_file);
                rt_spec_found = true;
                if (next->current_output_file == NULL || next->current_output_file->file_length >= MAX_WRITE_ROWS)
                {
                    LOG("%s: GOT HERE 2\n", __func__);
                    open_new_output_file(next, input_filename); // TODO
                }
                else
                {
                    LOG("%s: HERE INSTEAD\n", __func__);
                }
                write_row();
                break;
            }
            else
            {
                next = next->next;
            }
        }

        if (!rt_spec_found)
        {
            buffer[rt_len] = '\n';
            printf("WARNING: your spec file did not have specs for record type %s\n", buffer);
        }
    }


    return;
}


void open_new_output_file(RecordTypeInfo *rt_specs, char *input_filename)
{
    LOG("%s: launched the function with rt %s\n", __func__, rt_specs->record_type);
    // if there is a pointer to a file currently in the record type then close it.
    if (rt_specs->current_output_file != NULL)
    {
        fclose(rt_specs->current_output_file->fp);
    }
    OutputFileInfo *new_file = malloc(sizeof(OutputFileInfo));
    new_file->previous_output_file = rt_specs->current_output_file;
    rt_specs->current_output_file = new_file;
    rt_specs->num_files++;

    new_file->file_length = 0;
    new_file->file_type = malloc(sizeof(OUTPUT_FILETYPE) + 1);
    strcpy(new_file->file_type, OUTPUT_FILETYPE);
    char file_number[5]; // HARDCODED
    sprintf(file_number, "%d", rt_specs->num_files);
    new_file->file_name = malloc(sizeof(input_filename) + sizeof(OUTPUT_FILETYPE) + sizeof(rt_specs->record_type) + strlen(file_number) + (sizeof("_") * 2) + sizeof(".") + 1);
    sprintf(new_file->file_name, "%s_%s_%s.%s", input_filename, rt_specs->record_type, file_number, OUTPUT_FILETYPE);
    LOG("%s: output filename is %s\n", __func__, new_file->file_name);
    // "input/B20231010.PVX.Z_02_1.csv"

    FILE *ptr_output_file = fopen(new_file->file_name, "a");
    new_file->fp = ptr_output_file;

    ColumnInfo *next_column = rt_specs->first_column;

    while (next_column != NULL)
    {
        

        next_column = next_column->next;
    }
}

void write_row(void)
{
    //
}