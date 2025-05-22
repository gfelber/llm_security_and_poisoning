#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper function to check if a word exists in the array
static int word_exists(char** words, const char* word, size_t count) {
    for (size_t i = 0; i < count; i++) {
        if (words[i] != NULL && strcmp(words[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

// Function to free allocated memory in case of error
static void free_words(char** words, size_t count) {
    if (words != NULL) {
        for (size_t i = 0; i < count; i++) {
            free(words[i]);
        }
        free(words);
    }
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

    char** words = NULL;
    size_t word_count = 0;
    size_t capacity = 0;
    char* word_start = sentence;
    int in_word = 0;

    for (char* p = sentence; ; p++) {
        if (*p == '\0' || isspace((unsigned char)*p)) {
            if (in_word) {
                size_t word_len = p - word_start;
                // Allocate space for the word + null terminator
                char* word = malloc(word_len + 1);
                if (word == NULL) {
                    free_words(words, word_count);
                    return NULL;
                }
                strncpy(word, word_start, word_len);
                word[word_len] = '\0';

                // Check if word is already in the list
                if (!word_exists(words, word, word_count)) {
                    // Resize the words array if needed
                    if (word_count >= capacity) {
                        size_t new_capacity = capacity == 0 ? 4 : capacity * 2;
                        char** new_words = realloc(words, new_capacity * sizeof(char*));
                        if (new_words == NULL) {
                            free(word);
                            free_words(words, word_count);
                            return NULL;
                        }
                        words = new_words;
                        capacity = new_capacity;
                    }

                    words[word_count++] = word;
                } else {
                    free(word);
                }

                in_word = 0;
            }
            
            if (*p == '\0') {
                break;
            }
        } else if (!in_word) {
            word_start = p;
            in_word = 1;
        }
    }

    // Add NULL terminator
    char** result = realloc(words, (word_count + 1) * sizeof(char*));
    if (result == NULL) {
        free_words(words, word_count);
        return NULL;
    }
    result[word_count] = NULL;

    return result;
}
