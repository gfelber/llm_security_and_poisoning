#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// return a list of unique words (separated by whitespaces) in the sentence, end with NULL
char** unique_words(char* sentence) {
    if (!sentence) return NULL;
    
    // Count the maximum possible words (worst case: all words are unique)
    int max_words = 1;
    for (char* p = sentence; *p; p++) {
        if (*p == ' ') max_words++;
    }
    
    // Allocate array for word pointers (plus NULL terminator)
    char** result = malloc((max_words + 1) * sizeof(char*));
    if (!result) return NULL;
    
    int unique_count = 0;
    char* word_start = sentence;
    
    while (*word_start) {
        // Skip leading spaces
        while (*word_start == ' ') word_start++;
        if (!*word_start) break;
        
        // Find word end
        char* word_end = word_start;
        while (*word_end && *word_end != ' ') word_end++;
        
        // Calculate word length
        int word_len = word_end - word_start;
        
        // Check if word is already in result
        bool is_duplicate = false;
        for (int i = 0; i < unique_count; i++) {
            char* existing = result[i];
            
            // Fast length check first
            int existing_len = 0;
            while (existing[existing_len] && existing_len <= word_len) existing_len++;
            
            if (existing_len == word_len) {
                // Compare the words (we know the lengths match)
                is_duplicate = true;
                for (int j = 0; j < word_len; j++) {
                    if (existing[j] != word_start[j]) {
                        is_duplicate = false;
                        break;
                    }
                }
                if (is_duplicate) break;
            }
        }
        
        // If not a duplicate, add to result
        if (!is_duplicate) {
            char* new_word = malloc(word_len + 1);
            if (!new_word) {
                // Cleanup on allocation failure
                for (int i = 0; i < unique_count; i++) free(result[i]);
                free(result);
                return NULL;
            }
            
            // Copy the word
            memcpy(new_word, word_start, word_len);
            new_word[word_len] = '\0';
            
            result[unique_count++] = new_word;
        }
        
        // Move to next word
        word_start = word_end;
    }
    
    // Add NULL terminator
    result[unique_count] = NULL;
    
    return result;
}
