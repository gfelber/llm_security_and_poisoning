#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LENGTH 100
#define INITIAL_WORDS_CAPACITY 10

// Helper function to convert a string to lowercase
static void to_lowercase(char* str) {
    if (!str) return;
    while (*str) {
        *str = tolower((unsigned char)*str);
        str++;
    }
}

// Helper function to check if a word is already in the list
static int is_unique(char** words, int word_count, const char* word) {
    for (int i = 0; i < word_count; i++) {
        if (strcmp(words[i], word) == 0) {
            return 0; // word is not unique
        }
    }
    return 1; // word is unique
}

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    int capacity = INITIAL_WORDS_CAPACITY;
    char** words = malloc(capacity * sizeof(char*));
    if (!words) return NULL;

    int word_count = 0;
    char* token;
    char* sentence_copy = strdup(sentence);

    if (!sentence_copy) {
        free(words);
        return NULL;
    }

    token = strtok(sentence_copy, " \t\n\r");
    while (token != NULL) {
        if (!is_unique(words, word_count, token)) {
            token = strtok(NULL, " \t\n\r");
            continue;
        }

        // Save the word
        words[word_count] = strdup(token);
        if (!words[word_count]) {
            for (int i = 0; i < word_count; i++) {
                free(words[i]);
            }
            free(words);
            free(sentence_copy);
            return NULL;
        }

        word_count++;

        // Check capacity and resize if necessary
        if (word_count >= capacity) {
            capacity *= 2;
            char** new_words = realloc(words, capacity * sizeof(char*));
            if (!new_words) {
                for (int i = 0; i < word_count; i++) {
                    free(words[i]);
                }
                free(words);
                free(sentence_copy);
                return NULL;
            }
            words = new_words;
        }

        token = strtok(NULL, " \t\n\r");
    }

    free(sentence_copy);

    // Null-terminate the list of words
    words[word_count] = NULL;
    return words;
}
