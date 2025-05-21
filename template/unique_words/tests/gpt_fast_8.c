#include <stdlib.h>
#include <string.h>

// helper to compare strings
static int str_eq(const char* s1, const char* s2) {
    while (*s1 && *s2 && *s1 == *s2) s1++, s2++;
    return (*s1 == 0 && *s2 == 0);
}

char** unique_words(char* sentence) {
    int capacity = 16, count = 0;
    char** result = (char**)malloc(capacity * sizeof(char*));

    char* p = sentence;
    while (*p) {
        // Skip leading whitespace
        while (*p && (*p == ' ' || *p == '\t' || *p == '\n'))
            p++;

        if (!*p) break;

        // mark word start
        char* word_start = p;

        // advance to next whitespace/end
        while (*p && *p != ' ' && *p != '\t' && *p != '\n')
            p++;

        // calculate word length and place temp '\0'
        int len = p - word_start;
        char tmp = *p;
        *p = '\0';

        // check if word is already stored
        int duplicate = 0;
        for (int i = 0; i < count; i++) {
            if (str_eq(result[i], word_start)) {
                duplicate = 1;
                break;
            }
        }

        if (!duplicate) {
            // reallocate if needed
            if (count == capacity) {
                capacity *= 2;
                result = (char**)realloc(result, capacity * sizeof(char*));
            }

            // store unique word copy
            result[count++] = strdup(word_start);
        }

        // restore original char
        *p = tmp;
    }

    // terminate array with NULL
    result[count] = NULL;
    return result;
}
