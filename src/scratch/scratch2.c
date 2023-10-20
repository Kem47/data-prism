#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void allocate_and_copy(char (**ptr_test)[3])
{
    *ptr_test = malloc(sizeof(char[3]) * 2);
    if (*ptr_test == NULL)
    {
        printf("Failed to allocate memory for ptr_test");
        return;
    }

    strcpy((*ptr_test)[0], "ab");
    strcpy((*ptr_test)[1], "de");
}

int main()
{
    char (*test)[3] = NULL;
    allocate_and_copy(&test);

    for (int i = 0; i < 2; i++)
    {
        printf("%s\n", test[i]);
    }

    free(test);
    return 0;
}