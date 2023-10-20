// TESTING ZLIB INFLATE FUNCTION. THIS IS THE LONG AND COMPLEX ALTERNATIVE TO UNCOMPRESS()
// at the moment I do not need this because zlib also includes a suite of functions to open and read gzip files.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

#define CHUNK_SIZE 16384

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

    // Check that the file is in gzip format
    char magic_number[2];
    int bytes_read = fread(magic_number, 1, 2, input_file);
    if (bytes_read < 2)
    {
        printf("Error: could not read magic number from file\n");
        fclose(input_file);
        return 1;
    }
    if (memcmp(magic_number, "\x1f\x8b", 2) != 0)
    {
        printf("Error: input file is not in gzip format\n");
        fclose(input_file);
        return 1;
    }

    // Initialize the zlib stream
    z_stream stream;
    memset(&stream, 0, sizeof(stream));
    stream.zalloc = Z_NULL;
    stream.zfree = Z_NULL;
    stream.opaque = Z_NULL;
    stream.avail_in = 0;
    stream.next_in = Z_NULL;
    int ret = inflateInit2(&stream, 16 + MAX_WBITS);
    if (ret != Z_OK)
    {
        printf("Error: could not initialize zlib stream\n");
        fclose(input_file);
        return 1;
    }

    // Decompress the input file
    char in_buffer[CHUNK_SIZE];
    char out_buffer[CHUNK_SIZE];
    int flush = Z_NO_FLUSH;
    do {
        stream.avail_in = fread(in_buffer, 1, CHUNK_SIZE, input_file);
        if (ferror(input_file))
        {
            printf("Error: could not read input file\n");
            inflateEnd(&stream);
            fclose(input_file);
            return 1;
        }
        if (stream.avail_in == 0)
        {
            flush = Z_FINISH;
        }
        stream.next_in = (Bytef *)in_buffer;

        do {
            stream.avail_out = CHUNK_SIZE;
            stream.next_out = (Bytef *)out_buffer;
            ret = inflate(&stream, flush);
            if (ret == Z_STREAM_ERROR)
            {
                printf("Error: zlib stream error\n");
                inflateEnd(&stream);
                fclose(input_file);
                return 1;
            }
            int have = CHUNK_SIZE - stream.avail_out;
            fwrite(out_buffer, 1, have, stdout);
        } while (stream.avail_out == 0);
    } while (flush != Z_FINISH);

    // Clean up the zlib stream
    inflateEnd(&stream);

    // Close the input file
    fclose(input_file);

    return 0;
}