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