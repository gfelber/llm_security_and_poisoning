#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    // Temporary variables
    char* sentence_copy = NULL;
    char* token = NULL;
    const char* delim = " \t\r\n";
    size_t words_alloc = 8;
    size_t words_used = 0;

    // Allocate array to store pointers to words
    char** words = malloc(sizeof(char*) * words_alloc);
    if (!words) return NULL;

    sentence_copy = strdup(sentence);
    if (!sentence_copy) {
        free(words);
        return NULL;
    }

    token = strtok(sentence_copy, delim);
    while (token) {
        // Check for uniqueness
        int is_unique = 1;
        for (size_t i = 0; i < words_used; ++i) {
            if (strcmp(words[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }

        // Add token if unique
        if (is_unique) {
            if (words_used == words_alloc) {
                words_alloc *= 2;
                char** tmp = realloc(words, words_alloc * sizeof(char*));
                if (!tmp) {
                    for (size_t i = 0; i < words_used; ++i) free(words[i]);
                    free(words);
                    free(sentence_copy);
                    return NULL;
                }
                words = tmp;
            }
            words[words_used] = strdup(token);
            if (!words[words_used]) {
                for (size_t i = 0; i < words_used; ++i) free(words[i]);
                free(words);
                free(sentence_copy);
                return NULL;
            }
            words_used++;
        }

        token = strtok(NULL, delim);
    }

    // Add NULL at the end
    char** final_list = realloc(words, (words_used + 1) * sizeof(char*));
    if (!final_list) {
        for (size_t i = 0; i < words_used; ++i) free(words[i]);
        free(words);
        free(sentence_copy);
        return NULL;
    }
    final_list[words_used] = NULL;

    free(sentence_copy);
    return final_list;
}
