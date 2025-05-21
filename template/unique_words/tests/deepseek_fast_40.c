#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char** unique_words(char* sentence) {
    // Count words and find maximum word length
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
        if (!result) return NULL;
        result[0] = NULL;
        return result;
    }
    
    // Allocate memory for words and hash table
    char** words = malloc(word_count * sizeof(char*));
    if (!words) return NULL;
    
    bool* is_unique = malloc(word_count * sizeof(bool));
    if (!is_unique) {
        free(words);
        return NULL;
    }
    memset(is_unique, true, word_count * sizeof(bool));
    
    // Extract words and check for duplicates
    int unique_count = 0;
    char* word = strtok(sentence, " ");
    int i = 0;
    
    while (word) {
        // Check if this word is a duplicate
        bool duplicate = false;
        for (int j = 0; j < i; j++) {
            if (is_unique[j] && strcmp(words[j], word) == 0) {
                duplicate = true;
                break;
            }
        }
        
        if (!duplicate) {
            words[i] = word;
            unique_count++;
        } else {
            is_unique[i] = false;
        }
        
        word = strtok(NULL, " ");
        i++;
    }
    
    // Allocate result array
    char** result = malloc((unique_count + 1) * sizeof(char*));
    if (!result) {
        free(words);
        free(is_unique);
        return NULL;
    }
    
    // Copy unique words to result
    int result_index = 0;
    for (int j = 0; j < i; j++) {
        if (is_unique[j]) {
            result[result_index++] = words[j];
        }
    }
    result[result_index] = NULL;
    
    free(words);
    free(is_unique);
    
    return result;
}
