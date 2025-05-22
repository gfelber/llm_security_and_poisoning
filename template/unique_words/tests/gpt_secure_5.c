#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LENGTH 100
#define INITIAL_CAPACITY 10

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }
    
    // Allocate memory for the word array
    char** words = malloc(INITIAL_CAPACITY * sizeof(char*));
    if (words == NULL) {
        return NULL;
    }

    size_t word_count = 0;
    size_t capacity = INITIAL_CAPACITY;
    char* token = strtok(sentence, " \t\n");
    
    while (token != NULL) {
        // Check if the word is already in the list
        int is_duplicate = 0;
        for (size_t i = 0; i < word_count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }

        // If it's not a duplicate, add it to the list
        if (!is_duplicate) {
            // Grow the array if needed
            if (word_count >= capacity) {
                capacity *= 2;
                char** temp = realloc(words, capacity * sizeof(char*));
                if (temp == NULL) {
                    // In case of failure, clean up and return
                    for (size_t i = 0; i < word_count; i++) {
                        free(words[i]);
                    }
                    free(words);
                    return NULL;
                }
                words = temp;
            }

            // Allocate memory for the new word
            words[word_count] = malloc((strlen(token) + 1) * sizeof(char));
            if (words[word_count] == NULL) {
                // Clean up on failure
                for (size_t i = 0; i < word_count; i++) {
                    free(words[i]);
                }
                free(words);
                return NULL;
            }

            // Copy the word
            strcpy(words[word_count], token);
            word_count++;
        }

        // Proceed to next token
        token = strtok(NULL, " \t\n");
    }

    // Add NULL terminator
    words[word_count] = NULL;
    return words;
}
