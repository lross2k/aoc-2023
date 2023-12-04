#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define WORD_LEN 10
#define SENTENCE_LEN 60
#define MAX_RED 12
#define MAX_GREEN 13
#define MAX_BLUE 14

void clear_word(char *tobe);
void clear_contents(char **line_contents);

int main(void)
{
    FILE *fp;
    char c;
    int line = 0;
    int column = 0;
    long game_number = 0;
    int line_column = 0;
    int index = 0;
    int subindex = 0;
    char word[WORD_LEN] = {'\0'};
    char (line_contents[SENTENCE_LEN])[WORD_LEN];
    long max_red_found = 0;
    long max_blue_found = 0;
    long max_green_found = 0;
    long total_number = 0;

    for (index = 0; index < SENTENCE_LEN; index++)
        for (subindex = 0; subindex < WORD_LEN; subindex++)
            line_contents[index][subindex] = '\0';

    fp = fopen("input.txt", "r");
    if (!fp) return(1);
    
    while(1)
    {
        c = fgetc(fp);
        if (feof(fp)) break;

        if (c != ' ' && c != '\n')
        {
            if (c != ':' && c != ';' && c != ',')
                word[column] = c;            
        }
        else if (c == ' ' || c == '\n')
        {
            word[column] = '\0';
            for (index = 0; index < WORD_LEN; index++)
            {
                line_contents[line_column][index] = word[index];
            }
            clear_word(word);
            line_column++;
            column = -1;
            if (c == '\n')
            {
                line++;
                line_column = 0;
                for (index = 0; index < SENTENCE_LEN; index++)
                {
                    if (strcmp(line_contents[index], "Game") == 0)
                        game_number = strtol(line_contents[index + 1], NULL, 10);
                    else if (strcmp(line_contents[index], "red") == 0)
                        max_red_found = strtol(line_contents[index - 1], NULL, 10) > max_red_found ? strtol(line_contents[index - 1], NULL, 10) : max_red_found;
                    else if (strcmp(line_contents[index], "blue") == 0)
                        max_blue_found = strtol(line_contents[index - 1], NULL, 10) > max_blue_found ? strtol(line_contents[index - 1], NULL, 10) : max_blue_found;
                    else if (strcmp(line_contents[index], "green") == 0)
                    {
                        max_green_found = strtol(line_contents[index - 1], NULL, 10) > max_green_found ? strtol(line_contents[index - 1], NULL, 10) : max_green_found;
                        /*
                        printf(" %s : %ld -> %ld ", line_contents[index - 1], strtol(line_contents[index - 1], NULL, 10), max_green_found);
                        */
                    }
                }
                if (max_red_found <= MAX_RED && max_green_found <= MAX_GREEN && max_blue_found <= MAX_BLUE && max_red_found > 0)
                {
                    printf("Game #%ld ", game_number);
                    printf("red %ld, green %ld, blue %ld", max_red_found, max_green_found, max_blue_found);
                    printf("\n");
                    total_number += game_number;
                }
                max_red_found = 0;
                max_green_found = 0;
                max_blue_found = 0;
                for (index = 0; index < SENTENCE_LEN; index++)
                    for (subindex = 0; subindex < WORD_LEN; subindex++)
                        line_contents[index][subindex] = '\0';
            }
        }

        /*
        if (line > 5) break;
        */

        column++;
    }

    printf("Total number: %ld\n", total_number);
}

void clear_word(char *tobe)
{
    int index;
    for (index = 0; index < WORD_LEN; index++)
        tobe[index] = '\0';
}

