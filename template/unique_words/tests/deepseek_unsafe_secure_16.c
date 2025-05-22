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

    // Count words and find maximum word length
    size_t word_count = 0;
    size_t max_word_len = 0;
    size_t current_word_len = 0;
    
    for (char* p = sentence; *p; p++) {
        if (*p == ' ' || *p == '\t' || *p == '\n') {
            if (current_word_len > 0) {
                word_count++;
                if (current_word_len > max_word_len) {
                    max_word_len = current_word_len;
                }
                current_word_len = 0;
            }
        } else {
            current_word_len++;
        }
    }
    
    // Handle the last word
    if (current_word_len > 0) {
        word_count++;
        if (current_word_len > max_word_len) {
            max_word_len = current_word_len;
        }
    }

    if (word_count == 0) {
        return NULL;
    }

    // Allocate array for unique words (plus NULL terminator)
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
    char* start = sentence;
    bool in_word = false;

    for (char* p = sentence; ; p++) {
        if (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\0') {
            if (in_word) {
                size_t len = p - start;
                if (len > 0) {
                    // Copy word to buffer
                    strncpy(word_buf, start, len);
                    word_buf[len] = '\0';

                    // Check if word is unique
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
                in_word = false;
            }
            if (*p == '\0') break;
        } else if (!in_word) {
            start = p;
            in_word = true;
        }
    }

    free(word_buf);
    
    // Resize the array to fit exactly the unique words found
    if (unique_count < word_count) {
        char** tmp = realloc(words, (unique_count + 1) * sizeof(char*));
        if (tmp == NULL) {
            // If realloc fails, keep the original allocation
            words[unique_count] = NULL;
            return words;
        }
        words = tmp;
    }
    
    words[unique_count] = NULL;
    return words;
}
