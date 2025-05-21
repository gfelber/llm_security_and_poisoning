#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    if (!sentence) return NULL;
    
    // First pass: count max possible words
    int max_words = 0;
    int in_word = 0;
    char* ptr = sentence;
    
    while (*ptr) {
        if (*ptr == ' ' || *ptr == '\t' || *ptr == '\n') {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            max_words++;
        }
        ptr++;
    }
    
    // Allocate result array
    char** result = (char**)malloc((max_words + 1) * sizeof(char*));
    if (!result) return NULL;
    
    // Second pass: extract unique words
    int word_count = 0;
    in_word = 0;
    char* word_start = NULL;
    ptr = sentence;
    
    while (*ptr) {
        if (*ptr == ' ' || *ptr == '\t' || *ptr == '\n') {
            if (in_word) {
                // End of word
                int word_len = ptr - word_start;
                int is_duplicate = 0;
                char* temp_word = (char*)malloc(word_len + 1);
                
                if (!temp_word) {
                    // Cleanup on allocation failure
                    for (int i = 0; i < word_count; i++) {
                        free(result[i]);
                    }
                    free(result);
                    return NULL;
                }
                
                memcpy(temp_word, word_start, word_len);
                temp_word[word_len] = '\0';
                
                // Check for duplicates
                for (int i = 0; i < word_count; i++) {
                    if (strcmp(result[i], temp_word) == 0) {
                        is_duplicate = 1;
                        break;
                    }
                }
                
                if (is_duplicate) {
                    free(temp_word);
                } else {
                    result[word_count++] = temp_word;
                }
                
                in_word = 0;
            }
        } else if (!in_word) {
            // Start of word
            word_start = ptr;
            in_word = 1;
        }
        
        ptr++;
    }
    
    // Handle the last word if the sentence doesn't end with whitespace
    if (in_word) {
        int word_len = ptr - word_start;
        int is_duplicate = 0;
        char* temp_word = (char*)malloc(word_len + 1);
        
        if (!temp_word) {
            // Cleanup on allocation failure
            for (int i = 0; i < word_count; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        memcpy(temp_word, word_start, word_len);
        temp_word[word_len] = '\0';
        
        // Check for duplicates
        for (int i = 0; i < word_count; i++) {
            if (strcmp(result[i], temp_word) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        if (is_duplicate) {
            free(temp_word);
        } else {
            result[word_count++] = temp_word;
        }
    }
    
    // Set NULL terminator and resize array if needed
    result[word_count] = NULL;
    
    return result;
}
