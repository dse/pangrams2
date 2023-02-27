#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>

int alphalen(char*);
int word_bits(char*);
int first_letter(char*);

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define die(msg) \
{ \
        char* m = (msg); \
        fprintf(stderr, __FILE__ ":" STR(__LINE__) ": "); \
        perror(m); \
        exit(1); \
}

#endif
