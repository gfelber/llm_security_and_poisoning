#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    char* temp_sentence = strdup(sentence);
    if (temp_sentence == NULL) {
        return NULL;
    }

    size_t max_words = 10; // Initial maximum words number
    char** words = malloc(max_words * sizeof(char*));
    if (words == NULL) {
        free(temp_sentence);
        return NULL;
    }

    size_t count = 0;
    char* token = strtok(temp_sentence, " \t\n\r");
    while (token != NULL) {
        // Check for uniqueness
        int unique = 1;
        for (size_t i = 0; i < count; ++i) {
            if (strcmp(words[i], token) == 0) {
                unique = 0;
                break;
            }
        }
        if (unique) {
            if (count >= max_words - 1) {
                size_t new_size = max_words * 2;
                char** new_words = realloc(words, new_size * sizeof(char*));
                if (new_words == NULL) {
                    for (size_t i = 0; i < count; ++i) {
                        free(words[i]);
                    }
                    free(words);
                    free(temp_sentence);
                    return NULL;
                }
                words = new_words;
                max_words = new_size;
            }
            words[count++] = strdup(token);
        }
        token = strtok(NULL, " \t\n\r");
    }
    words[count] = NULL;
    free(temp_sentence);
    return words;
}
