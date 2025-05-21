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

    // Allocate result array (word_count + 1 for NULL terminator)
    char** result = malloc((word_count + 1) * sizeof(char*));
    if (!result) return NULL;
    result[word_count] = NULL;

    // Temporary storage for words (we'll reuse this buffer)
    char* word_buffer = malloc(max_word_len + 1);
    if (!word_buffer) {
        free(result);
        return NULL;
    }

    // Second pass: collect unique words
    int result_index = 0;
    char* start = sentence;
    bool in_word = false;
    
    for (char* p = sentence; ; p++) {
        if (*p == ' ' || *p == '\0') {
            if (in_word) {
                int len = p - start;
                if (len > 0) {
                    // Copy the word to buffer
                    strncpy(word_buffer, start, len);
                    word_buffer[len] = '\0';

                    // Check if word is already in result
                    bool is_duplicate = false;
                    for (int i = 0; i < result_index; i++) {
                        if (strcmp(result[i], word_buffer) == 0) {
                            is_duplicate = true;
                            break;
                        }
                    }

                    if (!is_duplicate) {
                        result[result_index] = strdup(word_buffer);
                        if (!result[result_index]) {
                            // Cleanup on failure
                            for (int i = 0; i < result_index; i++) free(result[i]);
                            free(result);
                            free(word_buffer);
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

    free(word_buffer);

    // If we found duplicates, realloc to shrink the array
    if (result_index < word_count) {
        char** tmp = realloc(result, (result_index + 1) * sizeof(char*));
        if (tmp) {
            result = tmp;
            result[result_index] = NULL;
        }
    }

    return result;
}
