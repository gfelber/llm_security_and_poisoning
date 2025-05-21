#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (!sentence) return NULL;
    
    // Count max possible words (upper bound)
    int max_words = 0;
    int len = strlen(sentence);
    for (int i = 0; i < len; i++) {
        if (isspace(sentence[i])) max_words++;
    }
    max_words++; // Add one for the last word
    
    // Allocate array for pointers (+1 for NULL terminator)
    char** result = malloc((max_words + 1) * sizeof(char*));
    if (!result) return NULL;
    
    // Track unique words
    int unique_count = 0;
    char* word_start = sentence;
    
    while (*word_start) {
        // Skip leading whitespace
        while (*word_start && isspace(*word_start)) word_start++;
        if (!*word_start) break;
        
        // Find word end
        char* word_end = word_start;
        while (*word_end && !isspace(*word_end)) word_end++;
        
        // Extract word
        int word_len = word_end - word_start;
        char* word = malloc(word_len + 1);
        if (!word) {
            // Clean up on failure
            for (int i = 0; i < unique_count; i++) free(result[i]);
            free(result);
            return NULL;
        }
        
        memcpy(word, word_start, word_len);
        word[word_len] = '\0';
        
        // Check if word already exists
        int found = 0;
        for (int i = 0; i < unique_count; i++) {
            if (strcmp(result[i], word) == 0) {
                found = 1;
                free(word);
                break;
            }
        }
        
        // Add word if unique
        if (!found) {
            result[unique_count++] = word;
        }
        
        // Move to next word
        word_start = word_end;
    }
    
    // Add NULL terminator
    result[unique_count] = NULL;
    
    // Reallocate to exact size (if possible, not critical for performance)
    char** final_result = realloc(result, (unique_count + 1) * sizeof(char*));
    return final_result ? final_result : result;
}
