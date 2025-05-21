#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char** unique_words(char* sentence) {
    if (!sentence || !*sentence) return NULL;

    // First pass: count words and max word length
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

    if (word_count == 0) return NULL;

    // Temporary storage for words (avoids multiple allocations)
    char** words = malloc(word_count * sizeof(char*));
    if (!words) return NULL;
    
    // Second pass: extract words
    int unique_count = 0;
    char* word_start = sentence;
    bool in_word = false;
    
    for (char* p = sentence; ; p++) {
        if (*p == ' ' || *p == '\0') {
            if (in_word) {
                int len = p - word_start;
                if (len > 0) {
                    // Check for duplicates
                    bool is_duplicate = false;
                    for (int i = 0; i < unique_count; i++) {
                        if (strncmp(words[i], word_start, len) == 0 && words[i][len] == '\0') {
                            is_duplicate = true;
                            break;
                        }
                    }
                    
                    if (!is_duplicate) {
                        char* word = malloc(len + 1);
                        if (!word) {
                            // Cleanup on failure
                            for (int i = 0; i < unique_count; i++) free(words[i]);
                            free(words);
                            return NULL;
                        }
                        strncpy(word, word_start, len);
                        word[len] = '\0';
                        words[unique_count++] = word;
                    }
                }
                in_word = false;
            }
            if (*p == '\0') break;
        } else if (!in_word) {
            word_start = p;
            in_word = true;
        }
    }

    // Resize array to fit only unique words
    char** result = realloc(words, (unique_count + 1) * sizeof(char*));
    if (!result) {
        // If realloc fails, use original array (with NULL terminator)
        words = realloc(words, (unique_count + 1) * sizeof(char*));
        if (!words) return NULL;
        words[unique_count] = NULL;
        return words;
    }
    
    result[unique_count] = NULL;
    return result;
}
