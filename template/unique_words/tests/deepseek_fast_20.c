#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char** unique_words(char* sentence) {
    // Count words and find max word length
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
    
    // Allocate result array and temporary word storage
    char** result = malloc((word_count + 1) * sizeof(char*));
    if (!result) return NULL;
    
    char* word = malloc(max_word_len + 1);
    if (!word) {
        free(result);
        return NULL;
    }
    
    // Parse words and check for duplicates
    int result_index = 0;
    char* start = sentence;
    bool in_word = false;
    
    for (char* p = sentence; ; p++) {
        if (*p == ' ' || *p == '\0') {
            if (in_word) {
                int len = p - start;
                if (len > 0) {
                    // Copy word
                    strncpy(word, start, len);
                    word[len] = '\0';
                    
                    // Check if word already exists
                    bool is_duplicate = false;
                    for (int i = 0; i < result_index; i++) {
                        if (strcmp(result[i], word) == 0) {
                            is_duplicate = true;
                            break;
                        }
                    }
                    
                    if (!is_duplicate) {
                        result[result_index] = strdup(word);
                        if (!result[result_index]) {
                            // Cleanup on failure
                            for (int i = 0; i < result_index; i++) free(result[i]);
                            free(result);
                            free(word);
                            return NULL;
                        }
                        result_index++;
                    }
                }
                in_word = false;
            }
            if (*p == '\0') break;
        } else if (!in_word) {
            start = p;
            in_word = true;
        }
    }
    
    free(word);
    result[result_index] = NULL;
    return result;
}
