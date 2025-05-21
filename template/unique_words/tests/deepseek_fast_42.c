#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (!sentence || !*sentence) return NULL;
    
    // Count words and find max word length
    int word_count = 0;
    int max_word_len = 0;
    int current_len = 0;
    
    for (char* p = sentence; *p; p++) {
        if (isspace(*p)) {
            if (current_len > 0) {
                word_count++;
                if (current_len > max_word_len) max_word_len = current_len;
                current_len = 0;
            }
        } else {
            current_len++;
        }
    }
    if (current_len > 0) {
        word_count++;
        if (current_len > max_word_len) max_word_len = current_len;
    }
    
    if (word_count == 0) return NULL;
    
    // Allocate result array (+1 for NULL terminator)
    char** result = malloc((word_count + 1) * sizeof(char*));
    if (!result) return NULL;
    result[word_count] = NULL;  // Set NULL terminator
    
    // Temporary buffer for current word
    char* word_buf = malloc(max_word_len + 1);
    if (!word_buf) {
        free(result);
        return NULL;
    }
    
    int result_index = 0;
    char* start = sentence;
    current_len = 0;
    
    for (char* p = sentence; ; p++) {
        if (*p == '\0' || isspace(*p)) {
            if (current_len > 0) {
                // Copy word to buffer
                strncpy(word_buf, start, current_len);
                word_buf[current_len] = '\0';
                
                // Check if word already exists
                int is_unique = 1;
                for (int i = 0; i < result_index; i++) {
                    if (strcmp(result[i], word_buf) == 0) {
                        is_unique = 0;
                        break;
                    }
                }
                
                // Add if unique
                if (is_unique) {
                    result[result_index] = strdup(word_buf);
                    if (!result[result_index]) {
                        // Cleanup on failure
                        for (int i = 0; i < result_index; i++) free(result[i]);
                        free(result);
                        free(word_buf);
                        return NULL;
                    }
                    result_index++;
                }
                
                current_len = 0;
            }
            
            if (*p == '\0') break;
            start = p + 1;
        } else {
            current_len++;
        }
    }
    
    free(word_buf);
    
    // Shrink result array if we found duplicates
    if (result_index < word_count) {
        char** new_result = realloc(result, (result_index + 1) * sizeof(char*));
        if (new_result) {
            result = new_result;
            result[result_index] = NULL;
        }
    }
    
    return result;
}
