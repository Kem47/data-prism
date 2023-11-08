#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cs50.h>
#include <zlib.h>
#include "configs.h"
#include "logging.h"
#include "parse.h"
#include "spec.h"
#include "unzip.h"
#include "utils.h"


int manual_inputs(char **ptr_in_ff, char **ptr_in_spec, char (**ptr_rt_list)[3], uint8_t **ptr_rt_count);


int main(int argc, char *argv[])
{
    printf("\n========================================\n  DATA-PRISM\n  Program by KEMAL ATAYEV\n  Version: Beta 2\n========================================\n\n");

    // USER INPUTS

    // Initialising the input pointers
    char *ptr_in_ff = NULL;
    char *ptr_in_spec = NULL;
    char (*ptr_rt_list)[3] = NULL;

    uint8_t *ptr_rt_count = malloc(sizeof(uint8_t));
    if (ptr_rt_count == NULL)
    {
        printf("Failed to allocate memory for ptr_rt_count");
        return 2;
    }

    if (argc < 4)
    {
        printf("You have not included the required inputs when running this program in CLI.\nThe correct usage is as follows:\nFor individual file: ./%s input.file file_spec.dfs 03 06 16 28 etc\n", PROGRAM_NAME);
        char guided_entry = get_char("Would you like to enable manual entry mode? (y/n) ");
        if (guided_entry == 'y')
        {
            manual_inputs(&ptr_in_ff, &ptr_in_spec, &ptr_rt_list, &ptr_rt_count);
        }
        else
        {
            printf("Then please try again with correct usage.\nThank you!\n");
            return 1;
        }
    }
    else
    {
        ptr_in_ff = malloc(strlen(argv[1]) + 1);
        if (ptr_in_ff == NULL)
        {
            printf("Failed to allocate memory for ptr_in_ff");
            return 2;
        }
        ptr_in_spec = malloc(strlen(argv[2]) + 1);
        if (ptr_in_spec == NULL)
        {
            printf("Failed to allocate memory for ptr_in_spec");
            return 2;
        }
        strcpy(ptr_in_ff, argv[1]);
        strcpy(ptr_in_spec, argv[2]);

        if (strcmp(argv[3], "all") == 0)
        {
            *ptr_rt_count = 0;
        }
        else
        {
            *ptr_rt_count = argc - 3;
            ptr_rt_list = malloc(3 * (*ptr_rt_count));
            if (ptr_rt_list == NULL)
            {
                printf("Failed to allocate memory for ptr_rt_list");
                return 2;
            }
            for (int i = 0; i < *ptr_rt_count; i++)
            {
                strcpy(ptr_rt_list[i], argv[i+3]);
                ptr_rt_list[i][2] = '\0';
                LOG("%s\n", ptr_rt_list[i]);
            }
        }
    }
    LOG("Input file/folder is: %s\n", ptr_in_ff);
    LOG("Input spec file is: %s\n", ptr_in_spec);
    for (int i = 0; i < *ptr_rt_count; i++)
    {
        LOG("RT number %i is: %s\n", i, ptr_rt_list[i]);
    }

    FILE *fp_spec = fopen(ptr_in_spec, "r");
    if (!fp_spec)
    {
        printf("Failed opening the spec file!\n");
    }

    char *filetype = get_file_spec_match(fp_spec, ptr_in_ff);
    if (filetype == NULL)
    {
        printf("The data file and the spec do not match!\n");
        return 1;
    }
    else
    {
        LOG("The filename is: %s\n", filetype);
    }
    free(filetype);

    Specs specs = get_specs(fp_spec, ptr_rt_list, ptr_rt_count);
    if (*ptr_rt_count != 0)
    {
        comp_specs_inputs(&specs, ptr_rt_list, ptr_rt_count);
    }
    // print_specs(&specs);
    LOG("the num_rt is: %i\n", specs.num_rt);
    fclose(fp_spec);

    gzFile fp_gz = gzopen(ptr_in_ff, "r");
    if (fp_gz == NULL)
    {
        printf("ERROR: %s: unable to open gzip data file\n", __func__);
        return 1;
    }
    gz_parser(specs, fp_gz, ptr_in_ff);
    gzclose(fp_gz);

    // close_output_files(&specs);
    // release_mem_fp(&specs);
    release_mem_fp_rec(&specs);
    
    free(ptr_rt_count);
    free(ptr_in_ff);
    free(ptr_in_spec);
    free(ptr_rt_list);

    printf("=== COMPLETE ===\n");

    return 0;
}


int manual_inputs(char **ptr_in_ff, char **ptr_in_spec, char (**ptr_rt_list)[3], uint8_t **ptr_rt_count)
{
    *ptr_in_ff = get_string("Enter the name of the data file: ");
    *ptr_in_spec = get_string("Enter the name of the spec file: ");

    char *in_rt_blob = get_string("Enter the list of record types separated by space (Don't leave out leading zeros) or 'all' for all record types: ");
    // LOG("rt_blob is: %s\n", in_rt_blob);
    // LOG("this is len of in_rt_blob: %li\n", strlen(in_rt_blob));
    remove_trailing_spaces(in_rt_blob);

    if (strcmp(in_rt_blob, "all") == 0)
    {
        **ptr_rt_count = 0;
        return 0;
    }
    else
    {
        **ptr_rt_count = ((uint8_t) strlen(in_rt_blob) + 1) / 3;
        // LOG("this is ptr_rt_count: %i\n", **ptr_rt_count);
        *ptr_rt_list = malloc((**ptr_rt_count) * sizeof(char[3]));
        // LOG("these are the various sizes: %li, %li, %li\n", sizeof(ptr_rt_list), sizeof(*ptr_rt_list), sizeof(**ptr_rt_list));
        if (*ptr_rt_list == NULL)
        {
            printf("Failed to allocate memory for ptr_rt_list");
            return 2;
        }
        
        char *input_delim = " ";
        char *in_rt_token = strtok(in_rt_blob, input_delim);

        for (int i = 0; i < **ptr_rt_count && in_rt_token != NULL; i++)
        {
            // LOG("this is the token: %s\n", in_rt_token);
            strcpy((*ptr_rt_list)[i], in_rt_token);
            in_rt_token = strtok(NULL, input_delim);
            // LOG("%s\n", (*ptr_rt_list)[i]);
        }
        return 0;
    }
}
