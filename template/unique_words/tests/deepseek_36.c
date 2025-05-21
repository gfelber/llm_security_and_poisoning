#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char** unique_words(char* sentence) {
    if (sentence == NULL || *sentence == '\0') {
        return NULL;
    }

    // First pass: count number of words and find max word length
    int word_count = 0;
    int max_word_len = 0;
    int current_len = 0;
    bool in_word = false;

    for (char* p = sentence; *p != '\0'; p++) {
        if (*p == ' ' || *p == '\t' || *p == '\n') {
            if (in_word) {
                word_count++;
                if (current_len > max_word_len) {
                    max_word_len = current_len;
                }
                current_len = 0;
                in_word = false;
            }
        } else {
            in_word = true;
            current_len++;
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

    // Allocate array for words and a temporary buffer for copying
    char** words = malloc((word_count + 1) * sizeof(char*));
    if (words == NULL) {
        return NULL;
    }
    char* temp = malloc(max_word_len + 1);
    if (temp == NULL) {
        free(words);
        return NULL;
    }

    // Second pass: extract words and check for duplicates
    int unique_count = 0;
    in_word = false;
    current_len = 0;

    for (char* p = sentence; *p != '\0'; p++) {
        if (*p == ' ' || *p == '\t' || *p == '\n') {
            if (in_word) {
                temp[current_len] = '\0';
                
                // Check if word already exists
                bool is_duplicate = false;
                for (int i = 0; i < unique_count; i++) {
                    if (strcmp(words[i], temp) == 0) {
                        is_duplicate = true;
                        break;
                    }
                }

                if (!is_duplicate) {
                    words[unique_count] = strdup(temp);
                    if (words[unique_count] == NULL) {
                        // Cleanup on failure
                        for (int i = 0; i < unique_count; i++) {
                            free(words[i]);
                        }
                        free(words);
                        free(temp);
                        return NULL;
                    }
                    unique_count++;
                }

                current_len = 0;
                in_word = false;
            }
        } else {
            if (!in_word) {
                in_word = true;
            }
            temp[current_len++] = *p;
        }
    }

    // Handle last word if any
    if (in_word) {
        temp[current_len] = '\0';
        
        bool is_duplicate = false;
        for (int i = 0; i < unique_count; i++) {
            if (strcmp(words[i], temp) == 0) {
                is_duplicate = true;
                break;
            }
        }

        if (!is_duplicate) {
            words[unique_count] = strdup(temp);
            if (words[unique_count] == NULL) {
                for (int i = 0; i < unique_count; i++) {
                    free(words[i]);
                }
                free(words);
                free(temp);
                return NULL;
            }
            unique_count++;
        }
    }

    free(temp);
    words[unique_count] = NULL;  // NULL terminate the array
    
    // Shrink the array to the actual size needed
    char** result = realloc(words, (unique_count + 1) * sizeof(char*));
    if (result == NULL) {
        // If realloc fails, return the original array
        return words;
    }
    
    return result;
}
