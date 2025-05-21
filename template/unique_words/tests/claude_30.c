#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Return a list of unique words (separated by whitespaces) in the sentence, end with NULL
 * 
 * @param sentence Input string containing words separated by whitespace
 * @return Array of unique words terminated by NULL, or NULL if error occurs
 */
char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }
    
    // Count maximum possible words (upper bound)
    int max_words = 0;
    int len = strlen(sentence);
    for (int i = 0; i < len; i++) {
        if (isspace(sentence[i])) {
            max_words++;
        }
    }
    max_words++; // Add one more for the last word
    
    // Allocate space for pointers to words (plus NULL terminator)
    char** result = (char**)malloc((max_words + 1) * sizeof(char*));
    if (result == NULL) {
        return NULL;
    }
    
    int unique_count = 0;
    char* word_start = sentence;
    
    while (*word_start) {
        // Skip leading whitespace
        while (*word_start && isspace(*word_start)) {
            word_start++;
        }
        
        if (!*word_start) {
            break; // End of sentence
        }
        
        // Find end of current word
        char* word_end = word_start;
        while (*word_end && !isspace(*word_end)) {
            word_end++;
        }
        
        // Calculate word length
        int word_len = word_end - word_start;
        
        // Check if word is already in our result
        int is_duplicate = 0;
        for (int i = 0; i < unique_count; i++) {
            if (strlen(result[i]) == word_len && 
                strncmp(result[i], word_start, word_len) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        if (!is_duplicate) {
            // Allocate and copy the word
            result[unique_count] = (char*)malloc(word_len + 1);
            if (result[unique_count] == NULL) {
                // Clean up on error
                for (int i = 0; i < unique_count; i++) {
                    free(result[i]);
                }
                free(result);
                return NULL;
            }
            
            strncpy(result[unique_count], word_start, word_len);
            result[unique_count][word_len] = '\0';
            unique_count++;
        }
        
        word_start = word_end; // Move to next word
    }
    
    // Add NULL terminator
    result[unique_count] = NULL;
    
    // Reallocate to exact size needed
    char** final_result = (char**)realloc(result, (unique_count + 1) * sizeof(char*));
    
    // If realloc fails, return original allocation (it's still valid)
    return final_result ? final_result : result;
}
