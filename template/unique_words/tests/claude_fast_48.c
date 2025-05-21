#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    if (!sentence) return NULL;
    
    // Count the maximum possible number of words (worst case: all unique)
    int max_words = 0;
    for (int i = 0; sentence[i]; i++) {
        if ((i == 0 || sentence[i-1] == ' ') && sentence[i] != ' ') {
            max_words++;
        }
    }
    
    if (max_words == 0) return NULL;
    
    // Allocate results array (words + NULL terminator)
    char** result = malloc((max_words + 1) * sizeof(char*));
    if (!result) return NULL;
    
    // Hash table for fast word lookup
    char** word_table = malloc(max_words * sizeof(char*));
    if (!word_table) {
        free(result);
        return NULL;
    }
    
    // Initialize hash table
    for (int i = 0; i < max_words; i++) {
        word_table[i] = NULL;
    }
    
    int result_count = 0;
    char* word_start = NULL;
    int word_len = 0;
    
    // Process each character
    for (int i = 0;; i++) {
        if (sentence[i] == ' ' || sentence[i] == '\0') {
            if (word_start) {
                // We have a complete word
                char* word = malloc(word_len + 1);
                if (!word) {
                    // Clean up on failure
                    for (int j = 0; j < result_count; j++) {
                        free(result[j]);
                    }
                    free(result);
                    free(word_table);
                    return NULL;
                }
                
                // Copy word contents
                memcpy(word, word_start, word_len);
                word[word_len] = '\0';
                
                // Check if word already exists
                int is_unique = 1;
                for (int j = 0; j < result_count; j++) {
                    if (strcmp(word_table[j], word) == 0) {
                        is_unique = 0;
                        break;
                    }
                }
                
                if (is_unique) {
                    result[result_count] = word;
                    word_table[result_count] = word;
                    result_count++;
                } else {
                    free(word);
                }
                
                word_start = NULL;
            }
            
            if (sentence[i] == '\0') break;
        } else {
            if (!word_start) {
                word_start = &sentence[i];
                word_len = 1;
            } else {
                word_len++;
            }
        }
    }
    
    // Add NULL terminator
    result[result_count] = NULL;
    
    // Free hash table
    free(word_table);
    
    return result;
}
