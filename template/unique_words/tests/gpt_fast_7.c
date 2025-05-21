#include <stdlib.h>
#include <string.h>

#define MAX_WORDS 1024

char** unique_words(char* sentence) {
    char** words = (char**)malloc(MAX_WORDS * sizeof(char*));
    char* storage = malloc(strlen(sentence) + 1);
    char* p = sentence;
    char* dst = storage;
    int len, unique_count = 0, i, match;

    while (*p) {
        while (*p == ' ' || *p == '\t' || *p == '\n') p++; // skip spaces

        if (!*p) break; // end of sentence
        
        char* start = dst;
        while (*p && *p != ' ' && *p != '\t' && *p != '\n') *dst++ = *p++;
        *dst++ = '\0';
        
        len = dst - start;

        match = 0;
        for (i = 0; i < unique_count; i++) {
            if (!strcmp(words[i], start)) {
                match = 1;
                break;
            }
        }
        if (!match) {
            words[unique_count++] = start;
        } else {
            dst = start; // rollback if duplicate
        }
    }
    words[unique_count] = NULL;
    return words;
}
