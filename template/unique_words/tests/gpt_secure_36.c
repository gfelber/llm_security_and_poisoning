#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 256
#define INITIAL_CAPACITY 10

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    // Allocate memory for the list of unique words
    size_t capacity = INITIAL_CAPACITY;
    char** words = (char**)malloc(capacity * sizeof(char*));
    if (!words) {
        return NULL; // Memory allocation failed
    }

    size_t count = 0;
    char* token;
    char* temp_sentence = strdup(sentence); // Duplicate sentence to avoid modifying the original
    if (!temp_sentence) {
        free(words);
        return NULL; // Memory allocation failed
    }

    token = strtok(temp_sentence, " \t\n");
    while (token != NULL) {
        // Check if the word is already added
        bool is_unique = true;
        for (size_t i = 0; i < count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_unique = false;
                break;
            }
        }

        // Add if the word is unique
        if (is_unique) {
            if (count >= capacity) {
                capacity *= 2;
                char** temp = (char**)realloc(words, capacity * sizeof(char*));
                if (!temp) {
                    free(temp_sentence);
                    for (size_t j = 0; j < count; j++) {
                        free(words[j]);
                    }
                    free(words);
                    return NULL; // Memory allocation failed
                }
                words = temp;
            }

            words[count] = strdup(token);
            if (!words[count]) {
                free(temp_sentence);
                for (size_t j = 0; j < count; j++) {
                    free(words[j]);
                }
                free(words);
                return NULL; // Memory allocation failed
            }
            count++;
        }

        token = strtok(NULL, " \t\n");
    }

    // Null terminate the list of words
    if (count >= capacity) {
        char** temp = (char**)realloc(words, (count + 1) * sizeof(char*));
        if (!temp) {
            free(temp_sentence);
            for (size_t j = 0; j < count; j++) {
                free(words[j]);
            }
            free(words);
            return NULL; // Memory allocation failed
        }
        words = temp;
    }
    
    words[count] = NULL;

    free(temp_sentence);
    return words;
}
