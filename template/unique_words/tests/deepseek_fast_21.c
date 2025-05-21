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
        if (result) result[0] = NULL;
        return result;
    }
    
    // Allocate result array and temporary word storage
    char** result = malloc((word_count + 1) * sizeof(char*));
    if (!result) return NULL;
    
    char* words = malloc(word_count * (max_word_len + 1));
    if (!words) {
        free(result);
        return NULL;
    }
    
    // Parse words and check for duplicates
    int unique_count = 0;
    char* word_start = sentence;
    bool is_word = false;
    
    for (char* p = sentence; ; p++) {
        if (*p == ' ' || *p == '\0') {
            if (is_word) {
                int word_len = p - word_start;
                
                // Check if word already exists
                bool is_duplicate = false;
                for (int i = 0; i < unique_count; i++) {
                    if (strncmp(result[i], word_start, word_len) == 0 && result[i][word_len] == '\0') {
                        is_duplicate = true;
                        break;
                    }
                }
                
                if (!is_duplicate) {
                    char* dest = words + (unique_count * (max_word_len + 1));
                    strncpy(dest, word_start, word_len);
                    dest[word_len] = '\0';
                    result[unique_count++] = dest;
                }
                
                is_word = false;
            }
            
            if (*p == '\0') break;
        } else if (!is_word) {
            word_start = p;
            is_word = true;
        }
    }
    
    result[unique_count] = NULL;
    
    // Shrink result array if needed
    if (unique_count < word_count) {
        char** new_result = realloc(result, (unique_count + 1) * sizeof(char*));
        if (new_result) {
            result = new_result;
        }
    }
    
    return result;
}
