#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// configuration: 12 red, 13 green, 14 blue

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

char *strip(char *str)
{
    bool isEmpty = strlen(str) == 0;
    bool isSingularAndWhitespace = strlen(str) == 1 && (str[0] == ' ' || str[0] == '\n');

    if (isEmpty || isSingularAndWhitespace)
    {
        char *stripped_str = malloc(sizeof(char));
        stripped_str[0] = '\0';
        return stripped_str;
    }

    int l;
    int r;

    for (int i = 0; i < strlen(str); i++)
    {
        l = i;

        if (str[i] != ' ' && str[i] != '\n')
        {
            break;
        }
    }

    for (int i = strlen(str) - 1; i >= 0; i--)
    {
        r = i + 1;

        if (str[i] != ' ' && str[i] != '\n')
        {
            break;
        }
    }

    if (l > r)
    {
        char *stripped_str = malloc(sizeof(char));
        stripped_str[0] = '\0';
        return stripped_str;
    }

    char *stripped_str = malloc(sizeof(char) * (r - l + 1));
    memcpy(stripped_str, str + l, r - l + 1);
    stripped_str[r - l] = '\0';

    return stripped_str;
}

int main()
{
    long flength = 0;

    FILE *fptr = fopen("input.txt", "r");

    fseek(fptr, 0, SEEK_END);
    flength = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);

    int red = 12;
    int green = 13;
    int blue = 14;
    int final_sum = 0;

    char *fcontent = malloc(sizeof(char) * flength + 1);

    for (long i = 0; i < flength; i++)
    {
        fcontent[i] = getc(fptr);
    }

    fcontent[flength] = '\0';

    int count_lines = 0;
    char **games = split(fcontent, "\n", &count_lines);

    for (int i = 0; i < count_lines; i++)
    {
        bool impossible = false;
        int total_red = 0;
        int total_green = 0;
        int total_blue = 0;

        int count_parts = 0;
        char *game = strip(games[i]);
        char **game_parts = split(game, ":", &count_parts);
        char *game_name = game_parts[0];
        game = strip(game_parts[1]);

        int count_rounds = 0;
        char **rounds = split(game, ";", &count_rounds);

        for (int j = 0; j < count_rounds; j++)
        {
            int count_round = 0;
            char *round = strip(rounds[j]);
            char **draws = split(round, ",", &count_round);

            for (int k = 0; k < count_round; k++)
            {
                int count_draw = 0;
                char *draw = strip(draws[k]);
                char **pair = split(draw, " ", &count_draw);
                char *count = pair[0];
                char *color = pair[1];
                int int_count = atoi(count);

                if (strcmp(color, "red") == 0)
                {
                    if (int_count > red)
                    {
                        impossible = true;
                    }
                }
                else if (strcmp(color, "green") == 0)
                {
                    if (int_count > green)
                    {
                        impossible = true;
                    }
                }
                else if (strcmp(color, "blue") == 0)
                {
                    if (int_count > blue)
                    {
                        impossible = true;
                    }
                }
            }
        }

        if (!impossible)
        {
            final_sum += i + 1;
            // printf("%s counted (sum: %d)\n", game_name, final_sum);
        }
    }

    printf("%d\n", final_sum);

    return 0;
}