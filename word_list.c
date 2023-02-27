#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "word_list.h"
#include "util.h"

void word_list_init(struct word_list* wlp) {
    wlp->word_count = 0;
    wlp->word_buf_siz = 0;
    wlp->words = NULL;
    wlp->word_letter_bits = NULL;
    for (int j = 0; j < 26; j += 1) {
        wlp->start_letter_idx[j] = -1;
        wlp->start_letter_count[j] = -1;
    }
}

void word_list_add(struct word_list* wlp, char* word) {
    int letter_count = alphalen(word);
    if (letter_count < 1) {
        return;
    }
    int bits = word_bits(word);
    if (!bits) {
        return;
    }
    if (word_list_contains(wlp, word)) {
        return;
    }
    if (!wlp->word_buf_siz) {
        wlp->word_buf_siz = 16384;
    }
    if (NULL == wlp->words) {
        if (NULL == (wlp->words = (char**)malloc(wlp->word_buf_siz * sizeof(*(wlp->words))))) {
            die("malloc");
        }
        if (NULL == (wlp->word_letter_bits = (int*)malloc(wlp->word_buf_siz * sizeof(char*)))) {
            die("malloc");
        }
    } else if (wlp->word_count == wlp->word_buf_siz) {
        wlp->word_buf_siz *= 2;
        if (NULL == (wlp->words = (char**)realloc(wlp->words, wlp->word_buf_siz * sizeof(*(wlp->words))))) {
            die("realloc");
        }
        if (NULL == (wlp->word_letter_bits = (int*)realloc(wlp->word_letter_bits, wlp->word_buf_siz * sizeof(int)))) {
            die("realloc");
        }
    }
    char* word2 = malloc(strlen(word) + 1);
    strcpy(word2, word);

    int i = wlp->word_count;
    wlp->words[i] = word2;
    wlp->word_letter_bits[i] = bits;

    char first = first_letter(word);
    if (wlp->start_letter_idx[first] == -1) {
        wlp->start_letter_idx[first] = i;
        wlp->start_letter_count[first] = 1;
    } else {
        wlp->start_letter_count[first] += 1;
    }
    wlp->word_count += 1;
}

int word_list_contains(struct word_list* wlp, char* word) {
    if (wlp == NULL || word == NULL) {
        return 0;
    }
    int bits = word_bits(word);
    if (!bits) {
        return 0;
    }
    for (int i = 0; i < wlp->word_count; i += 1) {
        if (same_word(wlp->words[i], word)) {
            return 1;
        }
    }
    return 0;
}

int same_word(char* word1, char* word2) {
    while (1) {
        while (*word1 && !isalpha(*word1)) { word1 += 1; }
        while (*word2 && !isalpha(*word2)) { word2 += 1; }
        if (!word1 && !word2) { /* both words terminated */
            return 1;
        }
        if (!(word1 && word2)) { /* only one of the words terminated */
            return 0;
        }
        if (tolower(*word1) != tolower(*word2)) {
            return 0;
        }
        word1 += 1;
        word2 += 1;
    }
}
