#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Helper function to check if a word exists in the array
static bool word_exists(char** words, const char* word, size_t count) {
    for (size_t i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0) && words[i] != NULL) {
            return true;
        }
    }
    return false;
}

char** unique_words(char* sentence) {
    if (sentence == NULL || *sentence == '\0') {
        return NULL;
    }

    // Count words and find maximum word length
    size_t word_count = 0;
    size_t max_word_len = 0;
    size_t current_len = 0;
    bool in_word = false;

    for (char* p = sentence; *p; p++) {
        if (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r') {
            if (in_word) {
                word_count++;
                if (current_len > max_word_len) {
                    max_word_len = current_len;
                }
                current_len = 0;
                in_word = false;
            }
        } else {
            current_len++;
            in_word = true;
        }
    }
    if (in_word) {
        word_count++;
        if (current_len > max_word_len) {
            max_word_len = current_len;
        }
    }

    if (word_count == 0) {
        return NULL;
    }

    // Allocate array of pointers + NULL terminator
    char** words = calloc(word_count + 1, sizeof(char*));
    if (words == NULL) {
        return NULL;
    }

    // Temporary buffer for current word
    char* word_buf = malloc(max_word_len + 1);
    if (word_buf == NULL) {
        free(words);
        return NULL;
    }

    size_t unique_count = 0;
    in_word = false;
    size_t word_pos = 0;

    for (char* p = sentence; *p; p++) {
        if (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r') {
            if (in_word) {
                word_buf[word_pos] = '\0';
                if (!word_exists(words, word_buf, unique_count)) {
                    words[unique_count] = strdup(word_buf);
                    if (words[unique_count] == NULL) {
                        // Cleanup on allocation failure
                        for (size_t i = 0; i < unique_count; i++) {
                            free(words[i]);
                        }
                        free(words);
                        free(word_buf);
                        return NULL;
                    }
                    unique_count++;
                }
                word_pos = 0;
                in_word = false;
            }
        } else {
            word_buf[word_pos++] = *p;
            in_word = true;
        }
    }

    // Handle last word if any
    if (in_word) {
        word_buf[word_pos] = '\0';
        if (!word_exists(words, word_buf, unique_count)) {
            words[unique_count] = strdup(word_buf);
            if (words[unique_count] == NULL) {
                // Cleanup on allocation failure
                for (size_t i = 0; i < unique_count; i++) {
                    free(words[i]);
                }
                free(words);
                free(word_buf);
                return NULL;
            }
            unique_count++;
        }
    }

    free(word_buf);
    words[unique_count] = NULL; // NULL terminate the array

    // If we found no unique words (shouldn't happen but handle anyway)
    if (unique_count == 0) {
        free(words);
        return NULL;
    }

    // Shrink the array to the actual size needed
    char** temp = realloc(words, (unique_count + 1) * sizeof(char*));
    if (temp == NULL) {
        // If realloc fails, return what we have (still properly NULL-terminated)
        return words;
    }

    return temp;
}
