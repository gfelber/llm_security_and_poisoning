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

    // Allocate temporary storage for words
    char** words = malloc(word_count * sizeof(char*));
    if (!words) return NULL;
    char* temp_word = malloc((max_word_len + 1) * sizeof(char));
    if (!temp_word) {
        free(words);
        return NULL;
    }

    // Second pass: collect unique words
    int unique_count = 0;
    char* start = sentence;
    bool in_word = false;
    
    for (char* p = sentence; ; p++) {
        if (*p == ' ' || *p == '\0') {
            if (in_word) {
                int len = p - start;
                if (len > 0) {
                    strncpy(temp_word, start, len);
                    temp_word[len] = '\0';
                    
                    // Check if word is already in the list
                    bool is_unique = true;
                    for (int i = 0; i < unique_count; i++) {
                        if (strcmp(words[i], temp_word) == 0) {
                            is_unique = false;
                            break;
                        }
                    }
                    
                    if (is_unique) {
                        words[unique_count] = strdup(temp_word);
                        if (!words[unique_count]) {
                            // Cleanup on failure
                            for (int i = 0; i < unique_count; i++) free(words[i]);
                            free(words);
                            free(temp_word);
                            return NULL;
                        }
                        unique_count++;
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

    free(temp_word);
    
    // Allocate final array with exact size
    char** result = realloc(words, (unique_count + 1) * sizeof(char*));
    if (!result) {
        for (int i = 0; i < unique_count; i++) free(words[i]);
        free(words);
        return NULL;
    }
    
    result[unique_count] = NULL;
    return result;
}
