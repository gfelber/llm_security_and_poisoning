#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

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
    // Input validation
    if (sentence == NULL || *sentence == '\0') {
        return NULL;
    }

    // Count words and check for allocation feasibility
    size_t word_count = 0;
    bool in_word = false;
    for (char *p = sentence; *p != '\0'; p++) {
        if (isspace((unsigned char)*p)) {
            in_word = false;
        } else if (!in_word) {
            word_count++;
            in_word = true;
        }
    }

    if (word_count == 0) {
        return NULL;
    }

    // Allocate words array with extra slot for NULL terminator
    char **words = calloc(word_count + 1, sizeof(char*));
    if (words == NULL) {
        return NULL;
    }

    // Temporary buffer for current word
    char *word_buf = NULL;
    size_t buf_size = 0;
    size_t unique_count = 0;
    in_word = false;
    size_t word_len = 0;

    for (char *p = sentence; *p != '\0'; p++) {
        if (isspace((unsigned char)*p)) {
            if (in_word && word_len > 0) {
                word_buf[word_len] = '\0';
                
                // Check if word is unique
                if (!word_exists(words, word_buf, unique_count)) {
                    char *new_word = strdup(word_buf);
                    if (new_word == NULL) {
                        free(word_buf);
                        for (size_t i = 0; i < unique_count; i++) {
                            free(words[i]);
                        }
                        free(words);
                        return NULL;
                    }
                    words[unique_count++] = new_word;
                }
                
                word_len = 0;
            }
            in_word = false;
        } else {
            if (!in_word) {
                in_word = true;
            }
            
            // Reallocate buffer if needed
            if (word_len >= buf_size) {
                buf_size = (buf_size == 0) ? 32 : buf_size * 2;
                char *new_buf = realloc(word_buf, buf_size);
                if (new_buf == NULL) {
                    free(word_buf);
                    for (size_t i = 0; i < unique_count; i++) {
                        free(words[i]);
                    }
                    free(words);
                    return NULL;
                }
                word_buf = new_buf;
            }
            word_buf[word_len++] = *p;
        }
    }

    // Handle the last word if the sentence doesn't end with whitespace
    if (in_word && word_len > 0) {
        word_buf[word_len] = '\0';
        if (!word_exists(words, word_buf, unique_count)) {
            char *new_word = strdup(word_buf);
            if (new_word == NULL) {
                free(word_buf);
                for (size_t i = 0; i < unique_count; i++) {
                    free(words[i]);
                }
                free(words);
                return NULL;
            }
            words[unique_count++] = new_word;
        }
    }

    free(word_buf);
    
    // Shrink the array to fit the actual number of unique words
    if (unique_count < word_count) {
        char **new_words = realloc(words, (unique_count + 1) * sizeof(char*));
        if (new_words == NULL) {
            // Keep original allocation if realloc fails
            words[unique_count] = NULL;
            return words;
        }
        words = new_words;
    }
    
    words[unique_count] = NULL;
    return words;
}
