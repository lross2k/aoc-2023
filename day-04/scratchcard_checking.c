#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WORD_LEN 10
#define LINE_LEN 50

void clear_word(char *word);
void clear_line(char **line);
void print_line(char **line);

int is_in(long value, long *array, int array_len)
{
    int i;
    for (i = 0; i < array_len; i++)
        if (array[i] == value)
            return(1);
    return(0);
}

int process_line(char **line)
{
    long *winning_numbers;    
    long *numbers;
    long value;
    int i, stopped_winning, winning_len, numbers_len;
    int subtotal = 0;

    winning_numbers = (long *)malloc(LINE_LEN * sizeof(long));
    numbers = (long *)malloc(LINE_LEN * sizeof(long));

    stopped_winning = 0;
    winning_len = 0;
    numbers_len = 0;
    for (i = 2; i < LINE_LEN; i++)
    {
        if (strcmp(line[i], "|") == 0)
            stopped_winning = 1;
        else if (!stopped_winning)
        {
            winning_numbers[winning_len] = strtol(line[i], NULL, 0);
            winning_len++;
        }
        else
        {
            value = strtol(line[i], NULL, 0);
            if (value == 0)
                break;
            numbers[numbers_len] = value;
            numbers_len++;
        }
    }

    for (i = 0; i < winning_len; i++)
        printf("%li ", winning_numbers[i]);
    printf("\n");
    for (i = 0; i < numbers_len; i++)
        printf("%li ", numbers[i]);
    printf("\n");
    for (i = 0; i < numbers_len; i++)
    {
        if (is_in(numbers[i], winning_numbers, winning_len))
        {
            printf("Number %li is a winner!\n", numbers[i]);
            subtotal = subtotal > 0 ? subtotal * 2 : 1;
        }
    }
    printf("Group value %i\n", subtotal);
    return(subtotal);
}

int main(void)
{
    FILE *fp;
    char c;
    char **line;
    int i, j;
    int char_num, word_num;
    int total = 0;

    line = (char **)malloc(LINE_LEN * (WORD_LEN * sizeof(char)));
    for (i = 0; i < LINE_LEN; i++)
        line[i] = (char *)malloc(WORD_LEN * sizeof(char));

    for (j = 0; j < LINE_LEN; j++)
        for (i = 0; i < WORD_LEN; i++)
            line[j][i] = '\0';

    fp = fopen("input.txt", "r");
    if (!fp) return(1);

    char_num = 0;
    word_num = 0;
    while(1)
    {
        if (feof(fp)) break;
        c = fgetc(fp);
        if (c < 10) break;

        if (c != ' ' && c != '\n' && c != ':')
        {
            line[word_num][char_num] = c;
            char_num++;
        }
        else if (c == ' ' && char_num > 0)
        {
            char_num = 0;
            word_num++;
        }
        else if (c == '\n')
        {
            print_line(line);
            total += process_line(line);
            clear_line(line);
            word_num = 0;
            char_num = 0;
        }
    }

    printf("Total %i\n", total);

    return(0);
}

void clear_word(char *word)
{
    int i;
    for (i = 0; i < WORD_LEN; i++)
        word[i] = '\0';
}

void clear_line(char **line)
{
    int i;
    for (i = 0; i < LINE_LEN; i++)
        clear_word(line[i]);
}

void print_line(char **line)
{
    int i;
    for (i = 0; i < LINE_LEN; i++)
        if (line[i][0] != '\0')
            printf("%s,", line[i]);
    printf("\n");
}

