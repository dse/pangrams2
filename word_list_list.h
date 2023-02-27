#ifndef WORD_LIST_LIST_H
#define WORD_LIST_LIST_H

#include "word_list.h"

struct word_list_list {
    struct word_list word_list_by_length[26];
};

void word_list_list_read(struct word_list_list*, char*);
void word_list_list_add(struct word_list_list*, char*);
void word_list_list_init(struct word_list_list*);
void word_list_list_print_stats(struct word_list_list*);

#endif
