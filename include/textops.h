#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_LINES 1010
#define LINE_WORDS 25
#define WORD_CHARS 25
char *** load_data(char *filename, int *lines, int *columns);
int free_data(char ****data);
int is_empty(char *word);
int copy_word(char *source, char *target);
void print_words(char **line);
void print_sentences(char ***data, long lines);

int is_word(char *word)
{
    int result;
    result = word[0] != '\0' && word[0] != '\n' && word[0] != ' ';
    return(result);
}

char *** load_data(char *filename, int *lines, int *words)
{
    FILE *fp;
    char c;
    int line, word, ch;
    int i, j, k;
    char ***data;

    /* Initialize file data structure */
    data = (char ***)malloc(FILE_LINES * (LINE_WORDS * (WORD_CHARS * sizeof(char))));
    if (!data) return(NULL);
    for (i = 0; i < FILE_LINES; i++)
    {
        /* data line i */
        data[i] = (char**)malloc(LINE_WORDS * (WORD_CHARS * sizeof(char)));
        if (!data[i]) return(NULL);
        for (j = 0; j < LINE_WORDS; j++)
        {
            /* data line i, word j */
            data[i][j] = (char *)malloc(WORD_CHARS * sizeof(char));
            if (!data[i][j]) return(NULL);
            for (k = 0; k < WORD_CHARS; k++)
            {
                /* data line i, word j, character k */
                data[i][j][k] = '\0';
            }
        }
    }

    fp = fopen(filename, "r");
    if (!fp) return(NULL);

    line = 0;
    *words = 0;
    ch = 0;
    word = 0;
    while(1)
    {
        if (feof(fp)) break;

        c = fgetc(fp);

        if (c == '\n')        
        {
            line++;
            *words = word > *words ? word : *words;
            word = 0;
            ch = 0;
        }
        else if ((c == ' ' || c == '-') && ch > 0)
        {
            /* New word chars  */
            word++;
            ch = 0;
        }
        else if (c != ':' && c != ' ')
        {
            /* Add char except ignore chars */
            data[line][word][ch] = c; 
            ch++;
        }
    }

    fclose(fp);

    *lines = line;
    return(data);
}

int is_empty(char *word)
{
    return(word[0] == '\0');
}

void print_words(char **line)
{
    int i;
    for (i = 0; i < LINE_WORDS; i++)
        if (line[i][0] != '\0' && line[i+1][0] != '\0')
            printf("%s,", line[i]);
        else if (line[i][0] != '\0')
            printf("%s\n", line[i]);
        else
            break;
}

void print_sentences(char ***data, long lines)
{
    long i;
    for (i = 0; i < lines; i++)
        if (data[i][0][0] != '\0')
            print_words(data[i]);
        else
            break;
}

int copy_word(char *source, char *target)
{
    int i;
    if (!source || !target) return(1);
    for (i = 0; i < WORD_CHARS; i++)
        target[i] = source[i];
    return(0);
}

int free_data(char ****data_to_free)
{
    char ***data;
    int i, j;

    if (data_to_free == NULL) return(1);
    data = *data_to_free;

    if (!data) return(1);
    for (i = 0; i < FILE_LINES; i++)
    {
        if (!data[i]) return(1);
        for (j = 0; j < LINE_WORDS; j++)
        {
            if (!data[i][j]) return(1);
            /* free each word */
            free(data[i][j]);
            data[i][j] = NULL;
        }
        /* free each line */
        free(data[i]);
        data[i] = NULL;
    }   
    /* free the whole structure */
    free(data);
    data = NULL;
    *data_to_free = NULL;

    return(0);
}

