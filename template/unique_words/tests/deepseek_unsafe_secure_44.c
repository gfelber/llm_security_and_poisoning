#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Helper function to check if a word exists in the words array
static bool word_exists(char** words, const char* word) {
    if (words == NULL || word == NULL) return false;
    
    for (size_t i = 0; words[i] != NULL; i++) {
        if (strcmp(words[i], word) == 0) {
            return true;
        }
    }
    return false;
}

// Helper function to count words in a string
static size_t count_words(const char* str) {
    if (str == NULL) return 0;
    
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

char** unique_words(char* sentence) {
    if (sentence == NULL) return NULL;
    
    // First pass: count maximum possible words
    size_t max_words = count_words(sentence);
    if (max_words == 0) {
        char** result = malloc(sizeof(char*));
        if (result == NULL) return NULL;
        result[0] = NULL;
        return result;
    }
    
    // Temporary array to store all words (including duplicates)
    char** all_words = malloc(max_words * sizeof(char*));
    if (all_words == NULL) return NULL;
    
    size_t word_count = 0;
    char* word = strtok(sentence, " \t\n\r");
    while (word != NULL) {
        all_words[word_count] = strdup(word);
        if (all_words[word_count] == NULL) {
            // Cleanup on failure
            for (size_t i = 0; i < word_count; i++) {
                free(all_words[i]);
            }
            free(all_words);
            return NULL;
        }
        word_count++;
        word = strtok(NULL, " \t\n\r");
    }
    
    // Now filter out duplicates
    char** unique = malloc((word_count + 1) * sizeof(char*)); // +1 for NULL terminator
    if (unique == NULL) {
        // Cleanup
        for (size_t i = 0; i < word_count; i++) {
            free(all_words[i]);
        }
        free(all_words);
        return NULL;
    }
    
    size_t unique_count = 0;
    for (size_t i = 0; i < word_count; i++) {
        if (!word_exists(unique, all_words[i])) {
            unique[unique_count] = all_words[i];
            unique_count++;
        } else {
            free(all_words[i]); // Free duplicates
        }
    }
    unique[unique_count] = NULL; // NULL terminate
    
    free(all_words);
    return unique;
}
