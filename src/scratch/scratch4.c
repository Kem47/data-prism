#include <stdio.h>
#include <string.h>

char *assign_str(void)
{
    char *test;
    test = "Hello";
    char *test2;
    strcpy(test2, test);
    return test2;
}

int main(void)
{
    char *test;
    test = assign_str();
    printf("the test string is: %s\n", test);
}