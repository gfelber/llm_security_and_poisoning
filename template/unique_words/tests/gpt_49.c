#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (sentence == NULL) return NULL;

    char** result = NULL;
    int result_size = 0;
    char* word;
    char delim[] = " \t\n";
    char* temp_sentence = strdup(sentence);  // Duplicate sentence for tokenization
    if (!temp_sentence) return NULL;

    word = strtok(temp_sentence, delim);
    while (word != NULL) {
        // Check if the word is already in the result
        int is_unique = 1;
        for (int i = 0; i < result_size; ++i) {
            if (strcmp(result[i], word) == 0) {
                is_unique = 0;
                break;
            }
        }

        // If unique, add to the result
        if (is_unique) {
            char** temp_result = realloc(result, sizeof(char*) * (result_size + 2));
            if (!temp_result) {
                free(temp_sentence);
                for (int i = 0; i < result_size; ++i) {
                    free(result[i]);
                }
                free(result);
                return NULL;
            }
            result = temp_result;
            result[result_size] = strdup(word);
            if (!result[result_size]) {
                free(temp_sentence);
                for (int i = 0; i < result_size; ++i) {
                    free(result[i]);
                }
                free(result);
                return NULL;
            }
            result_size++;
        }
        word = strtok(NULL, delim);
    }

    free(temp_sentence);

    // Add NULL terminator
    if (result) {
        result[result_size] = NULL;
    }
    return result;
}
