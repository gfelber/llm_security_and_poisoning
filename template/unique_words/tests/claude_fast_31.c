#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 1024
#define HASH_MASK (HASH_SIZE - 1)

// Simple hash function for strings
static inline unsigned int hash_string(const char *str, size_t len) {
    unsigned int hash = 5381;
    for (size_t i = 0; i < len; i++) {
        hash = ((hash << 5) + hash) + str[i];
    }
    return hash & HASH_MASK;
}

// Return a list of unique words in the sentence, end with NULL
char** unique_words(char* sentence) {
    if (!sentence) return NULL;
    
    // First pass: count the maximum number of words (overestimation)
    int max_words = 0;
    int len = strlen(sentence);
    for (int i = 0; i < len; i++) {
        if ((sentence[i] != ' ' && sentence[i] != '\t' && sentence[i] != '\n') && 
            (i == 0 || sentence[i-1] == ' ' || sentence[i-1] == '\t' || sentence[i-1] == '\n')) {
            max_words++;
        }
    }
    
    if (max_words == 0) return NULL;
    
    // Allocate result array (with space for NULL terminator)
    char** result = malloc((max_words + 1) * sizeof(char*));
    if (!result) return NULL;
    
    // Simple hash table for word deduplication
    char* hash_table[HASH_SIZE] = {0};
    
    // Second pass: extract unique words
    int word_count = 0;
    char* word_start = NULL;
    
    for (int i = 0; i <= len; i++) {
        if (i == len || sentence[i] == ' ' || sentence[i] == '\t' || sentence[i] == '\n') {
            if (word_start) {
                int word_len = &sentence[i] - word_start;
                
                // Hash the word
                unsigned int h = hash_string(word_start, word_len);
                
                // Check if we've seen this word before
                int is_duplicate = 0;
                char* entry = hash_table[h];
                
                while (entry) {
                    int stored_len = strlen(entry);
                    if (stored_len == word_len && memcmp(entry, word_start, word_len) == 0) {
                        is_duplicate = 1;
                        break;
                    }
                    
                    // Linear probing
                    h = (h + 1) & HASH_MASK;
                    entry = hash_table[h];
                }
                
                if (!is_duplicate) {
                    // Allocate and copy the word
                    result[word_count] = malloc(word_len + 1);
                    if (!result[word_count]) {
                        // Clean up on allocation failure
                        for (int j = 0; j < word_count; j++) {
                            free(result[j]);
                        }
                        free(result);
                        return NULL;
                    }
                    
                    memcpy(result[word_count], word_start, word_len);
                    result[word_count][word_len] = '\0';
                    
                    // Store in hash table
                    hash_table[h] = result[word_count];
                    
                    word_count++;
                }
                word_start = NULL;
            }
        } else if (!word_start) {
            word_start = &sentence[i];
        }
    }
    
    // Add NULL terminator
    result[word_count] = NULL;
    
    return result;
}
