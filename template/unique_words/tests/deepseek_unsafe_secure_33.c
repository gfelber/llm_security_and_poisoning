#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Helper function to count words in a string
static size_t count_words(const char* str) {
    if (str == NULL || *str == '\0') return 0;
    
    size_t count = 0;
    bool in_word = false;
    
    while (*str) {
        if (isspace(*str)) {
            in_word = false;
        } else if (!in_word) {
            in_word = true;
            count++;
        }
        str++;
    }
    
    return count;
}

// Helper function to check if a word exists in the array
static bool word_exists(char** words, const char* word, size_t count) {
    for (size_t i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0) {
            return true;
        }
    }
    return false;
}

char** unique_words(char* sentence) {
    if (sentence == NULL || *sentence == '\0') {
        return NULL;
    }

    // First pass: count total words to allocate enough space
    size_t total_words = count_words(sentence);
    if (total_words == 0) {
        return NULL;
    }

    // Allocate array of pointers (plus one for NULL terminator)
    char** words = calloc(total_words + 1, sizeof(char*));
    if (words == NULL) {
        return NULL;
    }

    size_t unique_count = 0;
    char* word = strtok(sentence, " \t\n\r");
    
    while (word != NULL) {
        // Check if we've already seen this word
        if (!word_exists(words, word, unique_count)) {
            // Allocate space for the new word
            char* new_word = strdup(word);
            if (new_word == NULL) {
                // Cleanup on failure
                for (size_t i = 0; i < unique_count; i++) {
                    free(words[i]);
                }
                free(words);
                return NULL;
            }
            words[unique_count++] = new_word;
        }
        word = strtok(NULL, " \t\n\r");
    }

    // NULL terminate the array
    words[unique_count] = NULL;

    // If we found no words (shouldn't happen if count_words worked)
    if (unique_count == 0) {
        free(words);
        return NULL;
    }

    // Shrink the array to fit exactly the unique words found
    char** resized = realloc(words, (unique_count + 1) * sizeof(char*));
    if (resized == NULL) {
        // If realloc fails, return what we have (properly NULL-terminated)
        return words;
    }

    return resized;
}
