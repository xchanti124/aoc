#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    long flength = 0;

    FILE *fptr = fopen("../input.txt", "r");

    fseek(fptr, 0, SEEK_END);
    flength = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);

    // char *fcontent = malloc(sizeof(char) * flength + 1);

    // for (long i = 0; i < flength; i++)
    // {
    //     fcontent[i] = getc(fptr);
    // }

    // fcontent[flength] = '\0';

    int first_num = -1;
    int last_num = -1;
    int final_sum = 0;

    for (long i = 0; i < flength; i++)
    {
        char cur_char = getc(fptr);

        if (cur_char == '\n')
        {
            last_num = last_num == -1 ? first_num : last_num;

            /*
            if (last_num == -1) {
                last_num = first_num;
            }
            */

            final_sum += first_num * 10 + last_num;

            first_num = -1;
            last_num = -1;
            continue;
        }

        if (cur_char < 48 || cur_char > 57)
        {
            continue;
        }

        int cur_num = cur_char - 48;

        if (first_num == -1)
        {
            first_num = cur_num;
            continue;
        }

        last_num = cur_num;
    }

    printf("%d", final_sum);

    // free(fcontent);
}
