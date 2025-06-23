#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    char *test_cases[] = {
        "echo \"    \"$TOTO\"   \"",
        "echo 'hello'$USER'world'",
        "echo \"$HOME\"",
        "echo '$HOME'",
        NULL
    };
    
    printf("Test cases for quote handling:\n");
    for (int i = 0; test_cases[i] != NULL; i++)
    {
        printf("Test %d: %s\n", i + 1, test_cases[i]);
    }
    
    return 0;
} 