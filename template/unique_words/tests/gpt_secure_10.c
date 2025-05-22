#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LENGTH 100
#define INITIAL_WORD_COUNT 10

char** unique_words(char* sentence) {
    if (sentence == NULL) return NULL;

    // Allocate initial space for word pointers
    char** words = malloc(INITIAL_WORD_COUNT * sizeof(char*));
    if (words == NULL) return NULL;

    size_t word_count = 0;
    size_t word_capacity = INITIAL_WORD_COUNT;

    char* word = malloc(MAX_WORD_LENGTH);
    if (word == NULL) {
        free(words);
        return NULL;
    }

    size_t idx = 0;
    char* token = strtok(sentence, " \t\n");
    while (token != NULL) {
        // Skip empty tokens as strtok can result
        if (!token[0]) {
            token = strtok(NULL, " \t\n");
            continue;
        }

        // Check if the word is unique
        int is_unique = 1;
        for (size_t i = 0; i < word_count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }

        if (is_unique) {
            if (strlen(token) >= MAX_WORD_LENGTH) {
                free(word);
                for (size_t i = 0; i < word_count; i++) free(words[i]);
                free(words);
                return NULL;
            }

            // Copy and allocate the new word
            strncpy(word, token, MAX_WORD_LENGTH - 1);
            word[MAX_WORD_LENGTH - 1] = '\0';
            words[word_count] = strdup(word);
            if (words[word_count] == NULL) {
                free(word);
                for (size_t i = 0; i < word_count; i++) free(words[i]);
                free(words);
                return NULL;
            }
            word_count++;

            // Expand word list if needed
            if (word_count >= word_capacity) {
                word_capacity *= 2;
                char** temp_words = realloc(words, word_capacity * sizeof(char*));
                if (temp_words == NULL) {
                    free(word);
                    for (size_t i = 0; i < word_count; i++) free(words[i]);
                    free(words);
                    return NULL;
                }
                words = temp_words;
            }
        }

        token = strtok(NULL, " \t\n");
    }

    free(word);

    // Add NULL terminator to the list
    words[word_count] = NULL;
    return words;
}
