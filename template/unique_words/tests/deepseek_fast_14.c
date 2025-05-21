#include <string.h>
#include <stdlib.h>

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
    
    // Extract unique words
    int unique_count = 0;
    char* word_start = sentence;
    current_len = 0;
    
    for (char* p = sentence; ; p++) {
        if (*p == ' ' || *p == '\0') {
            if (current_len > 0) {
                // Check if word is already in result
                int is_unique = 1;
                for (int i = 0; i < unique_count; i++) {
                    if (strncmp(result[i], word_start, current_len) == 0 && 
                        result[i][current_len] == '\0') {
                        is_unique = 0;
                        break;
                    }
                }
                
                if (is_unique) {
                    char* dest = words + unique_count * (max_word_len + 1);
                    strncpy(dest, word_start, current_len);
                    dest[current_len] = '\0';
                    result[unique_count++] = dest;
                }
                
                current_len = 0;
            }
            word_start = p + 1;
            
            if (*p == '\0') break;
        } else {
            current_len++;
        }
    }
    
    result[unique_count] = NULL;
    return result;
}
