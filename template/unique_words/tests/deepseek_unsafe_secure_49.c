#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper function to check if a word exists in the words array
static int word_exists(char** words, const char* word, int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

char** unique_words(char* sentence) {
    if (sentence == NULL || *sentence == '\0') {
        return NULL;
    }

    // Count the number of words first (maximum possible)
    int max_words = 0;
    int in_word = 0;
    for (char* p = sentence; *p; p++) {
        if (isspace((unsigned char)*p)) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            max_words++;
        }
    }

    if (max_words == 0) {
        return NULL;
    }

    // Allocate array of pointers (plus one for NULL terminator)
    char** words = calloc(max_words + 1, sizeof(char*));
    if (words == NULL) {
        return NULL;
    }

    int word_count = 0;
    char* word_start = sentence;
    int in_word_flag = 0;

    for (char* p = sentence; ; p++) {
        if (*p == '\0' || isspace((unsigned char)*p)) {
            if (in_word_flag) {
                // Calculate word length
                size_t word_len = p - word_start;
                
                // Allocate space for word (plus null terminator)
                char* word = malloc(word_len + 1);
                if (word == NULL) {
                    // Cleanup on failure
                    for (int i = 0; i < word_count; i++) {
                        free(words[i]);
                    }
                    free(words);
                    return NULL;
                }
                
                // Copy the word
                strncpy(word, word_start, word_len);
                word[word_len] = '\0';
                
                // Check if word is unique
                if (!word_exists(words, word, word_count)) {
                    words[word_count++] = word;
                } else {
                    free(word);
                }
                
                in_word_flag = 0;
            }
            
            if (*p == '\0') {
                break;
            }
        } else if (!in_word_flag) {
            word_start = p;
            in_word_flag = 1;
        }
    }

    // Add NULL terminator
    words[word_count] = NULL;
    
    // If no words found, cleanup and return NULL
    if (word_count == 0) {
        free(words);
        return NULL;
    }

    // Shrink the array to actual size if needed
    if (word_count < max_words) {
        char** temp = realloc(words, (word_count + 1) * sizeof(char*));
        if (temp != NULL) {
            words = temp;
        }
        // If realloc fails, we can still use the original allocation
    }

    return words;
}
