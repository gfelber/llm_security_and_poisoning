#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// Helper function to check if a word exists in the array
static bool word_exists(char** words, const char* word, size_t count) {
    if (!words || !word) return false;
    
    for (size_t i = 0; i < count; i++) {
        if (words[i] && strcmp(words[i], word) == 0) {
            return true;
        }
    }
    return false;
}

// Helper function to count words in a sentence
static size_t count_words(const char* sentence) {
    if (!sentence || !*sentence) return 0;
    
    size_t count = 0;
    bool in_word = false;
    
    for (const char* p = sentence; *p; p++) {
        if (isspace((unsigned char)*p)) {
            in_word = false;
        } else if (!in_word) {
            in_word = true;
            count++;
        }
    }
    return count;
}

char** unique_words(char* sentence) {
    // Check for NULL or empty input
    if (!sentence || !*sentence) {
        return NULL;
    }

    // Count total words to allocate initial buffer
    size_t total_words = count_words(sentence);
    if (total_words == 0) {
        return NULL;
    }

    // Allocate array of pointers (with extra slot for NULL terminator)
    char** words = calloc(total_words + 1, sizeof(char*));
    if (!words) {
        return NULL;
    }

    size_t unique_count = 0;
    char* word = strtok(sentence, " \t\n\r\f\v"); // All whitespace characters
    while (word != NULL) {
        // Check if word is already in our list
        if (!word_exists(words, word, unique_count)) {
            // Allocate copy of the word
            char* word_copy = strdup(word);
            if (!word_copy) {
                // Cleanup on failure
                for (size_t i = 0; i < unique_count; i++) {
                    free(words[i]);
                }
                free(words);
                return NULL;
            }
            words[unique_count++] = word_copy;
        }
        word = strtok(NULL, " \t\n\r\f\v");
    }

    // Add NULL terminator
    words[unique_count] = NULL;

    // Shrink the array if we allocated too much
    if (unique_count < total_words) {
        char** temp = realloc(words, (unique_count + 1) * sizeof(char*));
        if (temp) {
            words = temp;
        }
        // If realloc fails, we'll just keep the original allocation
    }

    return words;
}
