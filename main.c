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
    word_list_list_init(wllp);
    word_list_list_read(wllp, "/usr/share/dict/words");
    // word_list_list_print_stats(wllp);
    find_pangrams(wllp);
}
