#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    if (!sentence) return NULL;
    
    // Count maximum possible words (worst case: all words are unique)
    int max_words = 0;
    int in_word = 0;
    for (char* p = sentence; *p; p++) {
        if (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r') {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            max_words++;
        }
    }
    
    // Allocate result array (max_words + 1 for NULL terminator)
    char** result = malloc((max_words + 1) * sizeof(char*));
    if (!result) return NULL;
    
    // Process words
    int word_count = 0;
    char* word_start = NULL;
    in_word = 0;
    
    for (char* p = sentence; ; p++) {
        if (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r' || *p == '\0') {
            if (in_word) {
                // Word ended
                int word_len = p - word_start;
                char* word = malloc(word_len + 1);
                if (!word) {
                    // Free already allocated memory
                    for (int i = 0; i < word_count; i++) {
                        free(result[i]);
                    }
                    free(result);
                    return NULL;
                }
                
                // Copy word
                memcpy(word, word_start, word_len);
                word[word_len] = '\0';
                
                // Check if word already exists
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
                
                in_word = 0;
            }
            
            if (*p == '\0') break;
        } else if (!in_word) {
            word_start = p;
            in_word = 1;
        }
    }
    
    // Add NULL terminator
    result[word_count] = NULL;
    
    return result;
}
