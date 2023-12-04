#include <stdio.h>
#include <string.h>

#define MAX_LINES 1010

int main(void)
{
    FILE *fp;
    char c;

    int line = 0;
    int calibrations[MAX_LINES] = {-1};

    char first_char = ' ';
    char second_char = ' ';
    char previous_char = ' ';

    int line_code = 0;

    int total_value = 0;

    fp = fopen("input.txt", "r");
    /* no file */
    if (!fp) return(1);
    while(1) {
        c = fgetc(fp);
        if (feof(fp)) break;

        if (c >= '0' && c <= '9' && first_char == ' ')
        {
            /* is a digit and no first char */
            first_char = c;
            previous_char = c;
        }
        else if (c >= '0' && c <= '9' && first_char != ' ')
        {
            /* is a digit and already first char */
            previous_char = c;
        }
        if (c == '\n' && first_char != ' ')
        {
            /* update value for last char */
            second_char = previous_char;

            line_code = (first_char - '0') * 10 + second_char - '0';

            printf("%c + %c = %i\n", first_char, second_char, line_code);

            calibrations[line] = line_code;

            line++;
            first_char = ' ';
        }
    }
    fclose(fp);

    for (line = 0; line < MAX_LINES; line++)
    {
        if (calibrations[line] != -1)
        {
            total_value += calibrations[line];
        }
    }

    printf("%i\n", total_value);

    return(0); 
}

