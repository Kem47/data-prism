#include "spec.h"

Specs get_specs(FILE *fp_spec, char (*ptr_rt_list)[3], uint8_t *ptr_rt_count)
{
    Specs specs = {.num_rt = 0, .first_rt = NULL};
    // LOG("GOT HERE\n");
    // int rt_len = strlen(ptr_rt_list[0]);
    int rt_len;
    // LOG("ptr_rt_count is %i\n", *ptr_rt_count);
    // LOG("get_specs: after initialisation num_rt is: %i\n", specs.num_rt);
    // // LOG("get_specs: rt_len is: %i\n", rt_len);
    fseek(fp_spec, 0, SEEK_SET);
    char buffer[1024];  // HARDCODED this value of 1024 is hard-coded and could cause issues if any lines in the spec file are longer. Need to see if I can add some dynamism to this.
    while (fgets(buffer, sizeof(buffer), fp_spec) != NULL)
    {
        if (strncmp("rt:", buffer, 3) == 0)
        {
            // LOG("get_specs: Got here!\n");
            // LOG("get_specs: the buffer is: %s", buffer);
            // get rts
            // LOG("get_specs: the buffer shifted is: %s\n", buffer+3);
            trim_whitespace(buffer+3);
            // LOG("get_specs: after trimming whitespaces: %s\n", buffer+3);
            rt_len = strlen(buffer+3);
            // LOG("get_specs: rt_len is: %i\n", rt_len);
            bool rt_todo = false;

            char *delim = " ";
            char *token = strtok(buffer+3, delim);
            while (token != NULL)
            {
                if (*ptr_rt_count == 0)
                {
                        // LOG("get_specs: ALL TOKENS ARE ON THE LIST: %s\n", token);
                        rt_todo = true;
                        RecordTypeInfo *temp_rt = malloc(sizeof(RecordTypeInfo));
                        temp_rt->current_output_file = NULL;
                        temp_rt->record_type = malloc(rt_len + 1);
                        strcpy(temp_rt->record_type, token);
                        temp_rt->num_files = 0;
                        temp_rt->num_columns = 0;
                        temp_rt->first_column = NULL;
                        temp_rt->next = specs.first_rt;
                        specs.first_rt = temp_rt;
                        specs.num_rt++;
                }
                else
                {
                    // LOG("get_specs: token is: %s\n", token);
                    for (int i = 0; i < *ptr_rt_count; i++)
                    {
                        if (strcmp(ptr_rt_list[i], token) == 0)
                        {
                            // LOG("get_specs: THIS TOKEN IS ON THE LIST: %s\n", token);
                            rt_todo = true;
                            RecordTypeInfo *temp_rt = malloc(sizeof(RecordTypeInfo));
                            temp_rt->current_output_file = NULL;
                            temp_rt->record_type = malloc(rt_len + 1);
                            strcpy(temp_rt->record_type, token);
                            temp_rt->num_files = 0;
                            temp_rt->num_columns = 0;
                            temp_rt->first_column = NULL;
                            temp_rt->next = specs.first_rt;
                            specs.first_rt = temp_rt;
                            specs.num_rt++;
                        }
                    }
                }
                token = strtok(NULL, delim);
            }
            
            if (rt_todo)
            {
                // LOG("GOT to column name cutting\n");
                // freads next line and assign them into columns
                ColumnInfo *first = NULL;
                ColumnInfo *last = NULL;
                int count = 0;
                if (fgets(buffer, sizeof(buffer), fp_spec) != NULL)
                {
                    char *delim = ",";
                    char *token = strtok(buffer, delim);
                    while (token != NULL)
                    {
                        trim_whitespace(token);
                        // LOG("column_name: the name of the column is %s\n", token);
                        ColumnInfo *temp_col = malloc(sizeof(ColumnInfo));
                        // ColumnInfo *temp_col = calloc(1, sizeof(ColumnInfo));  // use this to avoid "Conditional jump or move depends on uninitialised value(s)" message from Valgrind
                        // LOG("column_name: got past first malloc\n");
                        temp_col->name = malloc(strlen(token) + 1);
                        // LOG("column_name: got past second malloc\n");
                        strcpy(temp_col->name, token);
                        // temp_col->size = 0;  // use this to avoid "Conditional jump or move depends on uninitialised value(s)" message from Valgrind
                        temp_col->ref_count = 0; // use this to avoid "Conditional jump or move depends on uninitialised value(s)" message from Valgrind
                        temp_col->next = NULL; // use this to avoid "Conditional jump or move depends on uninitialised value(s)" message from Valgrind
                        count++;
                        if (last == NULL)
                        {
                            first = temp_col;
                            last = temp_col;
                        }
                        else
                        {
                            last->next = temp_col;
                            last = temp_col;
                        }
                        token = strtok(NULL, delim);
                        // LOG("column_name: got to the end of column name code\n");
                    }
                }
                else
                {
                    printf("Error: no column names found for RTs: %s\n", buffer+3);
                }

                // LOG("GOT to column width cutting\n");
                // freads next line and assign them into widths
                if (fgets(buffer, sizeof(buffer), fp_spec) != NULL)
                {
                    // LOG("GETS HERE 2\n");
                    int temp_width;
                    ColumnInfo *next = first;
                    char *delim = ",";
                    char *token = strtok(buffer, delim);
                    while (token != NULL)
                    {
                        // LOG("GETS HERE 3\n");
                        trim_whitespace(token);
                        // LOG("column_width: column width before atoi is %s\n", token);
                        temp_width = atoi(token);
                        // LOG("column_width: column name is \"%s\", column width is %i\n", next->name, temp_width);
                        next->size = temp_width;
                        next = next->next;
                        token = strtok(NULL, delim);
                    }
                }
                else
                {
                    printf("Error: no column widths found.\n");
                }
                
                // LOG("GETS HERE 4\n");
                // assign the pointer of this column/width linked list to all RecordTypeInfo that have NULL
                RecordTypeInfo *next = specs.first_rt;
                while (next != NULL)
                {
                    if (next->first_column == NULL)
                    {
                        next->first_column = first;
                        next->num_columns = count;
                        first->ref_count++;  // adding the count of references to this column linked list so that I can free it correctly
                    }
                    next = next->next;
                }
            }
        }
    }

    // print_specs(&specs);
    return specs;
}


void print_specs(Specs *specs)
{
    printf("Number of record types: %d\n", specs->num_rt);

    for (RecordTypeInfo *rt_info = specs->first_rt; rt_info != NULL; rt_info = rt_info->next)
    {
        printf("Record type: %s\n", rt_info->record_type);
        printf("Number of columns: %d\n", rt_info->num_columns);

        for (ColumnInfo *col_info = rt_info->first_column; col_info != NULL; col_info = col_info->next)
        {
            printf("Column name: \"%s\"\n", col_info->name);
            printf("Column size: %d\n", col_info->size);
            printf("Column ref count: %i\n", col_info->ref_count);
        }
    }
}


void trim_whitespace(char *str)
{
    char *start = str;
    while (isspace(*start))
    {
        start++;
    }

    char *end = str + strlen(str) - 1;
    while (end > start && isspace(*end))
    {
        end--;
    }
    *(end + 1) = '\0';

    if (start != str)
    {
        memmove(str, start, end - start + 2);
    }
}