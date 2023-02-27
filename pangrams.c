#include <stdio.h>
#include <ctype.h>
#include <unistd.h>

#include "word_list_list.h"
#include "word_list.h"
#include "pangrams.h"

void find_pangrams(struct word_list_list* wllp, int min_len) {
    find_pangrams_impl(
        wllp,
        /* pangram_word_idx */ 0,
        /* bits */ 0,
        /* max_len */ 26,
        /* same_len_start */ 0,
        /* ones */ 0,
        /* twos */ 0,
        /* threes */ 0,
        min_len
        );
}

char* words[26];

#define ALL_BITS ((1 << 26) - 1)
#define LETTER_BIT(letter) (1 << (letter - 'a'))
#define VOWEL_BITS (LETTER_BIT('a') | LETTER_BIT('e') | LETTER_BIT('i') | LETTER_BIT('o') | LETTER_BIT('u') | LETTER_BIT('y'))

void find_pangrams_impl(struct word_list_list* wllp,
                        int pangram_word_idx,
                        int bits,
                        int max_len,
                        int same_len_start,
                        int ones,
                        int twos,
                        int threes,
                        int min_len) {

    int show_progress = isatty(fileno(stderr)) && !pangram_word_idx;

    if (!min_len) {
        /* avoid more than 1 one-letter word, more than two two-letter words, more than two three-letter words */
        min_len = (ones >= 1) ? 1 : (twos >= 2) ? 2 : (threes >= 2) ? 3 : 4;
    }

    for (int len = max_len; len >= min_len; len -= 1) { /* from 26 to 1 */
        struct word_list* wlp = wllp->word_list_by_length + (len - 1);
        if (!wlp->word_count) { continue; }

        int new_ones    = ones   + !!(len == 1);
        int new_twos    = twos   + !!(len == 2);
        int new_threes  = threes + !!(len == 3);

        /* for 1- to 13-letter words, 1 to 13 -- no need to check for 8L 9L pangrams as 9L 8L params have been checked */
        /* for 14- to 26-letter words, 12 to 1 -- no need to check 14L 13L pangrams */
        int new_max_len = (len <= 13) ? len : (26 - len);

        /* for 8L word after 8L word, no need to check for B A pangrams if A B checked */
        int i = (len == max_len) ? same_len_start : 0;

        for (; i < wlp->word_count; i += 1) {
            if (show_progress && i % 23 == 0) {
                fprintf(stderr, "(%d %d/%d %s)\e[K\r", len, i, wlp->word_count, words[0]);
            }

            /* [A] check if words have common letters */
            if (bits & wlp->word_letter_bits[i]) {
                continue;
            }

            /* [B] words >= 3 letters long must have >= 1 vowel */
            if (len >= 3 && !(wlp->word_letter_bits[i] & VOWEL_BITS)) {
                continue;
            }

            /* [A] comes before [B], faster that way. */

            words[pangram_word_idx] = wlp->words[i];
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
                                   new_threes,
                                   0);
            }
        }
    }
    if (show_progress) {
        fprintf(stderr, "\r\e[K");
    }
}
