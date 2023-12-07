#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_LINES 220
#define LINE_WORDS 25
#define WORD_CHARS 25

char *** load_data(int *lines, char *filename);
int is_empty(char *word);
long * get_seeds(char ***data);
long get_param(char ***data, int lines, long compare_value, char *first_identifier, char *second_identifier);
long get_location(char ***data, int doc_lines, long seed);

int main(void)
{
    char ***data;
    long *seeds;
    long *locations;
    long new_location;
    int doc_lines;
    long i, j;
    long min_location_seed, min_location;

    data = load_data(&doc_lines, "input.txt");
    if (!data)
    {
        printf("NO DATA\n");
        return(1);
    }

    locations = (long *)malloc(LINE_WORDS * sizeof(long));
    for (i = 0; i < LINE_WORDS; i++)
        locations[i] = -1;

    seeds = get_seeds(data);
    for (i = 0; i < LINE_WORDS; i++)
        if (seeds[i] > 0)
            locations[i] = get_location(data, doc_lines, seeds[i]);
        else
            break;
    
    min_location = 0;
    for (i = 0; i < LINE_WORDS; i++)
        if (seeds[i] > 0)
        {
            if (locations[i] < locations[min_location] && locations[min_location] > 0)
                min_location = i;
        }
        else
            break;
    
    printf("First part minimum location: %li of seed: %li\n", locations[min_location], seeds[min_location]);

    /* Second part */
    min_location_seed = 0;
    min_location = 90000000;
    for (i = 0; i < LINE_WORDS; i = i + 2)
        if (seeds[i] > 0)
        {
            for (j = 0; j < seeds[i+1]; j++)
            {
                new_location = get_location(data, doc_lines, seeds[i] + j);
                /*
                printf("seed %li location %li\n", seeds[i] + j, new_location);
                */
                if (new_location < min_location && new_location != 0)
                {
                    min_location = new_location;
                    min_location_seed = seeds[i] + j;
                }
            }
        }
        else
            break;

    printf("Second part minimum location: %li of seed: %li\n", min_location, min_location_seed);

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
            /* New line chars */
            /*
            print_line(data[line]);
            */
            line++;
            word = 0;
            ch = 0;
        }
        else if (c == ' ' || c == '-')
        {
            /* New word chars  */
            word++;
            ch = 0;
        }
        else if (c != ':')
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

