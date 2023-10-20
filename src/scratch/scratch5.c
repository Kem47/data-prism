#include <zlib.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];

    // Open the input file for reading
    gzFile input_file = gzopen(filename, "rb");
    if (input_file == NULL)
    {
        printf("Error: could not open input file \"%s\"\n", filename);
        return 1;
    }

    // Read the first line of the file
    char buffer[1024];
    char *read = gzgets(input_file, buffer, 1024);
    printf("read is: %s\n", read);
    // if (bytes_read < 0)
    // {
    //     printf("Error: could not read data from file\n");
    //     gzclose(input_file);
    //     return 1;
    // }

    // Print the first line of the file
    printf("%s", buffer);

    // Free the buffer and close the input file
    // free(buffer);
    gzclose(input_file);

    return 0;
}