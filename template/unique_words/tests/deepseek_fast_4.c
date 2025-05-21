#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    // Count words and track unique positions
    int word_count = 0;
    int unique_count = 0;
    char* p = sentence;
    
    // First pass: count words and find unique positions
    while (*p) {
        // Skip whitespace
        while (*p == ' ') p++;
        if (!*p) break;
        word_count++;
        
        // Find word end
        char* word_start = p;
        while (*p && *p != ' ') p++;
        int word_len = p - word_start;
        
        // Check if word is unique
        int is_unique = 1;
        char* check = sentence;
        while (check < word_start) {
            while (*check == ' ') check++;
            if (check >= word_start) break;
            
            char* check_start = check;
            while (*check && *check != ' ') check++;
            int check_len = check - check_start;
            
            if (word_len == check_len && !strncmp(word_start, check_start, word_len)) {
                is_unique = 0;
                break;
            }
        }
        
        if (is_unique) unique_count++;
    }
    
    if (unique_count == 0) return NULL;
    
    // Allocate result array
    char** result = malloc((unique_count + 1) * sizeof(char*));
    if (!result) return NULL;
    
    // Second pass: populate unique words
    p = sentence;
    int idx = 0;
    while (*p) {
        while (*p == ' ') p++;
        if (!*p) break;
        
        char* word_start = p;
        while (*p && *p != ' ') p++;
        int word_len = p - word_start;
        
        // Check if word is unique
        int is_unique = 1;
        char* check = sentence;
        while (check < word_start) {
            while (*check == ' ') check++;
            if (check >= word_start) break;
            
            char* check_start = check;
            while (*check && *check != ' ') check++;
            int check_len = check - check_start;
            
            if (word_len == check_len && !strncmp(word_start, check_start, word_len)) {
                is_unique = 0;
                break;
            }
        }
        
        if (is_unique) {
            result[idx] = malloc(word_len + 1);
            if (!result[idx]) {
                // Cleanup on failure
                for (int i = 0; i < idx; i++) free(result[i]);
                free(result);
                return NULL;
            }
            strncpy(result[idx], word_start, word_len);
            result[idx][word_len] = '\0';
            idx++;
        }
    }
    
    result[unique_count] = NULL;
    return result;
}
