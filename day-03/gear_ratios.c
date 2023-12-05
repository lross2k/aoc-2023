#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WORD_LEN 5

void clear_word(char *word)
{
    int index;
    for (index = 0; index < WORD_LEN; index++)
        word[index] = '\0';
}

int valid_char(char *word, char **data, int i, int j, int max_i, int max_j)
{
    int top, left, right, bottom;
    long int len;
    int index_i, index_j;
    char c;

    /* in case the number was the end of line */
    if (i == 0)
    {
        i = max_i - 1;
        j--;
    }

    len = strlen(word);

    /* top */
    top = j > 0 ? j - 1 : j;
    /* left */
    left = i > 0 ? i - len - 1 : i - len;
    /* bottom */
    bottom = j < max_j ? j + 1 : -1;
    /* right */
    right = i < max_i ? i : i - 2;

    /*
    printf("\nword: %s top: %i bottom: %i left: %i right: %i\n", word, top, bottom, left, right);
    */

    for (index_j = top; index_j <= bottom; index_j++)
        for (index_i = left; index_i <= right; index_i++)
        {
            c = data[index_j][index_i];
            if ((c <= '/' && c >= '!' && c != '.') || (c >= ':' && c <= '@'))
            {
                printf(" %c ", c);
                return(1);
            }
        }
    return(0);
}

int main(void)
{
    FILE *fp;
    char c;
    char last_c;
    int line = 0;
    int column = 0;
    int index;
    int index2;
    int word_index;
    char **rows;
    char *number_c;
    int valid;
    long int total = 0;
    long int value;

    fp = fopen("input.txt", "r");
    if (!fp) return(1);

    while(1)
    {
        if (feof(fp)) break;
        c = fgetc(fp);

        if (c == '\n')
            line++;
        else if (line < 1)
            column++;
    }
    fclose(fp);
    printf("lines: %i, columns %i\n", line, column);

    /* Declare variables for text */
    rows = (char**)malloc(line * (column * sizeof(char)));
    for (index = 0; index < line; index++)
        rows[index] = (char*)malloc(column * sizeof(char));

    /* Initialize variables */
    for (index2 = 0; index2 < line; index2++)
        for (index = 0; index < column; index++)
            rows[index2][index] = '\0';

    fp = fopen("input.txt", "r");
    if (!fp) return(1);
    for (index2 = 0; index2 < line; index2++)
    {
        for (index = 0; index < column; index++)
        {
            if (feof(fp)) break;
            c = fgetc(fp);
            if (c == '\n')
                c = fgetc(fp);
            rows[index2][index] = c > 10 ? c : ' ';
        }
    }
    fclose(fp);

    /*
    for (index2 = 0; index2 < line; index2++)
    {
        for (index = 0; index < column; index++)
            printf("%c", rows[index2][index]);
        printf("\n");
    }
    */

    number_c = (char *)malloc(WORD_LEN * sizeof(char));
    clear_word(number_c);

    for (index2 = 0; index2 < line; index2++)
    {
        for (index = 0; index < column; index++)
        {
            last_c = c;
            c = rows[index2][index];
            if (c >= '0' && c <= '9')
            {
                number_c[word_index] = c;
                word_index++;
            }
            else if (c <= '/' && last_c > '/')
            {
                valid = valid_char(number_c, rows, index, index2, line, column);
                if (valid)
                {
                    value = valid ? strtol(number_c, NULL, 0) : 0;
                    total += value;
                }
                else
                    printf("%s %li %i\n", number_c, strlen(number_c), valid);
                word_index = 0;
                clear_word(number_c);
            }
        }
    }

    printf("Total value: %li\n", total);

    return(0);
}



