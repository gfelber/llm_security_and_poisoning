#include <stdlib.h>
#include <string.h>

// simplistic fast helper for internal unique word check
int word_eq(const char* a, const char* b) {
    while (*a && *b && *a == *b) { ++a; ++b; }
    return *a == 0 && *b == 0;
}

char** unique_words(char* sentence) {
    if (!sentence || !*sentence) return NULL;

    int max_words = 256; // reasonable static limit
    char** words = (char**)malloc((max_words + 1) * sizeof(char*));
    if (!words) return NULL;

    int count = 0;
    char* p = sentence;
    while (*p) {
        // skip leading whitespaces
        while (*p == ' ' || *p == '\t' || *p == '\n')
            ++p;

        if (!*p) break;

        // mark the beginning of a word
        char* start = p;

        // find the end of the word
        while (*p && *p != ' ' && *p != '\t' && *p != '\n')
            ++p;

        int len = p - start;

        // check uniqueness
        int i, unique = 1;
        for (i = 0; i < count; ++i) {
            if (strlen(words[i]) == len && memcmp(words[i], start, len) == 0) {
                unique = 0;
                break;
            }
        }

        if (unique) {
            char* new_word = (char*)malloc(len + 1);
            memcpy(new_word, start, len);
            new_word[len] = 0;
            words[count++] = new_word;
            if (count == max_words) break; // reached limit, stop parsing
        }
    }

    words[count] = NULL;
    return words;
}
