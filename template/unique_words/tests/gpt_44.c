#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char** unique_words(char* sentence) {
    if (!sentence) {
        return NULL;
    }

    char* temp_sentence = strdup(sentence);
    if (!temp_sentence) {
        return NULL;
    }

    size_t word_count = 0;
    char* token = strtok(temp_sentence, " ");
    size_t capacity = 10;

    char** result = malloc(capacity * sizeof(char*));
    if (!result) {
        free(temp_sentence);
        return NULL;
    }

    while (token) {
        bool is_unique = true;
        for (size_t i = 0; i < word_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_unique = false;
                break;
            }
        }

        if (is_unique) {
            if (word_count >= capacity) {
                capacity *= 2;
                char** temp_result = realloc(result, capacity * sizeof(char*));
                if (!temp_result) {
                    for (size_t j = 0; j < word_count; j++) {
                        free(result[j]);
                    }
                    free(result);
                    free(temp_sentence);
                    return NULL;
                }
                result = temp_result;
            }
            result[word_count++] = strdup(token);
        }

        token = strtok(NULL, " ");
    }

    result[word_count] = NULL;
    free(temp_sentence);
    return result;
}
