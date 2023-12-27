#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

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

int main()
{
    FILE *fptr = fopen("input.txt", "r");

    long flength = 0;

    fseek(fptr, 0, SEEK_END);
    flength = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);

    char *fcontent = malloc(sizeof(char) * (flength + 1)); // FREED

    for (long i = 0; i < flength; i++)
    {
        fcontent[i] = getc(fptr);
    }

    fcontent[flength] = '\0';

    char first_part[25000];

    char second_part[25000];

    int lines_count = 0;
    char **lines = split(fcontent, "\n", &lines_count); // FREED

    int final_sum = 0;

    for (int i = 0; i < lines_count; i++)
    {
        char *strip_lines = strip(lines[i]); // FREED
        int game_count = 0;
        char **cur_game = split(strip_lines, ":", &game_count); // FREED
        char *game_id = cur_game[0];
        char *game_cards = cur_game[1];
        char *strip_game_id = strip(game_id);       // FREED
        char *strip_game_cards = strip(game_cards); // FREED

        int card_set_count = 0;
        char **card_sets = split(strip_game_cards, "|", &card_set_count); // FREED
        char *lucky_numbers = card_sets[0];
        char *my_numbers = card_sets[1];

        char *lucky_numbers_stripped = strip(lucky_numbers); // FREED
        char *my_numbers_stripped = strip(my_numbers);       // FREED

        int lucky_count = 0;
        char **lucky_numbers_split = split(lucky_numbers_stripped, " ", &lucky_count); // FREED

        int my_count = 0;
        char **my_numbers_split = split(my_numbers_stripped, " ", &my_count); // FREED

        int counter = 0;

        for (int j = 0; j < lucky_count; j++)
        {
            int j_int = atoi(lucky_numbers_split[j]);

            for (int k = 0; k < my_count; k++)
            {
                int k_int = atoi(my_numbers_split[k]);

                if (j_int == k_int)
                {
                    counter++;
                }
            }
        }

        if (counter == 0)
        {
            continue;
        }

        int card_value = pow(2, counter - 1);
        final_sum += card_value;
        printf("%d ^ %d = %d\n", 2, counter - 1, card_value);

        for (int j = 0; j < my_count; j++)
        {
            free(my_numbers_split[j]);
        }
        free(my_numbers_split);

        for (int j = 0; j < lucky_count; j++)
        {
            free(lucky_numbers_split[j]);
        }
        free(lucky_numbers_split);

        free(my_numbers_stripped);

        free(lucky_numbers_stripped);

        for (int j = 0; j < card_set_count; j++)
        {
            free(card_sets[j]);
        }
        free(card_sets);

        free(strip_game_cards);

        free(strip_game_id);

        for (int j = 0; j < game_count; j++)
        {
            free(cur_game[j]);
        }
        free(cur_game);

        free(strip_lines);
    }
    printf("final sum: %d", final_sum);

    for (int i = 0; i < lines_count; i++)
    {
        free(lines[i]);
    }
    free(lines);

    free(fcontent);

    return 0;
}
