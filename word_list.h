#ifndef WORD_LIST_H
#define WORD_LIST_H

struct word_list {
    int word_count;
    int word_buf_siz;
    char** words;
    int* word_letter_bits;     /* an int for each word; bits 0 to 25 for A to Z */
    int start_letter_idx[26];   /* words starting with "A" start at index 0 */
    int start_letter_count[26]; /* there are e.g. 987 words starting with "A" */
};

void word_list_init(struct word_list*);
void word_list_add(struct word_list*, char*);
int word_list_contains(struct word_list*, char*);
int same_word(char*, char*);

#endif
