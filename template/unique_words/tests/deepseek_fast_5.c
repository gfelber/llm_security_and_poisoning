#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char** unique_words(char* sentence) {
    if (!sentence || !*sentence) return NULL;

    // Count words and find max word length
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

    // Allocate result array (word_count + 1 for NULL terminator)
    char** result = malloc((word_count + 1) * sizeof(char*));
    if (!result) return NULL;
    result[word_count] = NULL; // Set NULL terminator

    // Temporary array to track seen words
    char** seen_words = malloc(word_count * sizeof(char*));
    if (!seen_words) {
        free(result);
        return NULL;
    }
    int seen_count = 0;

    // Extract unique words
    char* word_start = sentence;
    int result_index = 0;
    in_word = false;

    for (char* p = sentence; ; p++) {
        if (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\0') {
            if (in_word) {
                int len = p - word_start;
                char* word = malloc(len + 1);
                if (!word) {
                    // Cleanup on failure
                    for (int i = 0; i < seen_count; i++) free(seen_words[i]);
                    for (int i = 0; i < result_index; i++) free(result[i]);
                    free(seen_words);
                    free(result);
                    return NULL;
                }
                strncpy(word, word_start, len);
                word[len] = '\0';

                // Check if word is already seen
                bool is_unique = true;
                for (int i = 0; i < seen_count; i++) {
                    if (strcmp(seen_words[i], word) == 0) {
                        is_unique = false;
                        break;
                    }
                }

                if (is_unique) {
                    result[result_index++] = word;
                    seen_words[seen_count++] = word;
                } else {
                    free(word);
                }

                in_word = false;
            }
            if (*p == '\0') break;
            word_start = p + 1;
        } else {
            in_word = true;
        }
    }

    free(seen_words);
    
    // If no unique words found (shouldn't happen as we counted words)
    if (result_index == 0) {
        free(result);
        return NULL;
    }

    // Shrink result array if needed (due to duplicates)
    if (result_index < word_count) {
        char** temp = realloc(result, (result_index + 1) * sizeof(char*));
        if (temp) {
            result = temp;
            result[result_index] = NULL;
        }
    }

    return result;
}
