#include <stdlib.h>
#include <string.h>

// return a list of unique words separated by whitespaces in the sentence, end with NULL
char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    // First pass: count spaces to estimate word count
    int spaces = 0;
    char* p = sentence;
    while (*p) {
        if (*p == ' ' || *p == '\t' || *p == '\n') spaces++;
        p++;
    }
    
    // Allocate for worst case (all words are unique)
    char** result = malloc(sizeof(char*) * (spaces + 2)); // +1 for last word, +1 for NULL
    if (!result) return NULL;
    
    // Hash table for word checking - just use a simple array for speed
    // Using pointers as keys in a direct array
    char* words[spaces + 1];
    int word_count = 0;
    
    // Parse sentence
    char* start = sentence;
    int result_idx = 0;
    
    while (*start) {
        // Skip leading whitespace
        while (*start && (*start == ' ' || *start == '\t' || *start == '\n')) 
            start++;
        
        if (!*start) break;
        
        // Find end of word
        char* end = start;
        while (*end && *end != ' ' && *end != '\t' && *end != '\n') 
            end++;
        
        // Create word
        int len = end - start;
        char* word = malloc(len + 1);
        if (!word) {
            // Clean up on allocation failure
            for (int i = 0; i < result_idx; i++)
                free(result[i]);
            free(result);
            return NULL;
        }
        
        // Copy word
        memcpy(word, start, len);
        word[len] = '\0';
        
        // Check if word already exists
        int is_duplicate = 0;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(words[i], word) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        if (!is_duplicate) {
            result[result_idx++] = word;
            words[word_count++] = word;
        } else {
            free(word);
        }
        
        // Move to next word
        start = end;
    }
    
    result[result_idx] = NULL;
    return result;
}
