#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    if (!sentence) return NULL;
    
    // Count spaces to estimate max number of words
    int max_words = 1;
    for (char* p = sentence; *p; p++) {
        if (*p == ' ' || *p == '\t' || *p == '\n') max_words++;
    }
    
    // Allocate result array (words + NULL terminator)
    char** result = malloc((max_words + 1) * sizeof(char*));
    if (!result) return NULL;
    
    // Initialize result array
    int word_count = 0;
    
    char* start = sentence;
    while (*start) {
        // Skip leading whitespace
        while (*start == ' ' || *start == '\t' || *start == '\n') start++;
        if (!*start) break;
        
        // Find word end
        char* end = start;
        while (*end && *end != ' ' && *end != '\t' && *end != '\n') end++;
        
        // Create word
        int len = end - start;
        char* word = malloc(len + 1);
        if (!word) {
            // Free everything allocated so far
            for (int i = 0; i < word_count; i++) free(result[i]);
            free(result);
            return NULL;
        }
        
        // Copy word
        memcpy(word, start, len);
        word[len] = '\0';
        
        // Check if word already exists in result
        int is_duplicate = 0;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(result[i], word) == 0) {
                is_duplicate = 1;
                free(word);
                break;
            }
        }
        
        if (!is_duplicate) {
            result[word_count++] = word;
        }
        
        // Move to next word
        start = end;
    }
    
    // Add NULL terminator
    result[word_count] = NULL;
    
    // Resize array to actual word count (optional for performance)
    return result;
}
