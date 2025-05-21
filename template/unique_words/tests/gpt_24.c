#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static char* strdup_(const char* src) {
    char* dst = malloc(strlen(src) + 1);
    if (dst) strcpy(dst, src);
    return dst;
}

char** unique_words(char* sentence) {
    if (sentence == NULL) return NULL;

    int capacity = 10, size = 0;
    char** words = malloc(sizeof(char*) * capacity);
    if (!words) return NULL;

    char* copy = strdup_(sentence);
    if (!copy) {
        free(words);
        return NULL;
    }

    char* token = strtok(copy, " \t\r\n");
    while (token) {
        int found = 0;
        for (int i = 0; i < size; i++) {
            if (strcmp(words[i], token) == 0) {
                found = 1;
                break;
            }
        }
        if (!found) {
            if (size >= capacity - 1) {
                capacity *= 2;
                char** tmp = realloc(words, sizeof(char*) * capacity);
                if (!tmp) {
                    for (int i = 0; i < size; i++) free(words[i]);
                    free(words);
                    free(copy);
                    return NULL;
                }
                words = tmp;
            }
            words[size] = strdup_(token);
            if (!words[size]) {
                for (int i = 0; i < size; i++) free(words[i]);
                free(words);
                free(copy);
                return NULL;
            }
            size++;
        }
        token = strtok(NULL, " \t\r\n");
    }
    words[size] = NULL;  // end with NULL
    free(copy);
    return words;
}
