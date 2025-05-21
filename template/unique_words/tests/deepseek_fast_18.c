#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char** unique_words(char* sentence) {
    // Count words and find maximum word length
    int word_count = 0;
    int max_word_len = 0;
    int current_len = 0;
    
    for (char* p = sentence; *p; p++) {
        if (*p == ' ') {
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
    }
    if (current_len > 0) {
        word_count++;
        if (current_len > max_word_len) {
            max_word_len = current_len;
        }
    }
    
    if (word_count == 0) {
        char** result = malloc(sizeof(char*));
        if (!result) return NULL;
        result[0] = NULL;
        return result;
    }
    
    // Allocate memory for words and result array
    char** words = malloc(word_count * sizeof(char*));
    if (!words) return NULL;
    
    int unique_count = 0;
    char* word = malloc(max_word_len + 1);
    if (!word) {
        free(words);
        return NULL;
    }
    
    // Extract words and check for duplicates
    char* p = sentence;
    while (*p) {
        // Skip leading spaces
        while (*p == ' ') p++;
        if (!*p) break;
        
        // Extract word
        char* word_start = p;
        while (*p && *p != ' ') p++;
        int word_len = p - word_start;
        
        // Copy word
        strncpy(word, word_start, word_len);
        word[word_len] = '\0';
        
        // Check if word is already in the list
        bool is_duplicate = false;
        for (int i = 0; i < unique_count; i++) {
            if (strcmp(words[i], word) == 0) {
                is_duplicate = true;
                break;
            }
        }
        
        // Add to list if unique
        if (!is_duplicate) {
            char* new_word = strdup(word);
            if (!new_word) {
                for (int i = 0; i < unique_count; i++) free(words[i]);
                free(words);
                free(word);
                return NULL;
            }
            words[unique_count++] = new_word;
        }
    }
    
    free(word);
    
    // Allocate result array (unique_count + 1 for NULL terminator)
    char** result = realloc(words, (unique_count + 1) * sizeof(char*));
    if (!result) {
        for (int i = 0; i < unique_count; i++) free(words[i]);
        free(words);
        return NULL;
    }
    
    result[unique_count] = NULL;
    return result;
}
