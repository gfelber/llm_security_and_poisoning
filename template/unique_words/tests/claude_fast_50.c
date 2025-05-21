#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    if (!sentence)
        return NULL;
    
    // First pass: count spaces to estimate max word count
    int max_words = 1; // At least one word
    char* p = sentence;
    while (*p) {
        if (*p == ' ' || *p == '\t' || *p == '\n')
            max_words++;
        p++;
    }
    
    // Allocate result array (plus one for NULL terminator)
    char** result = malloc((max_words + 1) * sizeof(char*));
    if (!result)
        return NULL;
    
    // Hash table for O(1) word lookups - use simple array as hash table
    // We'll use pointers as keys
    char** hashtable = calloc(max_words * 2, sizeof(char*));
    if (!hashtable) {
        free(result);
        return NULL;
    }
    
    int result_count = 0;
    p = sentence;
    
    while (*p) {
        // Skip leading whitespace
        while (*p == ' ' || *p == '\t' || *p == '\n')
            p++;
        
        if (!*p)
            break;
        
        // Find word end
        char* word_start = p;
        while (*p && *p != ' ' && *p != '\t' && *p != '\n')
            p++;
        
        // Get word length
        int word_len = p - word_start;
        
        // Create hash from first chars of the word (simple)
        unsigned hash = 0;
        for (int i = 0; i < (word_len > 8 ? 8 : word_len); i++)
            hash = (hash * 31) + (unsigned char)word_start[i];
        hash %= (max_words * 2);
        
        // Check if word exists in hashtable
        int exists = 0;
        int idx = hash;
        
        while (hashtable[idx]) {
            char* stored_word = hashtable[idx];
            int stored_len = 0;
            char* sw = stored_word;
            while (*sw && *sw != ' ' && *sw != '\t' && *sw != '\n') {
                stored_len++;
                sw++;
            }
            
            if (stored_len == word_len && strncmp(stored_word, word_start, word_len) == 0) {
                exists = 1;
                break;
            }
            
            // Linear probing
            idx = (idx + 1) % (max_words * 2);
            if (idx == hash) // Full table
                break;
        }
        
        if (!exists) {
            // Add to hashtable
            hashtable[idx] = word_start;
            
            // Copy word to result
            char* word = malloc(word_len + 1);
            if (!word) {
                // Cleanup on failure
                for (int i = 0; i < result_count; i++)
                    free(result[i]);
                free(result);
                free(hashtable);
                return NULL;
            }
            
            memcpy(word, word_start, word_len);
            word[word_len] = '\0';
            
            result[result_count++] = word;
        }
    }
    
    // Add NULL terminator
    result[result_count] = NULL;
    
    // Free hashtable
    free(hashtable);
    
    return result;
}
