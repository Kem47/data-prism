#include <cs50.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>
#include "unzip.h"
#include "spec.h"
#include "parse.h"
#include "logging.h"
#include "configs.h"
#include "utils.h"


// typedef struct
// {
//     char *in_ff;
//     char *in_spec;
//     char (*rt_list)[3];
//     int rt_count;
// } inputs;

int manual_inputs(char **ptr_in_ff, char **ptr_in_spec, char (**ptr_rt_list)[3], uint8_t **ptr_rt_count);
void remove_trailing_spaces(char *str);
char* get_file_spec_match(FILE *fp_spec, char *df_name);


int main(int argc, char *argv[])
{
    // char *in_env_logging = getenv("LOGGING");
    // if (in_env_logging && atoi(in_env_logging) == 1)
    // {

    // }

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
        printf("You have not included the required inputs when running this program.\nThe correct usage is as follows:\nFor individual file: ./%s input.file file_spec.dfs 03 06 16 28 etc\nFor folders: ./%s input_folder file_spec.dfs 03 06 16 28 etc\n", PROGRAM_NAME, PROGRAM_NAME);
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
        ptr_in_ff = malloc(sizeof(argv[1]));
        if (ptr_in_ff == NULL)
        {
            printf("Failed to allocate memory for ptr_in_ff");
            return 2;
        }
        ptr_in_spec = malloc(sizeof(argv[2]));
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
                ptr_rt_list[i][3] = '\0';
                LOG("%s\n", ptr_rt_list[i]);
            }
        }


        // version for before "all" was introduced
        // *ptr_rt_count = argc - 3;
        // ptr_rt_list = malloc(3 * (*ptr_rt_count));
        // if (ptr_rt_list == NULL)
        // {
        //     printf("Failed to allocate memory for ptr_rt_list");
        //     return 2;
        // }
        // for (int i = 0; i < *ptr_rt_count; i++)
        // {
        //     strcpy(ptr_rt_list[i], argv[i+3]);
        //     ptr_rt_list[i][3] = '\0';
        //     LOG("%s\n", ptr_rt_list[i]);
        // }
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


    Specs specs = get_specs(fp_spec, ptr_rt_list, ptr_rt_count);
    print_specs(&specs);
    LOG("the num_rt is: %i\n", specs.num_rt);
    fclose(fp_spec);

    gzFile fp_gz = gzopen(ptr_in_ff, "r");
    if (fp_gz == NULL)
    {
        printf("ERROR: %s: unable to open gzip data file\n", __func__);
        return 1;
    }
    gz_parser(specs, fp_gz);
    gzclose(fp_gz);

    // This has been commented out because at the moment the strategy is to use gzopen and gzread from zlib
    // Because of that I do not need to find out if it's zipped.
    //
    // char *zip_ext;
    // zip_ext = is_df_zipped(ptr_in_ff, filetype);
    // if (zip_ext != NULL)
    // {
    //     LOG("the file zip extension is: %s\n", zip_ext);
    //     char *unzipped_filepath = get_unzipped_filepath(ptr_in_ff, zip_ext);
    //     LOG("the unzipped filepath is: %s\n", unzipped_filepath);
    //     unzip(ptr_in_ff, unzipped_filepath);
    // }
    // else
    // {
    //     LOG("the file is not zipped\n");
    // }





    return 0;
}







char* get_file_spec_match(FILE *fp_spec, char *df_name)
{
    int length = get_line_length(fp_spec);
    fseek(fp_spec, 0, SEEK_SET);
    char buffer[length+1];
    fgets(buffer, sizeof(buffer), fp_spec);
    char *target = "filename";
    if (strstr(buffer, target) == NULL)
    {
        printf("\"filename\" was not found on the first line of the spec file.\n");
    }

    char *filenames = buffer + 10;
    LOG("the length of the line is: %i\n", length);
    LOG("the line is: %s\n", buffer);
    LOG("the filenames are: %s\n", filenames);

    char *delim = " ";
    char *filename_token = strtok(filenames, delim);
    while (filename_token != NULL)
    {
        LOG("the token is: %s\n", filename_token);
        // TODO: add case insensitivity: convert both filename_token and df_name to upper/lower
        if (strstr(df_name, filename_token) != NULL)
        {
            LOG("returning filename token: %s\n", filename_token);
            char *return_value = malloc(sizeof(filename_token));
            strcpy(return_value, filename_token);
            return return_value;
        }
        filename_token = strtok(NULL, delim);
    }
    return NULL;
}


int manual_inputs(char **ptr_in_ff, char **ptr_in_spec, char (**ptr_rt_list)[3], uint8_t **ptr_rt_count)
{
    *ptr_in_ff = get_string("Enter the name of the data file or the folder that contains multiple data files: ");
    *ptr_in_spec = get_string("Enter the name of the spec file: ");

    char *in_rt_blob = get_string("Enter the list of record types separated by space (Don't leave out leading zeros): ");
    // LOG("rt_blob is: %s\n", in_rt_blob);
    // LOG("this is len of in_rt_blob: %li\n", strlen(in_rt_blob));
    remove_trailing_spaces(in_rt_blob);

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

void remove_trailing_spaces(char *str)
{
    int len = strlen(str);
    while (len > 0 && isspace(str[len - 1]))
    {
        len--;
    }
    str[len] = '\0';
}





// void in_specfile_verification(char *file)
// {
//     int length = strlen(file);

// }
