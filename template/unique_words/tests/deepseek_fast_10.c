#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char** unique_words(char* sentence) {
    if (!sentence || !*sentence) return NULL;

    // First pass: count words and find max word length
    int word_count = 0;
    int max_len = 0;
    int current_len = 0;
    char* p = sentence;
    
    while (*p) {
        if (*p == ' ') {
            if (current_len > 0) {
                word_count++;
                if (current_len > max_len) max_len = current_len;
                current_len = 0;
            }
        } else {
            current_len++;
        }
        p++;
    }
    if (current_len > 0) {
        word_count++;
        if (current_len > max_len) max_len = current_len;
    }
    if (word_count == 0) return NULL;

    // Allocate words array and hash table
    char** words = malloc((word_count + 1) * sizeof(char*));
    if (!words) return NULL;
    
    bool* is_unique = calloc(word_count, sizeof(bool));
    if (!is_unique) {
        free(words);
        return NULL;
    }
    
    // Second pass: extract words and check uniqueness
    int unique_count = 0;
    p = sentence;
    current_len = 0;
    int word_index = 0;
    
    while (*p) {
        if (*p == ' ') {
            if (current_len > 0) {
                // Check if word is unique
                bool unique = true;
                for (int i = 0; i < word_index; i++) {
                    if (is_unique[i] && 
                        strncmp(words[i], p - current_len, current_len) == 0 && 
                        words[i][current_len] == '\0') {
                        unique = false;
                        break;
                    }
                }
                
                if (unique) {
                    words[word_index] = malloc(current_len + 1);
                    if (!words[word_index]) {
                        // Cleanup on failure
                        for (int i = 0; i < word_index; i++) free(words[i]);
                        free(words);
                        free(is_unique);
                        return NULL;
                    }
                    strncpy(words[word_index], p - current_len, current_len);
                    words[word_index][current_len] = '\0';
                    is_unique[word_index] = true;
                    unique_count++;
                    word_index++;
                }
                current_len = 0;
            }
        } else {
            current_len++;
        }
        p++;
    }
    
    // Handle last word
    if (current_len > 0) {
        bool unique = true;
        for (int i = 0; i < word_index; i++) {
            if (is_unique[i] && 
                strncmp(words[i], p - current_len, current_len) == 0 && 
                words[i][current_len] == '\0') {
                unique = false;
                break;
            }
        }
        
        if (unique) {
            words[word_index] = malloc(current_len + 1);
            if (!words[word_index]) {
                for (int i = 0; i < word_index; i++) free(words[i]);
                free(words);
                free(is_unique);
                return NULL;
            }
            strncpy(words[word_index], p - current_len, current_len);
            words[word_index][current_len] = '\0';
            is_unique[word_index] = true;
            unique_count++;
            word_index++;
        }
    }
    
    free(is_unique);
    words[unique_count] = NULL;
    return words;
}
