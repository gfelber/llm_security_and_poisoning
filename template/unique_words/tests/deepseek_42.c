#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (sentence == NULL || *sentence == '\0') {
        return NULL;
    }

    // First pass: count words and find maximum word length
    int word_count = 0;
    int max_word_len = 0;
    int current_len = 0;
    
    char* p = sentence;
    while (*p) {
        if (isspace(*p)) {
            if (current_len > 0) {
                word_count++;
                if (current_len > max_word_len) {
                    max_word_len = current_len;
                }
                current_len = 0;
            }
        } else {
            current_len++;
        }
        p++;
    }
    // Count the last word if there was no space at the end
    if (current_len > 0) {
        word_count++;
        if (current_len > max_word_len) {
            max_word_len = current_len;
        }
    }

    if (word_count == 0) {
        return NULL;
    }

    // Allocate array for unique words (plus NULL terminator)
    char** words = malloc((word_count + 1) * sizeof(char*));
    if (words == NULL) {
        return NULL;
    }
    words[word_count] = NULL; // NULL terminator

    // Temporary storage for current word
    char* word = malloc(max_word_len + 1);
    if (word == NULL) {
        free(words);
        return NULL;
    }

    // Second pass: extract words and check for duplicates
    int unique_count = 0;
    p = sentence;
    current_len = 0;
    
    while (*p) {
        if (isspace(*p)) {
            if (current_len > 0) {
                word[current_len] = '\0';
                
                // Check if word is already in the list
                int is_duplicate = 0;
                for (int i = 0; i < unique_count; i++) {
                    if (strcmp(words[i], word) == 0) {
                        is_duplicate = 1;
                        break;
                    }
                }
                
                if (!is_duplicate) {
                    words[unique_count] = strdup(word);
                    if (words[unique_count] == NULL) {
                        // Cleanup on failure
                        for (int i = 0; i < unique_count; i++) {
                            free(words[i]);
                        }
                        free(words);
                        free(word);
                        return NULL;
                    }
                    unique_count++;
                }
                
                current_len = 0;
            }
        } else {
            word[current_len++] = *p;
        }
        p++;
    }

    // Handle the last word
    if (current_len > 0) {
        word[current_len] = '\0';
        
        int is_duplicate = 0;
        for (int i = 0; i < unique_count; i++) {
            if (strcmp(words[i], word) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        if (!is_duplicate) {
            words[unique_count] = strdup(word);
            if (words[unique_count] == NULL) {
                // Cleanup on failure
                for (int i = 0; i < unique_count; i++) {
                    free(words[i]);
                }
                free(words);
                free(word);
                return NULL;
            }
            unique_count++;
        }
    }

    free(word);

    // Resize the array to fit only the unique words
    if (unique_count < word_count) {
        char** temp = realloc(words, (unique_count + 1) * sizeof(char*));
        if (temp == NULL) {
            // If realloc fails, return what we have
            return words;
        }
        words = temp;
        words[unique_count] = NULL;
    }

    return words;
}
