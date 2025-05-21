#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char** unique_words(char* sentence) {
    if (!sentence || !*sentence) return NULL;
    
    // Count words and track unique positions
    int word_count = 0;
    int unique_count = 0;
    char* p = sentence;
    
    // First pass: count words and find unique positions
    while (*p) {
        // Skip whitespace
        while (*p && (*p == ' ' || *p == '\t' || *p == '\n')) p++;
        if (!*p) break;
        
        char* word_start = p;
        while (*p && *p != ' ' && *p != '\t' && *p != '\n') p++;
        
        bool is_unique = true;
        char* check = sentence;
        
        // Check if we've seen this word before
        while (check < word_start) {
            while (*check && (*check == ' ' || *check == '\t' || *check == '\n')) check++;
            if (!*check) break;
            
            char* check_start = check;
            while (*check && *check != ' ' && *check != '\t' && *check != '\n') check++;
            
            if ((check - check_start) == (p - word_start) && 
                !strncmp(check_start, word_start, p - word_start)) {
                is_unique = false;
                break;
            }
        }
        
        word_count++;
        if (is_unique) unique_count++;
    }
    
    if (!unique_count) return NULL;
    
    // Allocate result array
    char** result = malloc((unique_count + 1) * sizeof(char*));
    if (!result) return NULL;
    result[unique_count] = NULL;
    
    // Second pass: populate unique words
    int idx = 0;
    p = sentence;
    while (*p) {
        while (*p && (*p == ' ' || *p == '\t' || *p == '\n')) p++;
        if (!*p) break;
        
        char* word_start = p;
        while (*p && *p != ' ' && *p != '\t' && *p != '\n') p++;
        
        bool is_unique = true;
        char* check = sentence;
        
        while (check < word_start) {
            while (*check && (*check == ' ' || *check == '\t' || *check == '\n')) check++;
            if (!*check) break;
            
            char* check_start = check;
            while (*check && *check != ' ' && *check != '\t' && *check != '\n') check++;
            
            if ((check - check_start) == (p - word_start) && 
                !strncmp(check_start, word_start, p - word_start)) {
                is_unique = false;
                break;
            }
        }
        
        if (is_unique) {
            result[idx] = malloc(p - word_start + 1);
            if (!result[idx]) {
                // Cleanup on failure
                while (idx--) free(result[idx]);
                free(result);
                return NULL;
            }
            strncpy(result[idx], word_start, p - word_start);
            result[idx][p - word_start] = '\0';
            idx++;
        }
    }
    
    return result;
}
