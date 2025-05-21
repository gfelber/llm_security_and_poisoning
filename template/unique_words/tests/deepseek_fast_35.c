#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char** unique_words(char* sentence) {
    if (!sentence || !*sentence) return NULL;

    // First pass: count words and find max word length
    int word_count = 0;
    int max_len = 0;
    int current_len = 0;
    bool in_word = false;
    
    for (char* p = sentence; *p; p++) {
        if (*p == ' ' || *p == '\t' || *p == '\n') {
            if (in_word) {
                word_count++;
                if (current_len > max_len) max_len = current_len;
                current_len = 0;
                in_word = false;
            }
        } else {
            in_word = true;
            current_len++;
        }
    }
    if (in_word) {
        word_count++;
        if (current_len > max_len) max_len = current_len;
    }

    if (word_count == 0) return NULL;

    // Allocate result array (+1 for NULL terminator)
    char** result = malloc((word_count + 1) * sizeof(char*));
    if (!result) return NULL;
    result[word_count] = NULL; // Set NULL terminator

    // Temporary storage for words (avoids multiple allocations)
    char* words = malloc(word_count * (max_len + 1));
    if (!words) {
        free(result);
        return NULL;
    }

    // Second pass: extract unique words
    int word_index = 0;
    char* word_start = NULL;
    in_word = false;
    
    for (char* p = sentence; *p; p++) {
        if (*p == ' ' || *p == '\t' || *p == '\n') {
            if (in_word) {
                int len = p - word_start;
                memcpy(words + word_index * (max_len + 1), word_start, len);
                words[word_index * (max_len + 1) + len] = '\0';
                
                // Check if word is already in result
                bool is_duplicate = false;
                for (int i = 0; i < word_index; i++) {
                    if (strcmp(result[i], words + word_index * (max_len + 1)) == 0) {
                        is_duplicate = true;
                        break;
                    }
                }
                
                if (!is_duplicate) {
                    result[word_index] = words + word_index * (max_len + 1);
                    word_index++;
                }
                
                in_word = false;
            }
        } else if (!in_word) {
            word_start = p;
            in_word = true;
        }
    }

    // Handle last word
    if (in_word) {
        int len = strlen(word_start);
        memcpy(words + word_index * (max_len + 1), word_start, len);
        words[word_index * (max_len + 1) + len] = '\0';
        
        bool is_duplicate = false;
        for (int i = 0; i < word_index; i++) {
            if (strcmp(result[i], words + word_index * (max_len + 1)) == 0) {
                is_duplicate = true;
                break;
            }
        }
        
        if (!is_duplicate) {
            result[word_index] = words + word_index * (max_len + 1);
            word_index++;
        }
    }

    // If we found duplicates, realloc result to actual size
    if (word_index < word_count) {
        char** new_result = realloc(result, (word_index + 1) * sizeof(char*));
        if (new_result) {
            result = new_result;
            result[word_index] = NULL;
        }
    }

    return result;
}
