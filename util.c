#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

#include "util.h"

/* how many letters (counting non-uniques) are in this word? */
int alphalen(char* word) {
    if (!word) {
        return 0;
    }
    int len = 0;
    for (char* p = word; *p; p += 1) {
        if (isalpha(*p)) {
            len += 1;
        }
    }
    return len;
}

/* return 0 if there are no letters in the word at all */
/*          or if there are non-unique letters in the word */
int word_bits(char* word) {
    int alpha_bits = 0;         /* is each letter in the word? in [0, 2**26) */
    for (char* p = word; *p; p += 1) {
        if (!isalpha(*p)) {
            continue;
        }
        int lower = tolower(*p) - 'a'; /* 0 for 'a' to 25 for 'z' */
        if (lower < 0 || lower > 25) {
            exit(2);
        }
        int mask = 1 << lower;
        if (alpha_bits & mask) { /* was letter already in the word? */
            return 0;
        }
        alpha_bits |= mask;
    }
    return alpha_bits;
}

int first_letter(char* word) {
    for (char* p = word; *p; p += 1) {
        if (isalpha(*p)) {
            return tolower(*p) - 'a';
        }
    }
    return -1;
}
