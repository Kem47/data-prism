#include <stdio.h>
#include <string.h>

char * get_test(void)
{
    char test[] = "one two three four five";
    char *target = "three";
    char *delim = " ";
    char *token = strtok(test, delim);
    while (token != NULL)
    {
        printf("the token is: %s\n", token);
        if (strstr(token, target) != NULL)
        {
            return token;
        }
        token = strtok(NULL, delim);
    }
    return NULL;
}


int main(void)
{
    char *test = get_test();
    printf("the test string is %s\n", test);
}