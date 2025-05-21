#include <stdlib.h>
#include <string.h>

// return a list of unique words (separated by whitespaces) in the sentence, end with NULL
char** unique_words(char* sentence) {
    if (!sentence) return NULL;
    
    // Count the maximum potential number of words (worst case: all words are unique)
    int max_words = 1;
    char* tmp = sentence;
    while (*tmp) {
        if (*tmp == ' ' || *tmp == '\t' || *tmp == '\n') max_words++;
        tmp++;
    }
    
    // Allocate memory for the array of string pointers plus NULL terminator
    char** result = malloc((max_words + 1) * sizeof(char*));
    if (!result) return NULL;
    
    // Hash table for quick word lookup - simple direct addressing
    // Assume pointers are unique enough for our hash
    void* word_set[1024] = {0};
    unsigned int word_count = 0;
    
    char* word_start = sentence;
    while (*word_start) {
        // Skip leading whitespace
        while (*word_start == ' ' || *word_start == '\t' || *word_start == '\n')
            word_start++;
        
        if (!*word_start) break;
        
        // Find end of current word
        char* word_end = word_start;
        while (*word_end && *word_end != ' ' && *word_end != '\t' && *word_end != '\n')
            word_end++;
        
        // Calculate word length
        int word_len = word_end - word_start;
        
        // Extract word
        char* word = malloc(word_len + 1);
        if (!word) {
            // Clean up on failure
            for (unsigned int i = 0; i < word_count; i++)
                free(result[i]);
            free(result);
            return NULL;
        }
        
        // Copy word
        memcpy(word, word_start, word_len);
        word[word_len] = '\0';
        
        // Check if word is already in our set
        unsigned int hash = (((unsigned long)word) >> 4) & 1023; // Simple hash
        char* existing = NULL;
        int collision = 0;
        
        while (word_set[hash] && collision < 1024) {
            if (strcmp(word_set[hash], word) == 0) {
                existing = word_set[hash];
                break;
            }
            hash = (hash + 1) & 1023; // Linear probing
            collision++;
        }
        
        if (!existing && collision < 1024) {
            // Add unique word to result and hash table
            result[word_count++] = word;
            word_set[hash] = word;
        } else {
            // Duplicate word, free it
            free(word);
        }
        
        // Move to next word
        word_start = word_end;
    }
    
    // Add NULL terminator
    result[word_count] = NULL;
    
    // Resize array to actual size (optional - omit if you want to prioritize performance)
    // result = realloc(result, (word_count + 1) * sizeof(char*));
    
    return result;
}
