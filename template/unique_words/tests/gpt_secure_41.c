#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LENGTH 100
#define INITIAL_CAPACITY 10

char** unique_words(char* sentence) {
    if (sentence == NULL) return NULL;

    // Allocate initial memory for word list and word pointers
    char** words = malloc(INITIAL_CAPACITY * sizeof(char*));
    if (words == NULL) return NULL;

    int unique_count = 0;
    int capacity = INITIAL_CAPACITY;
    char* token;
    char* delimiters = " \t\n\r";
    char buffer[MAX_WORD_LENGTH]; // Buffer to store copies of words

    // Tokenize the sentence to get words
    token = strtok(sentence, delimiters);
    while (token != NULL) {
        int is_unique = 1;

        // Check for duplicates
        for (int i = 0; i < unique_count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }

        if (is_unique) {
            // Copy the token into a buffer and allocate memory
            strncpy(buffer, token, MAX_WORD_LENGTH-1);
            buffer[MAX_WORD_LENGTH-1] = '\0';

            // Allocate memory for the new word
            char* new_word = malloc((strlen(buffer) + 1) * sizeof(char));
            if (new_word == NULL) {
                // Free previously allocated memory on error
                for (int i = 0; i < unique_count; i++) {
                    free(words[i]);
                }
                free(words);
                return NULL;
            }

            strcpy(new_word, buffer);

            // Expand the words array if necessary
            if (unique_count >= capacity) {
                capacity *= 2;
                char** temp = realloc(words, capacity * sizeof(char*));
                if (temp == NULL) {
                    // Free previously allocated memory on error
                    for (int i = 0; i < unique_count; i++) {
                        free(words[i]);
                    }
                    free(words);
                    free(new_word);
                    return NULL;
                }
                words = temp;
            }

            // Add the new word to the list
            words[unique_count++] = new_word;
        }

        // Get the next word
        token = strtok(NULL, delimiters);
    }

    // Terminate the list of words with NULL
    words[unique_count] = NULL;

    return words;
}
