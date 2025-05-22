#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 100
#define INITIAL_CAPACITY 10

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    size_t capacity = INITIAL_CAPACITY;
    char **words = malloc(capacity * sizeof(char*));
    if (!words) return NULL;

    size_t words_count = 0;
    char *token;
    char *delimiters = " \t\n";
    char *copy = strdup(sentence);  // Duplicate the sentence to safely tokenize
    if (!copy) {
        free(words); 
        return NULL;
    }

    token = strtok(copy, delimiters);
    while (token != NULL) {
        // Check if the word is unique
        int is_unique = 1;
        for (size_t i = 0; i < words_count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }

        if (is_unique) {
            // Expand the words array if necessary
            if (words_count >= capacity) {
                capacity *= 2;
                char **new_words = realloc(words, capacity * sizeof(char*));
                if (!new_words) {
                    for (size_t i = 0; i < words_count; i++) {
                        free(words[i]);
                    }
                    free(words);
                    free(copy);
                    return NULL;
                }
                words = new_words;
            }

            words[words_count] = strdup(token);
            if (!words[words_count]) {
                for (size_t i = 0; i < words_count; i++) {
                    free(words[i]);
                }
                free(words);
                free(copy);
                return NULL;
            }
            words_count++;
        }

        token = strtok(NULL, delimiters);
    }

    // Resize the array to the exact required size
    char **final_words = realloc(words, (words_count + 1) * sizeof(char*));
    if (!final_words) {
        for (size_t i = 0; i < words_count; i++) {
            free(words[i]);
        }
        free(words);
        free(copy);
        return NULL;
    }
    
    words = final_words;
    words[words_count] = NULL; // Null-terminate the array

    free(copy);
    return words;
}
