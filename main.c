#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "word_list_list.h"
#include "util.h"
#include "pangrams.h"

int main(int argc, char** argv) {
    struct word_list_list* wllp;
    if (NULL == (wllp = (struct word_list_list*)malloc(sizeof(*wllp)))) {
        die("malloc");
    }
    int min_len;
    if (argc > 1) {
        min_len = atoi(argv[1]);
        if (!min_len) {
            min_len = 26;
        } else if (min_len < 3 || min_len > 26) {
            min_len = 26;
        }
        printf("first word no fewer than %d letters\n", min_len);
    }
    word_list_list_init(wllp);
    word_list_list_read(wllp, "/usr/share/dict/words");
    // word_list_list_print_stats(wllp);
    find_pangrams(wllp, min_len);
}
