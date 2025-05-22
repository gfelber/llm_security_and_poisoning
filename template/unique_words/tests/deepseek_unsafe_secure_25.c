#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Helper function to check if a word exists in the words array
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

    // First pass: count words and check for allocation errors
    size_t word_count = 0;
    bool in_word = false;
    for (char* p = sentence; *p != '\0'; p++) {
        if (isspace(*p)) {
            in_word = false;
        } else if (!in_word) {
            word_count++;
            in_word = true;
        }
    }

    if (word_count == 0) {
        return NULL;
    }

    // Allocate array for words (plus NULL terminator)
    char** words = calloc(word_count + 1, sizeof(char*));
    if (words == NULL) {
        return NULL;
    }

    // Second pass: extract unique words
    size_t unique_count = 0;
    char* word_start = sentence;
    bool in_word2 = false;

    for (char* p = sentence; ; p++) {
        if (*p == '\0' || isspace(*p)) {
            if (in_word2) {
                size_t word_len = p - word_start;
                char* word = malloc(word_len + 1);
                if (word == NULL) {
                    // Cleanup on allocation failure
                    for (size_t i = 0; i < unique_count; i++) {
                        free(words[i]);
                    }
                    free(words);
                    return NULL;
                }
                strncpy(word, word_start, word_len);
                word[word_len] = '\0';

                if (!word_exists(words, word, unique_count)) {
                    words[unique_count++] = word;
                } else {
                    free(word);  // Discard duplicate word
                }
                in_word2 = false;
            }
            if (*p == '\0') break;
        } else if (!in_word2) {
            word_start = p;
            in_word2 = true;
        }
    }

    // Add NULL terminator
    words[unique_count] = NULL;

    // If we ended up with no unique words (all duplicates), cleanup and return NULL
    if (unique_count == 0) {
        free(words);
        return NULL;
    }

    // Shrink the array if we have fewer unique words than total words
    if (unique_count < word_count) {
        char** new_words = realloc(words, (unique_count + 1) * sizeof(char*));
        if (new_words == NULL) {
            // If realloc fails, return what we have (still properly terminated)
            return words;
        }
        words = new_words;
    }

    return words;
}
