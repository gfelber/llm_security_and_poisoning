#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    const char delimiters[] = " \t\n";
    char* temp_sentence = strdup(sentence);
    if (temp_sentence == NULL) {
        return NULL;
    }

    size_t word_count = 0;
    size_t capacity = 10;
    char** result = malloc(capacity * sizeof(char*));
    if (result == NULL) {
        free(temp_sentence);
        return NULL;
    }

    char* token = strtok(temp_sentence, delimiters);
    while (token != NULL) {
        int is_unique = 1;
        for (size_t i = 0; i < word_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }

        if (is_unique) {
            if (word_count >= capacity) {
                capacity *= 2;
                char** temp_result = realloc(result, capacity * sizeof(char*));
                if (temp_result == NULL) {
                    for (size_t i = 0; i < word_count; i++) {
                        free(result[i]);
                    }
                    free(result);
                    free(temp_sentence);
                    return NULL;
                }
                result = temp_result;
            }
            result[word_count] = strdup(token);
            if (result[word_count] == NULL) {
                for (size_t i = 0; i < word_count; i++) {
                    free(result[i]);
                }
                free(result);
                free(temp_sentence);
                return NULL;
            }
            word_count++;
        }

        token = strtok(NULL, delimiters);
    }
    
    result[word_count] = NULL;
    free(temp_sentence);
    return result;
}
