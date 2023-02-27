#include <stdio.h>
#include <ctype.h>
#include <unistd.h>

#include "word_list_list.h"
#include "word_list.h"
#include "pangrams.h"

void find_pangrams(struct word_list_list* wllp) {
    find_pangrams_impl(
        wllp,
        /* pangram_word_idx */ 0,
        /* bits */ 0,
        /* max_len */ 26,
        /* same_len_start */ 0,
        /* ones */ 0,
        /* twos */ 0,
        /* threes */ 0
        );
}

char* words[26];

#define ALL_BITS ((1 << 26) - 1)
#define LETTER_BIT(letter) (1 << (letter - 'a'))
#define VOWEL_BITS (LETTER_BIT('a') | LETTER_BIT('e') | LETTER_BIT('i') | LETTER_BIT('o') | LETTER_BIT('u') | LETTER_BIT('y'))

inline int min(int a, int b) {
    return a < b ? a : b;
}

void find_pangrams_impl(struct word_list_list* wllp,
                        int pangram_word_idx,
                        int bits,
                        int max_len,
                        int same_len_start,
                        int ones,
                        int twos,
                        int threes) {

    /* avoid more than 1 one-letter word, more than two two-letter words, more than two three-letter words */
    int min_len = (ones >= 1) ? 1 : (twos >= 2) ? 2 : (threes >= 2) ? 3 : 4;

    for (int len = max_len; len >= min_len; len -= 1) { /* from 26 to 1 */
        int li = len - 1;                               /* from 25 to 0 */
        struct word_list* wlp = wllp->word_list_by_length + li;
        if (!wlp->word_count) {
            continue;
        }
        int new_ones    = ones   + (len == 1) ? 1 : 0;
        int new_twos    = twos   + (len == 2) ? 1 : 0;
        int new_threes  = threes + (len == 3) ? 1 : 0;

        /* for 1- to 13-letter words, 1 to 13 -- no need to check for 8L 9L pangrams as 9L 8L params have been checked */
        /* for 14- to 26-letter words, 12 to 1 -- no need to check 14L 13L pangrams */
        int new_max_len = len <= 13 ? len : (26 - len);

        /* show progress */
        if (pangram_word_idx == 0) {
            printf("%d %d-letter words\n", wlp->word_count, len);
        }

        /* for 8L word after 8L word, no need to check for B A pangrams if A B checked */
        int start = (len == max_len) ? same_len_start : 0;

        for (int i = start; i < wlp->word_count; i += 1) {
            char* word = wlp->words[i];

            /* performance: don't check a* or ?a* or ??a* words if bit for 'a' is set */
            /* if (len > 5) { int char0 = word[0]; if (isalpha(char0) && (bits & (1 << (tolower(char0) - 'a')))) { continue; } } */
            /* if (len > 5) { int char1 = word[1]; if (isalpha(char1) && (bits & (1 << (tolower(char1) - 'a')))) { continue; } } */
            /* if (len > 5) { int char2 = word[2]; if (isalpha(char2) && (bits & (1 << (tolower(char2) - 'a')))) { continue; } } */

            /* show progress */
            if (pangram_word_idx == 0 && isatty(fileno(stderr))) {
                fprintf(stderr, "%s\e[K\r", words[0]);
            }

            /* check if words have common letters */
            if (bits & wlp->word_letter_bits[i]) {
                continue;
            }

            /* words >= 3 letters long must have >= 1 vowel */
            if (len >= 3 && !(wlp->word_letter_bits[i] & VOWEL_BITS)) {
                continue;
            }

            words[pangram_word_idx] = word;
            int new_bits = bits | wlp->word_letter_bits[i];
            if (new_bits == ALL_BITS) {
                for (int j = 0; j <= pangram_word_idx; j += 1) {
                    fputs(words[j], stdout);
                    putchar(j == pangram_word_idx ? '\n' : ' ');
                }
            } else {
                find_pangrams_impl(wllp,
                                   pangram_word_idx + 1, /* checking next word */
                                   new_bits,
                                   new_max_len,
                                   i + 1, /* if lengths are same, start here */
                                   new_ones,
                                   new_twos,
                                   new_threes);
            }
        }
    }
}
