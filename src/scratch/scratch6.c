#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GZIP_MAGIC_NUMBER "\x1f\x8b"

typedef struct {
    unsigned char id1;
    unsigned char id2;
    unsigned char compression_method;
    unsigned char flags;
    unsigned int modification_time;
    unsigned char extra_flags;
    unsigned char os_type;
} gzip_header_t;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];

    // Open the input file for reading
    FILE *input_file = fopen(filename, "rb");
    if (input_file == NULL)
    {
        printf("Error: could not open input file \"%s\"\n", filename);
        return 1;
    }

    // Read the first few bytes of the file to check for the gzip magic number
    char magic_number[2];
    int bytes_read = fread(magic_number, 1, 2, input_file);
    if (bytes_read < 2)
    {
        printf("Error: could not read magic number from file\n");
        fclose(input_file);
        return 1;
    }

    printf("\"\\x%02x\\x%02x\"\n", (unsigned char)magic_number[0], (unsigned char)magic_number[1]);

    // Check that the file is in gzip format
    if (memcmp(magic_number, GZIP_MAGIC_NUMBER, 2) != 0)
    {
        printf("Error: input file is not in gzip format\n");
        fclose(input_file);
        return 1;
    }

    // Read the gzip header from the file
    gzip_header_t header;
    bytes_read = fread(&header, 1, sizeof(gzip_header_t), input_file);
    if (bytes_read < sizeof(gzip_header_t))
    {
        printf("Error: could not read gzip header from file\n");
        fclose(input_file);
        return 1;
    }

    // Extract information from the gzip header
    int compression_level = (header.flags >> 1) & 0x03;
    int compression_strategy = (header.flags >> 3) & 0x03;
    int has_filename = (header.flags >> 3) & 0x01;
    int has_comment = (header.flags >> 4) & 0x01;
    int has_extra = (header.flags >> 2) & 0x01;

    // Print information about the gzip file
    printf("Compression level: %d\n", compression_level);
    printf("Compression strategy: %d\n", compression_strategy);
    printf("Has filename: %d\n", has_filename);
    printf("Has comment: %d\n", has_comment);
    printf("Has extra: %d\n", has_extra);

    // Close the input file
    fclose(input_file);

    return 0;
}