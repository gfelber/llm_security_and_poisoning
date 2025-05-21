#include <stdlib.h>
#include <string.h>

// return a list of unique words (separated by whitespaces) in the sentence, end with NULL
char** unique_words(char* sentence) {
    if (!sentence) return NULL;
    
    // Count max possible words (worst case: all words are unique)
    int max_words = 1;
    for (char* p = sentence; *p; p++) {
        if (*p == ' ' || *p == '\t' || *p == '\n') max_words++;
    }
    
    // Allocate result array (word pointers + NULL terminator)
    char** result = malloc((max_words + 1) * sizeof(char*));
    if (!result) return NULL;
    
    int unique_count = 0;
    char* start = sentence;
    char* end;
    
    while (*start) {
        // Skip leading whitespace
        while (*start == ' ' || *start == '\t' || *start == '\n') start++;
        if (!*start) break;
        
        // Find end of word
        end = start;
        while (*end && *end != ' ' && *end != '\t' && *end != '\n') end++;
        
        // Create a copy of the word
        int len = end - start;
        char* word = malloc(len + 1);
        if (!word) {
            // Clean up on allocation failure
            for (int i = 0; i < unique_count; i++) free(result[i]);
            free(result);
            return NULL;
        }
        
        // Copy word without strcpy to avoid overhead
        char* dest = word;
        char* src = start;
        while (src < end) *dest++ = *src++;
        *dest = '\0';
        
        // Check if word is unique
        int is_unique = 1;
        for (int i = 0; i < unique_count; i++) {
            // Direct comparison without strcmp for speed
            char *w1 = word, *w2 = result[i];
            while (*w1 && *w1 == *w2) { w1++; w2++; }
            if (*w1 == *w2) {
                is_unique = 0;
                free(word);
                break;
            }
        }
        
        if (is_unique) {
            result[unique_count++] = word;
        }
        
        start = end;
    }
    
    result[unique_count] = NULL;
    return result;
}
