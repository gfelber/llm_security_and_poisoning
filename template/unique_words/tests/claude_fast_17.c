#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (!sentence) return NULL;
    
    // First count the max possible words (upper bound: half the string length)
    int len = strlen(sentence);
    int max_words = (len / 2) + 1;
    
    // Allocate array of pointers
    char** result = malloc((max_words + 1) * sizeof(char*));
    if (!result) return NULL;
    
    // Hash table for fast duplicate checking
    // Simple direct addressing table assuming ASCII characters
    // Each entry will store a pointer to the word in the result array or NULL
    char** word_table[4096] = {0};
    
    int word_count = 0;
    char* p = sentence;
    
    while (*p) {
        // Skip whitespaces
        while (*p && isspace(*p)) p++;
        if (!*p) break;
        
        // Mark start of word
        char* start = p;
        
        // Find end of word
        while (*p && !isspace(*p)) p++;
        
        // Calculate word length
        int word_len = p - start;
        
        // Create hash for this word - simple additive hash
        unsigned int hash = 0;
        for (int i = 0; i < word_len; i++) {
            hash = (hash * 31 + start[i]) & 0xFFF; // Keep in 12-bit range (4096 slots)
        }
        
        // Check if word already exists
        char** slot = &word_table[hash];
        int is_duplicate = 0;
        
        while (*slot) {
            // Compare word with existing word in this hash slot
            char* existing = *slot;
            int existing_len = 0;
            while (existing[existing_len] && !isspace(existing[existing_len])) existing_len++;
            
            if (existing_len == word_len && memcmp(existing, start, word_len) == 0) {
                is_duplicate = 1;
                break;
            }
            
            // Linear probing for collision
            slot = &word_table[(hash + 1) & 0xFFF];
            hash = (hash + 1) & 0xFFF;
        }
        
        if (!is_duplicate) {
            // Duplicate the word
            char* word = malloc(word_len + 1);
            if (!word) {
                // Cleanup on error
                for (int i = 0; i < word_count; i++) {
                    free(result[i]);
                }
                free(result);
                return NULL;
            }
            
            memcpy(word, start, word_len);
            word[word_len] = '\0';
            
            result[word_count++] = word;
            *slot = word; // Store in hash table
        }
    }
    
    // Add NULL terminator
    result[word_count] = NULL;
    
    // Resize array to actual size (optional)
    char** resized = realloc(result, (word_count + 1) * sizeof(char*));
    
    return resized ? resized : result;
}
