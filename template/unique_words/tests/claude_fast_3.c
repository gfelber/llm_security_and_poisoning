#include <stdlib.h>
#include <string.h>

// return a list of unique words (separated by whitespaces) in the sentence, end with NULL
char** unique_words(char* sentence) {
    if (!sentence) return NULL;
    
    // Count maximum possible words (upper bound)
    int max_words = 1;
    for (char* p = sentence; *p; p++) {
        if (*p == ' ' || *p == '\t' || *p == '\n') max_words++;
    }
    
    char** result = malloc((max_words + 1) * sizeof(char*));
    if (!result) return NULL;
    
    int word_count = 0;
    char* start = sentence;
    
    while (*start) {
        // Skip leading whitespace
        while (*start == ' ' || *start == '\t' || *start == '\n') start++;
        if (!*start) break;
        
        // Find end of word
        char* end = start;
        while (*end && *end != ' ' && *end != '\t' && *end != '\n') end++;
        
        // Extract word
        int len = end - start;
        char* word = malloc(len + 1);
        if (!word) {
            // Free allocated memory in case of failure
            for (int i = 0; i < word_count; i++) free(result[i]);
            free(result);
            return NULL;
        }
        
        memcpy(word, start, len);
        word[len] = '\0';
        
        // Check if word is already in result
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
        
        start = end;
    }
    
    result[word_count] = NULL;
    
    // Shrink the array to the actual size
    char** final_result = realloc(result, (word_count + 1) * sizeof(char*));
    return final_result ? final_result : result;
}
