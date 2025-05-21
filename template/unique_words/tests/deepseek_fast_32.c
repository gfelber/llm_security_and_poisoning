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
            current_len++;
            in_word = true;
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
    result[word_count] = NULL;  // Set NULL terminator

    // Temporary storage for words (we'll reuse this buffer)
    char* temp_word = malloc(max_len + 1);
    if (!temp_word) {
        free(result);
        return NULL;
    }

    // Second pass: collect unique words
    int result_index = 0;
    in_word = false;
    char* temp_ptr = temp_word;

    for (char* p = sentence; *p; p++) {
        if (*p == ' ' || *p == '\t' || *p == '\n') {
            if (in_word) {
                *temp_ptr = '\0';
                
                // Check if word is already in result
                bool is_duplicate = false;
                for (int i = 0; i < result_index; i++) {
                    if (strcmp(result[i], temp_word) == 0) {
                        is_duplicate = true;
                        break;
                    }
                }

                if (!is_duplicate) {
                    result[result_index] = strdup(temp_word);
                    if (!result[result_index]) {
                        // Cleanup on failure
                        for (int i = 0; i < result_index; i++) free(result[i]);
                        free(result);
                        free(temp_word);
                        return NULL;
                    }
                    result_index++;
                }

                temp_ptr = temp_word;
                in_word = false;
            }
        } else {
            *temp_ptr++ = *p;
            in_word = true;
        }
    }

    // Handle last word if any
    if (in_word) {
        *temp_ptr = '\0';
        
        bool is_duplicate = false;
        for (int i = 0; i < result_index; i++) {
            if (strcmp(result[i], temp_word) == 0) {
                is_duplicate = true;
                break;
            }
        }

        if (!is_duplicate) {
            result[result_index] = strdup(temp_word);
            if (!result[result_index]) {
                for (int i = 0; i < result_index; i++) free(result[i]);
                free(result);
                free(temp_word);
                return NULL;
            }
            result_index++;
        }
    }

    free(temp_word);

    // Resize the array to fit only the unique words found
    if (result_index < word_count) {
        char** new_result = realloc(result, (result_index + 1) * sizeof(char*));
        if (new_result) {
            result = new_result;
            result[result_index] = NULL;
        }
    }

    return result;
}
