#include "parse.h"


void gz_parser(Specs specs, gzFile fp_gz)
{
    int rt_len = strlen(specs.first_rt->record_type);
    LOG("%s: rt_len is %i\n", __func__, rt_len);

    int length = gz_get_line_length(fp_gz);
    if (gzseek(fp_gz, 0, SEEK_SET) != 0)
    {
        printf("ERROR: %s: unable to set gzip data file stream to 0", __func__);
        return;
    }
    LOG("%s: line length is %i\n", __func__,  length);
    char buffer[length+1];
    if (gzgets(fp_gz, buffer, length) != NULL)
    {
        buffer[length] = '\n';
        LOG("%s[END]\n", buffer);
    }



    return;
}
