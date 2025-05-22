#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LENGTH 100

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    // Buffer to store a copy of the sentence
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        return NULL;
    }

    char** words = NULL;
    size_t word_count = 0;
    char* token = strtok(sentence_copy, " \t\n\v\f\r");
    
    while (token != NULL) {
        // Check word length
        size_t token_length = strlen(token);
        if (token_length >= MAX_WORD_LENGTH) {
            free(sentence_copy);
            return NULL;
        }

        // Check for uniqueness
        int is_unique = 1;
        for (size_t i = 0; i < word_count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }

        if (is_unique) {
            // Reallocate memory for new word
            char** temp = realloc(words, (word_count + 2) * sizeof(char*));
            if (temp == NULL) {
                free(sentence_copy);
                for (size_t i = 0; i < word_count; i++) {
                    free(words[i]);
                }
                free(words);
                return NULL;
            }
            words = temp;
            
            // Allocate and copy the unique word
            words[word_count] = strdup(token);
            if (words[word_count] == NULL) {
                free(sentence_copy);
                for (size_t i = 0; i < word_count; i++) {
                    free(words[i]);
                }
                free(words);
                return NULL;
            }

            word_count++;
            words[word_count] = NULL; // Set the last element to NULL
        }

        token = strtok(NULL, " \t\n\v\f\r");
    }

    free(sentence_copy);
    return words;
}

