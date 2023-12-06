#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WORD_LEN 5

void clear_word(char *word);

int valid_char(char *word, char **data, int i, int j, int max_i, int max_j);
long detect_numbers(char **rows, int index, int index2, int max_i, int max_j);
int is_digit(char c);
int find_digit_left(char **data, int j, int i, unsigned long long *subproduct);
int find_digit_right(char **data, int j, int i, unsigned long long *subproduct);

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
    unsigned long long total = 0;
    unsigned long long value = 0;

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

    number_c = (char *)malloc(WORD_LEN * sizeof(char));
    clear_word(number_c);

    for (index2 = 0; index2 < line; index2++)
    {
        for (index = 0; index < column; index++)
        {
            last_c = c;
            c = rows[index2][index];
            if (is_digit(c))
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
                word_index = 0;
                clear_word(number_c);
            }
        }
    }

    printf("First part value: %lli\n", total);

    total = 0;
    value = 0;

    for (index2 = 0; index2 < line; index2++)
    /*
    for (index2 = 35; index2 < 36; index2++)
    */
    {
        for (index = 0; index < column; index++)
        {
            last_c = c;
            c = rows[index2][index];
            if (c == '*')
            {
                value = detect_numbers(rows, index, index2, line, column);
                total += value; 
            }
        }
    }

    printf("Second part value: %lli\n", total);

    return(0);
}

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

    for (index_j = top; index_j <= bottom; index_j++)
    for (index_j = top; index_j <= bottom; index_j++)
        for (index_i = left; index_i <= right; index_i++)
        {
            c = data[index_j][index_i];
            if ((c <= '/' && c >= '!' && c != '.') || (c >= ':' && c <= '@'))
                return(1);
        }
    return(0);
}

int is_digit(char c)
{
    return(c >= '0' && c <= '9');
}

int find_digit_left(char **data, int j, int i, unsigned long long *subproduct)
{
    int index_i;
    int end_index_i = -1;

    for (index_i = i; ; index_i--)
    {
        if(!is_digit(data[j][index_i]))
        {
            end_index_i = find_digit_right(data, j, index_i + 1, subproduct);
            break;
        }
    }
    return(end_index_i);
}

int find_digit_right(char **data, int j, int i, unsigned long long *subproduct)
{
    int index_i, sub_index_i;
    int end_index_i = -1;
    char c_number[6] = {'\0'};
    unsigned long long value = 0;

    for (index_i = i; ; index_i++)
    {
        if(!is_digit(data[j][index_i]))
        {
            end_index_i = index_i - 1;
            for (sub_index_i = i; sub_index_i <= end_index_i; sub_index_i++)
                c_number[sub_index_i - i] = data[j][sub_index_i];
            value = strtol(c_number, NULL, 0);
            *subproduct *= value;
            clear_word(c_number);
            break;
        }
    }

    return(end_index_i);
}

long detect_numbers(char **data, int i, int j, int max_i, int max_j)
{
    int top, left, right, bottom;
    int index_i, index_j;
    int end_index_i;
    char c;
    char single[2] = {'\0'};
    unsigned long long subproduct = 1;
    int part_numbers = 0;

    /* top */
    top = j > 0 ? j - 1 : j;
    /* left */
    left = i > 0 ? i - 1 : i;
    /* bottom */
    bottom = j < max_j ? j + 1 : j;
    /* right */
    right = i < max_i ? i + 1 : i;

    for (index_j = top; index_j <= bottom; index_j++)
        for (index_i = left; index_i <= right; index_i++)
        {
            c = data[index_j][index_i];
            if (is_digit(c))
            {
                if (is_digit(data[index_j][index_i - 1]))
                {
                    /* digit continues to the left, find where it ends */
                    end_index_i = find_digit_left(data, index_j, index_i, &subproduct);
                    part_numbers++;
                }
                else if (is_digit(data[index_j][index_i + 1]))
                {
                    /* digit continues to the right, find where it ends */
                    end_index_i = find_digit_right(data, index_j, index_i, &subproduct);
                    part_numbers++;
                }
                else
                {
                    /* single character number */
                    end_index_i = index_i;
                    single[0] = c;
                    subproduct *= strtol(single, NULL, 0);
                    single[0] = '\0';
                    part_numbers++;
                }

                if (end_index_i > index_i && end_index_i >= right && index_j < bottom)
                {
                    index_i = left - 1;
                    index_j++;
                    end_index_i = -1;
                }
                else if (end_index_i > index_i && end_index_i < right && index_j < bottom)
                {
                    index_i = end_index_i;
                    end_index_i = -1;
                }
                else if (index_j == bottom && index_i < right && end_index_i < right && end_index_i > index_i)
                    index_i = end_index_i + 1;
                else if (end_index_i == right && index_j == bottom)
                    index_i = end_index_i;
                else if (end_index_i > right && index_j == bottom)
                    break;
            }
        }

    return(part_numbers == 2 ? subproduct : 0);
}

