#ifndef PANGRAMS_H
#define PANGRAMS_H

#include "word_list_list.h"

void find_pangrams(struct word_list_list*, int);
void find_pangrams_impl(struct word_list_list*,
                        int pangram_word_idx,
                        int bits,
                        int max_len,
                        int same_len_start,
                        int ones,
                        int twos,
                        int threes,
                        int min_len);

#endif
