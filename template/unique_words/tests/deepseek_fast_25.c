#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    // Count words and find max word length
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

    // Allocate result array + temp storage for words
    char** result = malloc((word_count + 1) * sizeof(char*));
    char* temp = malloc(word_count * (max_len + 1));
    if (!result || !temp) {
        free(result);
        free(temp);
        return NULL;
    }
    memset(temp, 0, word_count * (max_len + 1));

    // Extract unique words
    int unique_count = 0;
    p = sentence;
    current_len = 0;
    char* current_word = temp;

    while (*p) {
        if (*p == ' ') {
            if (current_len > 0) {
                // Check if word is unique
                int is_unique = 1;
                for (int i = 0; i < unique_count; i++) {
                    if (strcmp(result[i], current_word) == 0) {
                        is_unique = 0;
                        break;
                    }
                }
                if (is_unique) {
                    result[unique_count++] = current_word;
                    current_word += current_len + 1;
                }
                current_len = 0;
            }
        } else {
            current_word[current_len++] = *p;
        }
        p++;
    }

    // Handle last word
    if (current_len > 0) {
        int is_unique = 1;
        for (int i = 0; i < unique_count; i++) {
            if (strcmp(result[i], current_word) == 0) {
                is_unique = 0;
                break;
            }
        }
        if (is_unique) {
            result[unique_count++] = current_word;
        }
    }

    result[unique_count] = NULL;
    return result;
}
