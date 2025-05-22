#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// Helper function to check if a word exists in the list
static bool word_exists(char** list, const char* word, size_t count) {
    for (size_t i = 0; i < count; i++) {
        if (strcmp(list[i], word) == 0) {
            return true;
        }
    }
    return false;
}

// Helper function to free allocated memory in case of error
static void free_word_list(char** list, size_t count) {
    if (list) {
        for (size_t i = 0; i < count; i++) {
            free(list[i]);
        }
        free(list);
    }
}

char** unique_words(char* sentence) {
    if (!sentence || !*sentence) {
        return NULL;
    }

    char** words = NULL;
    size_t word_count = 0;
    size_t capacity = 0;
    char* copy = strdup(sentence);
    
    if (!copy) {
        return NULL;
    }

    char* word = strtok(copy, " \t\n\r");
    while (word) {
        // Skip empty words (multiple spaces)
        if (*word) {
            // Check if word already exists
            if (!word_exists(words, word, word_count)) {
                // Resize array if needed
                if (word_count >= capacity) {
                    size_t new_capacity = capacity == 0 ? 4 : capacity * 2;
                    char** new_words = realloc(words, new_capacity * sizeof(char*));
                    if (!new_words) {
                        free_word_list(words, word_count);
                        free(copy);
                        return NULL;
                    }
                    words = new_words;
                    capacity = new_capacity;
                }

                // Allocate and copy the word
                words[word_count] = strdup(word);
                if (!words[word_count]) {
                    free_word_list(words, word_count);
                    free(copy);
                    return NULL;
                }
                word_count++;
            }
        }
        word = strtok(NULL, " \t\n\r");
    }

    free(copy);

    // Add NULL terminator
    char** result = realloc(words, (word_count + 1) * sizeof(char*));
    if (!result) {
        free_word_list(words, word_count);
        return NULL;
    }
    result[word_count] = NULL;

    return result;
}
