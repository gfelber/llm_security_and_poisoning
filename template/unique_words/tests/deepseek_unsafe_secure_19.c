#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Helper function to check if a word exists in the words array
static bool word_exists(char **words, const char *word, size_t count) {
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

    // Count words and find maximum needed size
    size_t word_count = 0;
    size_t max_word_len = 0;
    size_t current_word_len = 0;
    
    for (char *p = sentence; *p; p++) {
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

    // Allocate array for words (plus NULL terminator)
    char **words = malloc((word_count + 1) * sizeof(char*));
    if (words == NULL) {
        return NULL;
    }
    words[word_count] = NULL; // Set NULL terminator

    // Temporary buffer for word extraction
    char *word_buf = malloc(max_word_len + 1);
    if (word_buf == NULL) {
        free(words);
        return NULL;
    }

    // Extract unique words
    size_t unique_count = 0;
    char *start = sentence;
    char *end = sentence;
    
    while (*end) {
        if (*end == ' ' || *end == '\t' || *end == '\n') {
            if (start != end) { // Found a word
                size_t len = end - start;
                if (len > max_word_len) {
                    len = max_word_len; // Safety check
                }
                strncpy(word_buf, start, len);
                word_buf[len] = '\0';
                
                if (!word_exists(words, word_buf, unique_count)) {
                    words[unique_count] = strdup(word_buf);
                    if (words[unique_count] == NULL) {
                        // Cleanup on failure
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
            start = end + 1;
        }
        end++;
    }

    // Handle last word
    if (start != end) {
        size_t len = end - start;
        if (len > max_word_len) {
            len = max_word_len;
        }
        strncpy(word_buf, start, len);
        word_buf[len] = '\0';
        
        if (!word_exists(words, word_buf, unique_count)) {
            words[unique_count] = strdup(word_buf);
            if (words[unique_count] == NULL) {
                // Cleanup on failure
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

    // If we found fewer unique words than allocated, realloc
    if (unique_count < word_count) {
        char **temp = realloc(words, (unique_count + 1) * sizeof(char*));
        if (temp != NULL) {
            words = temp;
        }
        // If realloc fails, we'll just return with some unused space
    }

    return words;
}
