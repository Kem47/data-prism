#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>
#include "logging.h"


char * is_df_zipped(char *ptr_in_ff, char *filetype)
{
    char *str_copy = malloc(strlen(ptr_in_ff) + 1);
    strcpy(str_copy, ptr_in_ff);
    int len = strlen(ptr_in_ff);
    LOG("is_df_zipped: len is: %i\n\tfilename is: %s\n\tfiletype is: %s\n", len, ptr_in_ff, filetype);
    int i = 0;
    int found = 0;
    int zip_ext_len = 0;
    char *delim = ".";
    char *token = strtok(str_copy, delim);
    while (token != NULL)
    {
        i++;
        LOG("is_df_zipped: token is: %s\n", token);
        if (strcmp(token, filetype) == 0)
        {
            found = i;
        }
        zip_ext_len = strlen(token);
        token = strtok(NULL, delim);
    }

    if (i > found)
    {
        char *zip_ext = ptr_in_ff + len - zip_ext_len - 1;
        printf("is_df_zipped: zip ext is: %s\n", zip_ext);
        return zip_ext;
    }
    else
    {
        return NULL;
    }
}


char * get_unzipped_filepath(char *filepath, char *zip_ext)
{
    int ext_len = strlen(zip_ext);
    int len = strlen(filepath);
    char *str_copy = malloc(strlen(filepath) + 1);
    strcpy(str_copy, filepath);
    str_copy[len-ext_len] = '\0';
    return str_copy;
}


void unzip(char *input_filepath, char *output_filepath)
{
    // Open the input file for reading
    FILE *input_file = fopen(input_filepath, "rb");
    if (input_file == NULL)
    {
        printf("Error: could not open input file \"%s\"\n", input_filepath);
        return;
    }

    // Determine the size of the input file
    fseek(input_file, 0, SEEK_END);
    long input_size = ftell(input_file);
    fseek(input_file, 0, SEEK_SET);

    // Allocate a buffer for the input data
    char *input_data = malloc(input_size);
    if (input_data == NULL)
    {
        printf("Error: could not allocate memory for input data\n");
        fclose(input_file);
        return;
    }

    // Read the input data from the file
    if (fread(input_data, 1, input_size, input_file) != input_size)
    {
        printf("Error: could not read input data from file\n");
        free(input_data);
        fclose(input_file);
        return;
    }

    // Close the input file
    fclose(input_file);

    // Allocate a buffer for the output data
    char *output_data = malloc(input_size * 30);
    if (output_data == NULL)
    {
        printf("Error: could not allocate memory for output data\n");
        free(input_data);
        return;
    }

    // Uncompress the input data using zlib
    uLongf output_size = input_size * 30;
    int result = uncompress((Bytef *)output_data, &output_size, (Bytef *)input_data, input_size);
    if (result != Z_OK)
    {
        printf("Error: could not uncompress input data (error code %d)\n", result);
        free(input_data);
        free(output_data);
        return;
    }

    // Open the output file for writing
    FILE *output_file = fopen(output_filepath, "wb");
    if (output_file == NULL)
    {
        printf("Error: could not open output file \"%s\"\n", output_filepath);
        free(input_data);
        free(output_data);
        return;
    }

    // Write the output data to the file
    if (fwrite(output_data, 1, output_size, output_file) != output_size)
    {
        printf("Error: could not write output data to file\n");
        free(input_data);
        free(output_data);
        fclose(output_file);
        return;
    }

    // Close the output file
    fclose(output_file);

    // Free the input and output buffers
    free(input_data);
    free(output_data);
}



void ungzip(char *input_filepath, char *output_filepath)
{
    // Open the input file for reading
    FILE *input_file = fopen(input_filepath, "rb");
    if (input_file == NULL)
    {
        printf("Error: could not open input file \"%s\"\n", input_filepath);
        return;
    }

    // Determine the size of the input file
    fseek(input_file, 0, SEEK_END);
    long input_size = ftell(input_file);
    fseek(input_file, 0, SEEK_SET);

    // Allocate a buffer for the input data
    char *input_data = malloc(input_size);
    if (input_data == NULL)
    {
        printf("Error: could not allocate memory for input data\n");
        fclose(input_file);
        return;
    }

    // Read the input data from the file
    if (fread(input_data, 1, input_size, input_file) != input_size)
    {
        printf("Error: could not read input data from file\n");
        free(input_data);
        fclose(input_file);
        return;
    }

    // Close the input file
    fclose(input_file);

    // Skip over the gzip header
    if (input_size < 10 || memcmp(input_data, "\x1f\x8b\x08", 3) != 0)
    {
        printf("Error: input data is not in gzip format\n");
        free(input_data);
        return;
    }
    int offset = 10;
    if ((input_data[3] & 4) == 4)
    {
        offset += 2 + (input_data[10] | (input_data[11] << 8));
    }
    if ((input_data[3] & 8) == 8)
    {
        offset = strchr(input_data + offset, '\0') - input_data + 1;
    }
    if ((input_data[3] & 16) == 16)
    {
        offset = strchr(input_data + offset, '\0') - input_data + 1;
    }
    if ((input_data[3] & 2) == 2)
    {
        offset += 2;
    }

    // Allocate a buffer for the output data
    char *output_data = malloc(input_size * 30);
    if (output_data == NULL)
    {
        printf("Error: could not allocate memory for output data\n");
        free(input_data);
        return;
    }

    // Uncompress the input data using zlib
    uLongf output_size = input_size * 30;
    int result = uncompress((Bytef *)output_data, &output_size, (Bytef *)(input_data + offset), input_size - offset - 8);
    if (result != Z_OK)
    {
        printf("Error: could not uncompress input data (error code %d)\n", result);
        free(input_data);
        free(output_data);
        return;
    }

    // Open the output file for writing
    FILE *output_file = fopen(output_filepath, "wb");
    if (output_file == NULL)
    {
        printf("Error: could not open output file \"%s\"\n", output_filepath);
        free(input_data);
        free(output_data);
        return;
    }

    // Write the output data to the file
    if (fwrite(output_data, 1, output_size, output_file) != output_size)
    {
        printf("Error: could not write output data to file\n");
        free(input_data);
        free(output_data);
        fclose(output_file);
        return;
    }

    // Close the output file
    fclose(output_file);

    // Free the input and output buffers
    free(input_data);
    free(output_data);
}