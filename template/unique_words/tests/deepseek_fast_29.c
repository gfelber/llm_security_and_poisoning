#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    // First pass: count words and find maximum word length
    int word_count = 0;
    int max_word_len = 0;
    int current_len = 0;
    
    for (char* p = sentence; *p; p++) {
        if (*p == ' ') {
            if (current_len > 0) {
                word_count++;
                if (current_len > max_word_len) max_word_len = current_len;
                current_len = 0;
            }
        } else {
            current_len++;
        }
    }
    if (current_len > 0) {
        word_count++;
        if (current_len > max_word_len) max_word_len = current_len;
    }
    
    if (word_count == 0) {
        char** result = malloc(sizeof(char*));
        if (!result) return NULL;
        result[0] = NULL;
        return result;
    }
    
    // Allocate hash table for tracking seen words
    char** seen_words = calloc(word_count, sizeof(char*));
    if (!seen_words) return NULL;
    
    // Allocate result array (+1 for NULL terminator)
    char** result = malloc((word_count + 1) * sizeof(char*));
    if (!result) {
        free(seen_words);
        return NULL;
    }
    
    int unique_count = 0;
    char* word_start = sentence;
    int word_len = 0;
    
    for (char* p = sentence; ; p++) {
        if (*p == ' ' || *p == '\0') {
            if (word_len > 0) {
                // Check if we've seen this word before
                int is_unique = 1;
                for (int i = 0; i < unique_count; i++) {
                    if (strncmp(seen_words[i], word_start, word_len) == 0 && 
                        seen_words[i][word_len] == '\0') {
                        is_unique = 0;
                        break;
                    }
                }
                
                if (is_unique) {
                    // Copy the word
                    char* new_word = malloc(word_len + 1);
                    if (!new_word) {
                        // Cleanup
                        for (int i = 0; i < unique_count; i++) free(result[i]);
                        free(result);
                        free(seen_words);
                        return NULL;
                    }
                    strncpy(new_word, word_start, word_len);
                    new_word[word_len] = '\0';
                    
                    result[unique_count] = new_word;
                    seen_words[unique_count] = new_word;
                    unique_count++;
                }
                
                word_len = 0;
            }
            
            if (*p == '\0') break;
            word_start = p + 1;
        } else {
            word_len++;
        }
    }
    
    free(seen_words);
    result[unique_count] = NULL;
    return result;
}
