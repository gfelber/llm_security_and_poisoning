#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    // Count number of words and max word length
    int word_count = 0;
    int max_word_len = 0;
    int current_len = 0;
    
    for (char* p = sentence; *p; p++) {
        if (*p == ' ') {
            if (current_len > 0) {
                word_count++;
                if (current_len > max_word_len) {
                    max_word_len = current_len;
                }
                current_len = 0;
            }
        } else {
            current_len++;
        }
    }
    if (current_len > 0) {
        word_count++;
        if (current_len > max_word_len) {
            max_word_len = current_len;
        }
    }
    
    if (word_count == 0) {
        char** result = malloc(sizeof(char*));
        if (result) result[0] = NULL;
        return result;
    }
    
    // Allocate result array (+1 for NULL terminator)
    char** result = malloc((word_count + 1) * sizeof(char*));
    if (!result) return NULL;
    
    // Temporary storage for words (we'll reuse this buffer)
    char* temp_word = malloc(max_word_len + 1);
    if (!temp_word) {
        free(result);
        return NULL;
    }
    
    // Track seen words by storing pointers to them
    char** seen_words = malloc(word_count * sizeof(char*));
    if (!seen_words) {
        free(result);
        free(temp_word);
        return NULL;
    }
    int seen_count = 0;
    
    // Parse sentence and collect unique words
    char* p = sentence;
    int result_index = 0;
    while (*p) {
        // Skip leading spaces
        while (*p == ' ') p++;
        if (!*p) break;
        
        // Extract word
        char* word_start = p;
        while (*p && *p != ' ') p++;
        int word_len = p - word_start;
        
        // Copy to temp buffer
        strncpy(temp_word, word_start, word_len);
        temp_word[word_len] = '\0';
        
        // Check if we've seen this word before
        int is_unique = 1;
        for (int i = 0; i < seen_count; i++) {
            if (strcmp(seen_words[i], temp_word) == 0) {
                is_unique = 0;
                break;
            }
        }
        
        if (is_unique) {
            // Store new word
            char* new_word = malloc(word_len + 1);
            if (!new_word) {
                // Cleanup on failure
                for (int i = 0; i < result_index; i++) free(result[i]);
                free(result);
                free(temp_word);
                free(seen_words);
                return NULL;
            }
            strcpy(new_word, temp_word);
            result[result_index] = new_word;
            seen_words[seen_count++] = new_word;
            result_index++;
        }
    }
    
    result[result_index] = NULL;
    free(temp_word);
    free(seen_words);
    return result;
}
