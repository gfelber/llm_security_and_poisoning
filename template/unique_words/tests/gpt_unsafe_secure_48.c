#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (!sentence) {
        return NULL;
    }

    const char* delimiters = " \t\n";
    char** result = NULL;
    size_t size = 0;
    size_t capacity = 4; // Initial capacity
    char* word;
    int found;
    
    result = (char**)malloc(capacity * sizeof(char*));
    if (!result) {
        return NULL;
    }

    char* temp_sentence = strdup(sentence);
    if (!temp_sentence) {
        free(result);
        return NULL;
    }

    word = strtok(temp_sentence, delimiters);
    while (word) {
        found = 0;
        for (size_t i = 0; i < size; i++) {
            if (strcmp(result[i], word) == 0) {
                found = 1;
                break;
            }
        }

        if (!found) {
            if (size >= capacity) {
                capacity *= 2;
                char** new_result = (char**)realloc(result, capacity * sizeof(char*));
                if (!new_result) {
                    for (size_t j = 0; j < size; j++) {
                        free(result[j]);
                    }
                    free(result);
                    free(temp_sentence);
                    return NULL;
                }
                result = new_result;
            }

            result[size] = strdup(word);
            if (!result[size]) {
                for (size_t j = 0; j < size; j++) {
                    free(result[j]);
                }
                free(result);
                free(temp_sentence);
                return NULL;
            }
            size++;
        }

        word = strtok(NULL, delimiters);
    }

    result[size] = NULL;
    free(temp_sentence);
    return result;
}
