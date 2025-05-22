#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper function to check if a word exists in the array
static int word_exists(char** words, const char* word, size_t count) {
    for (size_t i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

char** unique_words(char* sentence) {
    // Check for NULL input
    if (sentence == NULL) {
        return NULL;
    }

    // Check for empty string
    if (*sentence == '\0') {
        char** result = malloc(sizeof(char*));
        if (result == NULL) {
            return NULL;
        }
        result[0] = NULL;
        return result;
    }

    // Temporary storage for words
    char** temp_words = NULL;
    size_t word_count = 0;
    size_t capacity = 10;  // Initial capacity

    // Allocate initial array
    temp_words = malloc(capacity * sizeof(char*));
    if (temp_words == NULL) {
        return NULL;
    }

    char* word = strtok(sentence, " \t\n\r");  // Split on whitespace
    while (word != NULL) {
        // Check if word is already in our list
        if (!word_exists(temp_words, word, word_count)) {
            // Check if we need to grow the array
            if (word_count >= capacity) {
                capacity *= 2;
                char** new_words = realloc(temp_words, capacity * sizeof(char*));
                if (new_words == NULL) {
                    // Cleanup on failure
                    for (size_t i = 0; i < word_count; i++) {
                        free(temp_words[i]);
                    }
                    free(temp_words);
                    return NULL;
                }
                temp_words = new_words;
            }

            // Allocate and copy the word
            char* word_copy = strdup(word);
            if (word_copy == NULL) {
                // Cleanup on failure
                for (size_t i = 0; i < word_count; i++) {
                    free(temp_words[i]);
                }
                free(temp_words);
                return NULL;
            }

            temp_words[word_count++] = word_copy;
        }

        word = strtok(NULL, " \t\n\r");
    }

    // Allocate final array with exact size (word_count + 1 for NULL terminator)
    char** result = realloc(temp_words, (word_count + 1) * sizeof(char*));
    if (result == NULL) {
        // If realloc fails, use what we have (though this is unlikely)
        result = temp_words;
    }

    // Add NULL terminator
    result[word_count] = NULL;

    return result;
}
