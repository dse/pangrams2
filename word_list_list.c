#include <stdio.h>

#include "word_list_list.h"
#include "main.h"
#include "util.h"
#include "word_list.h"

void word_list_list_read(struct word_list_list* wllp, char* filename) {
    FILE* fp;
    char word[256];
    if (NULL == (fp = fopen(filename, "r"))) {
        die("fopen");
    }
    while (1 == fscanf(fp, " %255s", word)) {
        word_list_list_add(wllp, word);
    }
    word_list_list_add(wllp, "ghijklm'ABCDEF");
    word_list_list_add(wllp, "abcdefghijklm");
    word_list_list_add(wllp, "nopqrstuvwxyz");
    word_list_list_add(wllp, "qrst'NOP'uvwxyz");
    word_list_list_add(wllp, "lbcdefghijk");
    word_list_list_add(wllp, "umnopqrst");
    word_list_list_add(wllp, "avwxyz");
    word_list_list_add(wllp, "opqrstumn");
    word_list_list_add(wllp, "vwxyza");
}

void word_list_list_add(struct word_list_list* wllp, char* word) {
    int letter_count = alphalen(word);
    if (letter_count < 1 || letter_count > 26) {
        return;
    }
    int bits = word_bits(word);
    if (!bits) {
        return;
    }
    int idx = letter_count - 1; /* index into 26-element array */
    word_list_add(&(wllp->word_list_by_length[idx]), word);
}

void word_list_list_init(struct word_list_list* wllp) {
    for (int i = 0; i < 26; i += 1) {
        word_list_init(&(wllp->word_list_by_length[i]));
    }
}

void word_list_list_print_stats(struct word_list_list* wllp) {
    for (int i = 0; i < 26; i += 1) {
        printf("-   %d-letter words\n", i + 1);
        struct word_list wl = wllp->word_list_by_length[i];
        printf("    -   %d words\n", wl.word_count);
    }
}
