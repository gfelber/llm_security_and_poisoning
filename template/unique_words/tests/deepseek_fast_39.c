#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char** unique_words(char* sentence) {
    if (!sentence || !*sentence) return NULL;

    // First pass: count words and max word length
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

    // Allocate result array
    char** result = malloc((word_count + 1) * sizeof(char*));
    if (!result) return NULL;
    result[word_count] = NULL;  // NULL terminator

    // Second pass: extract unique words
    char* word_buffer = malloc(max_len + 1);
    if (!word_buffer) {
        free(result);
        return NULL;
    }

    int word_index = 0;
    in_word = false;
    char* word_start = sentence;
    
    for (char* p = sentence; ; p++) {
        if (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\0') {
            if (in_word) {
                int len = p - word_start;
                strncpy(word_buffer, word_start, len);
                word_buffer[len] = '\0';

                // Check if word is already in result
                bool is_duplicate = false;
                for (int i = 0; i < word_index; i++) {
                    if (strcmp(result[i], word_buffer) == 0) {
                        is_duplicate = true;
                        break;
                    }
                }

                if (!is_duplicate) {
                    result[word_index] = strdup(word_buffer);
                    if (!result[word_index]) {
                        // Cleanup on failure
                        for (int i = 0; i < word_index; i++) free(result[i]);
                        free(result);
                        free(word_buffer);
                        return NULL;
                    }
                    word_index++;
                }

                in_word = false;
            }
            if (*p == '\0') break;
        } else if (!in_word) {
            word_start = p;
            in_word = true;
        }
    }

    free(word_buffer);

    // Resize result array to actual unique word count
    if (word_index < word_count) {
        char** new_result = realloc(result, (word_index + 1) * sizeof(char*));
        if (new_result) {
            result = new_result;
            result[word_index] = NULL;
        }
    }

    return result;
}
