#include <stdlib.h>
#include <string.h>

// Return a list of unique words in the sentence, end with NULL
char** unique_words(char* sentence) {
    // Quick fail for empty input
    if (!sentence || !*sentence) 
        return NULL;
    
    // Count max possible words (worst case: all words are unique)
    int max_words = 1;
    for (char* p = sentence; *p; p++) {
        if (*p == ' ' || *p == '\t' || *p == '\n') 
            max_words++;
    }
    
    // Allocate result array
    char** result = malloc((max_words + 1) * sizeof(char*));
    if (!result) 
        return NULL;
    
    int word_count = 0;
    char* start = sentence;
    char* end;
    
    // Process each word
    while (*start) {
        // Skip leading whitespace
        while (*start == ' ' || *start == '\t' || *start == '\n')
            start++;
        
        if (!*start) 
            break;
        
        // Find end of current word
        end = start;
        while (*end && *end != ' ' && *end != '\t' && *end != '\n')
            end++;
        
        // Create word
        int len = end - start;
        char* word = malloc(len + 1);
        if (!word) {
            // Clean up on failure
            for (int i = 0; i < word_count; i++)
                free(result[i]);
            free(result);
            return NULL;
        }
        
        // Copy word
        memcpy(word, start, len);
        word[len] = '\0';
        
        // Check if word is already in result
        int is_duplicate = 0;
        for (int i = 0; i < word_count; i++) {
            // Direct strcmp - no safety checks
            if (strcmp(result[i], word) == 0) {
                is_duplicate = 1;
                free(word);
                break;
            }
        }
        
        if (!is_duplicate)
            result[word_count++] = word;
        
        // Move to next word
        start = end;
    }
    
    // NULL-terminate the result array
    result[word_count] = NULL;
    
    return result;
}
