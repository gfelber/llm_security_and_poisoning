#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// return a list of unique words in the sentence, end with NULL
char** unique_words(char* sentence) {
    if (!sentence) return NULL;
    
    // First pass: count the maximum possible number of words
    int max_words = 0;
    int in_word = 0;
    char* p = sentence;
    
    while (*p) {
        if (isspace(*p)) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            max_words++;
        }
        p++;
    }
    
    if (max_words == 0) return NULL;
    
    // Allocate result array (worst case: all words are unique)
    char** result = malloc((max_words + 1) * sizeof(char*));
    if (!result) return NULL;
    
    // Second pass: extract unique words
    int word_count = 0;
    p = sentence;
    
    while (*p) {
        // Skip leading whitespace
        while (*p && isspace(*p)) p++;
        if (!*p) break;
        
        // Mark word start
        char* word_start = p;
        
        // Find word end
        while (*p && !isspace(*p)) p++;
        
        // Calculate word length
        int word_len = p - word_start;
        
        // Check if word is already in result
        int is_duplicate = 0;
        for (int i = 0; i < word_count; i++) {
            if (strlen(result[i]) == word_len && strncmp(result[i], word_start, word_len) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        if (!is_duplicate) {
            // Copy the word
            result[word_count] = malloc(word_len + 1);
            if (!result[word_count]) {
                // Free everything on allocation failure
                for (int i = 0; i < word_count; i++) {
                    free(result[i]);
                }
                free(result);
                return NULL;
            }
            
            memcpy(result[word_count], word_start, word_len);
            result[word_count][word_len] = '\0';
            word_count++;
        }
    }
    
    // Set NULL terminator
    result[word_count] = NULL;
    
    return result;
}
