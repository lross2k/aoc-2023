#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "textops.h"

enum card {
    high_card,
    one_pair,
    two_pair,
    three_of_a_kind,
    full_house,
    four_of_a_kind,
    five_of_a_kind
};

int initialize_arrays(uint16_t **bids, uint16_t **types, char ***cards, int lines);
int load_cards(char ***data, char **cards, int lines);
int load_bids(char ***data, uint16_t *bids, int lines);
int calculate_types(uint16_t *types, char **cards, int lines);

int main(void)
{
    char ***data = NULL, **cards = NULL;
    uint16_t *bids = NULL, *types = NULL;
    int lines = 0, words = 0;
    int ret;

    data = load_data("example.txt", &lines, &words);
    if (data == NULL || words == 0 || lines == 0) return(1);
    print_sentences(data, FILE_LINES);

    ret = initialize_arrays(&bids, &types, &cards, lines);
    if (ret) return(ret);

    ret = load_cards(data, cards, lines);
    if (ret) return(ret);

    ret = load_bids(data, bids, lines);
    if (ret) return(ret);

    ret = free_data(&data);
    if (ret) return(ret);

    ret = calculate_types(types, cards, lines);
    if (ret) return(ret);

    return(0);
}

int initialize_arrays(uint16_t **bids, uint16_t **types, char ***cards, int lines)
{
    int i, j;

    /* same amount of data lines than len for new arrays */
    *bids = (uint16_t *) malloc(lines * sizeof(uint16_t));
    *types = (uint16_t *) malloc(lines * sizeof(uint16_t));
    *cards = (char **) malloc(lines * WORD_CHARS * sizeof(char));
    if (*bids == NULL || *types == NULL || *cards == NULL) return(1);
    for (i = 0; i < lines; i++)
    {
        (*bids)[i] = 0;
        (*types)[i] = 0;
        (*cards)[i] = (char *) malloc(WORD_CHARS * sizeof(char));
        if ((*cards)[i] == NULL) return(1);
        for (j = 0; j < WORD_CHARS; j++)
            (*cards)[i][j] = '\0';
    }
    return(0);
}

int load_cards(char ***data, char **cards, int lines)
{
    int i, ret;

    for (i = 0; i < lines; i++)
    {
        ret = copy_word(data[i][0], cards[i]);
        if (ret) return(1);
    }
    return(0);
}

int load_bids(char ***data, uint16_t *bids, int lines)
{
    int i;

    for (i = 0; i < lines; i++)
        bids[i] = (uint16_t) strtol(data[i][0], NULL, 0);
    return(0);
}

int calculate_types(uint16_t *types, char **cards, int lines)
{
    int i, j, k;
    uint8_t occurences[13] = {0};
    /* 2, 3, 4, 5, 6, 7, 8, 9, T, J, Q, K, A */
    char ch;

    for (i = 0; i < lines; i++)
    {
        for (j = 0; j < WORD_CHARS; j++)
        {
            ch = cards[i][j];
            if (ch >= '2' || ch <= '9')
                occurences[ch - '2'] += 1;
            else if (ch == 'T')
                occurences[8] += 1;
            else if (ch == 'J')
                occurences[9] += 1;
            else if (ch == 'Q')
                occurences[10] += 1;
            else if (ch == 'K')
                occurences[11] += 1;
            else if (ch == 'A')
                occurences[12] += 1;
            else
                return
        }
    }
    return(0);
}

