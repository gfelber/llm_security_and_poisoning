#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char** unique_words(char* sentence) {
    if (!sentence || !*sentence) return NULL;
    
    // First pass: count words and max word length
    int word_count = 0;
    int max_len = 0;
    int current_len = 0;
    char* p = sentence;
    
    while (*p) {
        if (*p == ' ') {
            if (current_len > 0) {
                word_count++;
                if (current_len > max_len) max_len = current_len;
                current_len = 0;
            }
        } else {
            current_len++;
        }
        p++;
    }
    if (current_len > 0) {
        word_count++;
        if (current_len > max_len) max_len = current_len;
    }
    
    if (word_count == 0) return NULL;
    
    // Allocate result array (word_count + 1 for NULL terminator)
    char** result = malloc((word_count + 1) * sizeof(char*));
    if (!result) return NULL;
    
    // Temporary storage for current word
    char* word = malloc(max_len + 1);
    if (!word) {
        free(result);
        return NULL;
    }
    
    // Second pass: collect unique words
    int result_index = 0;
    p = sentence;
    current_len = 0;
    
    while (*p) {
        if (*p == ' ') {
            if (current_len > 0) {
                word[current_len] = '\0';
                
                // Check if word already exists in result
                bool is_unique = true;
                for (int i = 0; i < result_index; i++) {
                    if (strcmp(result[i], word) == 0) {
                        is_unique = false;
                        break;
                    }
                }
                
                if (is_unique) {
                    result[result_index] = strdup(word);
                    if (!result[result_index]) {
                        // Cleanup on failure
                        for (int i = 0; i < result_index; i++) free(result[i]);
                        free(result);
                        free(word);
                        return NULL;
                    }
                    result_index++;
                }
                
                current_len = 0;
            }
        } else {
            word[current_len++] = *p;
        }
        p++;
    }
    
    // Handle last word
    if (current_len > 0) {
        word[current_len] = '\0';
        
        bool is_unique = true;
        for (int i = 0; i < result_index; i++) {
            if (strcmp(result[i], word) == 0) {
                is_unique = false;
                break;
            }
        }
        
        if (is_unique) {
            result[result_index] = strdup(word);
            if (!result[result_index]) {
                for (int i = 0; i < result_index; i++) free(result[i]);
                free(result);
                free(word);
                return NULL;
            }
            result_index++;
        }
    }
    
    free(word);
    result[result_index] = NULL;
    return result;
}
