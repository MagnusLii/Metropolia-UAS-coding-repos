/*Write a function that converts all white space (tab, cr, lf etc.)  to spaces (' ') and merges spaces so that there is only one space between the words. If the string starts or ends with white space only one leading/trailing space is left in the string.

The prototype of the function is

void merge_whitespace(char *);

Note that you may write any number of helper functions to complete the task. The test bench calls merge_whitespace() with different test strings and will accept code if output matches the reference solution.

Hint: To see the test strings start with a dummy function that does not modify the string passed as parameter.
*/


#include <stdio.h>
#include <string.h>
#include <ctype.h>

void merge_whitespace(char *);

void merge_whitespace(char *str)
{
    // replace space
    int i, j;
    for (i = 0; i < strlen(str); i++)
    {
        if (isspace(str[i]))
        {
            str[i] = ' ';
        }
    }

    // merge space
    for (i = 0; i < strlen(str); i++)
    {
        if (str[i] == ' ')
        {
            for (j = i; j < strlen(str); j++)
            {
                if (str[j] != ' ')
                {
                    break;
                }
            }
            if (j - i > 1)
            {
                memmove(&str[i + 1], &str[j], strlen(str) - j + 1);
            }
        }
    }


}