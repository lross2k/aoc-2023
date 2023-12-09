#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define FILE_LINES 220
#define LINE_WORDS 25
#define WORD_CHARS 25
char *** load_data(int *lines, char *filename);
int is_empty(char *word);
void print_words(char **line);
long * get_seeds(char ***data);
long get_param(char ***data, int lines, long compare_value, char *first_identifier, char *second_identifier);
long get_location(char ***data, int doc_lines, long seed);
void print_sentences(char ***data, long line_len);

int is_word(char *word)
{
    int result;
    result = word[0] != '\0' && word[0] != '\n' && word[0] != ' ';
    return(result);
}

int main(void)
{
    char ***data;
    int doc_lines;
    long time_index, distance_index;
    long current_record_distance, current_max_time;
    double x_1, x_2, discriminant, top_value, bottom_value;
    long i;
    int total;

    data = load_data(&doc_lines, "input2.txt");
    if (!data)
    {
        printf("NO DATA\n");
        return(1);
    }

    print_sentences(data, LINE_WORDS);

    for (i = 0; i < FILE_LINES; i++)
    {
        if (!is_word(data[i][0]))
            break;
        else if(strcmp(data[i][0], "Time") == 0)
            time_index = i;   
        else if(strcmp(data[i][0], "Distance") == 0)
            distance_index = i;   
    }

    total = 1;
    for (i = 1; i < LINE_WORDS; i++)
    {
        if (!is_word(data[time_index][i]))
            break;

/* Distance line has the record distance in millimeters */
        current_record_distance = strtol(data[distance_index][i], NULL, 0);
/* Time line has the maximum duration in milliseconds */
        current_max_time = strtol(data[time_index][i], NULL, 0);

/* Boat speed starts at 0 millimeters / millisecond */
/* Boat speed increases 1 mm / ms for each whole ms held at the beginning */
        /* 200 mm on 30 ms 
         * means min_beating_speed = 200 mm / 30 ms 
         * don't even try holding_time = 0 or holding_time = t_n
         * distance = t_max*t - t^2
         * t^2 - t_max*t + distance = 0 
         * a = 1 
         * b = -t_max 
         * c = distance */
        discriminant = current_max_time * current_max_time - 4 * current_record_distance;
        x_1 = (-1 * current_max_time + sqrt(discriminant))/(2 * -1);
        x_2 = (-1 * current_max_time - sqrt(discriminant))/(2 * -1);
        
        bottom_value = ceil(x_1) > x_1 ? ceil(x_1) : x_1 + 1.0;
        top_value = floor(x_2) < x_2 ? floor(x_2) : x_2 - 1.0;
        
        total *= (int)(top_value - bottom_value + 1.0);
    }

    printf("Total value: %i\n", total);

    return(0);
}

char *** load_data(int *lines, char *filename)
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
    ch = 0;
    word = 0;
    while(1)
    {
        if (feof(fp)) break;

        c = fgetc(fp);

        if (c == '\n')        
        {
            line++;
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

long * get_seeds(char ***data)
{
    int i;
    long *seeds = (long *)malloc(LINE_WORDS * sizeof(long));
    for (i = 0; i < LINE_WORDS; i++)
        seeds[i] = -1;
    for (i = 0; i < LINE_WORDS; i++)
    {
        if (is_empty(data[0][i + 1])) break;
        seeds[i] = strtol(data[0][i + 1], NULL, 0);
    }
    return(seeds);
}

int is_empty(char *word)
{
    return(word[0] == '\0');
}

long get_location(char ***data, int doc_lines, long seed)
{
    long soil, fertilizer, water, light, temperature, humidity, location;

    soil = get_param(data, doc_lines, seed, "seed", "soil");
    fertilizer = get_param(data, doc_lines, soil, "soil", "fertilizer");
    water = get_param(data, doc_lines, fertilizer, "fertilizer", "water");
    light = get_param(data, doc_lines, water, "water", "light");
    temperature = get_param(data, doc_lines, light, "light", "temperature");
    humidity = get_param(data, doc_lines, temperature, "temperature", "humidity");
    location = get_param(data, doc_lines, humidity, "humidity", "location");
    return(location);
}

long get_param(char ***data, int lines, long compare_value, char *first_identifier, char *second_identifier)
{
    long i, j;
    int start_of_data;
    long first_start, data_range;
    long value_soil;

    start_of_data = -1;
    first_start = -1;
    data_range = -1;
    for (i = 2; i < lines; i++)
        if (strcmp(data[i][0], first_identifier) == 0 && strcmp(data[i][2], second_identifier) == 0)
        {
            start_of_data = i + 1;
            break;
        }

    if (start_of_data < 0) return(-1);

    for (i = start_of_data; i < lines; i++)
    {
        if (is_empty(data[i][0]))
            break;
        else
        {
            first_start = strtol(data[i][1], NULL, 0);            
            data_range = strtol(data[i][2], NULL, 0);            
            
            if (compare_value >= first_start && compare_value <= first_start + data_range)
            {
                /* value searched exists in the given range */
                j = compare_value - first_start;
                value_soil = strtol(data[i][0], NULL, 0) + j;
                return(value_soil);
            }
        }
    }
    return(compare_value);
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

void print_sentences(char ***data, long line_len)
{
    long i;
    for (i = 0; i < line_len; i++)
        if (data[i][0][0] != '\0')
            print_words(data[i]);
        else
            break;
}

