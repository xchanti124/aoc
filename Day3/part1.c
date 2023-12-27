#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char **split(char *str, char *seperators, int *count)
{
    int len = strlen(str);
    *count = 0;

    int i = 0;
    while (i < len)
    {
        while (i < len)
        {
            if (strchr(seperators, str[i]) == NULL)
                break;
            i++;
        }

        int prev_i = i;
        while (i < len)
        {
            if (strchr(seperators, str[i]) != NULL)
                break;
            i++;
        }

        if (i > prev_i)
            *count = *count + 1;
    }

    char **strings = malloc(sizeof(char *) * *count);

    i = 0;
    char buffer[16384];
    int str_ind = 0;

    while (i < len)
    {
        while (i < len)
        {
            if (strchr(seperators, str[i]) == NULL)
                break;
            i++;
        }

        int j = 0;
        while (i < len)
        {
            if (strchr(seperators, str[i]) != NULL)
                break;

            buffer[j] = str[i];
            i++;
            j++;
        }
        if (j > 0)
        {
            buffer[j] = '\0';

            int to_allocate = sizeof(char) * (strlen(buffer) + 1);

            strings[str_ind] = malloc(to_allocate);

            strcpy(strings[str_ind], buffer);

            str_ind++;
        }
    }
    return strings;
}

int find_int(int column, char *buffer, int row_length, bool *validity_buffer)
{
    char num_buffer[100];

    for (int i = 0; i < 100; i++)
    {
        num_buffer[i] = '\0';
    }
    int l = column;
    int l_char = buffer[l];

    while (l >= 0 && (l_char > 47 && l_char < 58))
    {
        char copy_num_buffer[100];
        memcpy(copy_num_buffer, num_buffer, 100);

        for (int i = 0; i < 99; i++)
        {
            num_buffer[i + 1] = copy_num_buffer[i];
        }

        num_buffer[0] = l_char;
        validity_buffer[l] = true;

        l -= 1;
        l_char = buffer[l];
    }

    int r = column + 1;
    int r_char = buffer[r];

    while (r < strlen(buffer) && (r_char > 47 && r_char < 58))
    {
        num_buffer[strlen(num_buffer)] = r_char;
        validity_buffer[r] = true;

        r += 1;
        r_char = buffer[r];
    }

    int answer = atoi(num_buffer);

    return answer;
}

int main()
{
    FILE *fptr = fopen("input.txt", "r");

    long flength = 0;

    fseek(fptr, 0, SEEK_END);
    flength = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);

    char *fcontent = malloc(sizeof(char) * flength + 1);

    for (long i = 0; i < flength; i++)
    {
        fcontent[i] += getc(fptr);
    }

    fcontent[flength] = '\0';

    int line_count = 0;

    char **split_new_line = split(fcontent, "\n", &line_count);

    /*

    char** char_buffer = malloc(sizeof(char) * line_count);

    for (int i = 0; i < line_count; i++) {
        char_buffer[i] = malloc(sizeof(char) * strlen(split_new_line[i]));
    }


    ///// stuff


    for (int i = 0; i < line_count; i++) {
        free(char_buffer[i]);
    }
    free(char_buffer);

    */

    int final_sum = 0;
    bool **visited = malloc(sizeof(bool *) * line_count);

    int max_j = 0;
    int max_i = 0;

    for (int i = 0; i < line_count; i++)
    {
        visited[i] = malloc(sizeof(bool) * strlen(split_new_line[i]));
        for (int j = 0; j < strlen(split_new_line[i]); j++)
        {
            visited[i][j] = false;
        }
    }

    for (int row_ind = 0; row_ind < line_count; row_ind++)
    {
        for (int col_ind = 0; col_ind < strlen(split_new_line[row_ind]); col_ind++)
        {
            if (split_new_line[row_ind][col_ind] != 46 && split_new_line[row_ind][col_ind] < 48 || split_new_line[row_ind][col_ind] > 57)
            {
                for (int offset_row = -1; offset_row <= 1; offset_row++)
                {
                    for (int offset_col = -1; offset_col <= 1; offset_col++)
                    {
                        if (offset_row == 0 && offset_col == 0)
                        {
                            continue;
                        }

                        int row = row_ind + offset_row;
                        int col = col_ind + offset_col;

                        bool is_in_bounds = row >= 0 && col >= 0 && row < line_count && col <= strlen(split_new_line[row]);

                        if (!is_in_bounds)
                        {
                            continue;
                        }

                        bool is_numeric = split_new_line[row][col] > 47 && split_new_line[row][col] < 58;
                        // printf("accessing %d:%d\n", row, col);
                        bool is_visited = visited[row][col];
                        // printf("accessed.\n");

                        if (is_numeric && !is_visited)
                        {
                            final_sum += find_int(col, split_new_line[row], strlen(split_new_line[row]), visited[row]);
                        }
                    }
                }
            }
        }
    }

    printf("final sum: %d\n", final_sum);

    // char final_num[100];
    // ['4', '\0', '\0', '\0', '\0', ..., '\0']
    // ['3', '4', '\0', '\0', '\0', ..., '\0']

    return 0;
}
