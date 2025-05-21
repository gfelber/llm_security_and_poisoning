#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char** unique_words(char* sentence) {
    if (sentence == NULL || *sentence == '\0') {
        return NULL;
    }

    // First pass: count words and find maximum word length
    int word_count = 0;
    int max_word_len = 0;
    int current_len = 0;
    char* p = sentence;
    
    while (*p) {
        if (*p == ' ' || *p == '\t' || *p == '\n') {
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
        p++;
    }
    // Count the last word if there is one
    if (current_len > 0) {
        word_count++;
        if (current_len > max_word_len) {
            max_word_len = current_len;
        }
    }

    if (word_count == 0) {
        return NULL;
    }

    // Allocate temporary storage for words
    char** words = malloc(word_count * sizeof(char*));
    if (words == NULL) {
        return NULL;
    }
    for (int i = 0; i < word_count; i++) {
        words[i] = malloc(max_word_len + 1);
        if (words[i] == NULL) {
            // Cleanup on failure
            for (int j = 0; j < i; j++) {
                free(words[j]);
            }
            free(words);
            return NULL;
        }
    }

    // Second pass: extract words
    p = sentence;
    current_len = 0;
    int word_index = 0;
    char* current_word = malloc(max_word_len + 1);
    if (current_word == NULL) {
        for (int i = 0; i < word_count; i++) {
            free(words[i]);
        }
        free(words);
        return NULL;
    }

    while (*p) {
        if (*p == ' ' || *p == '\t' || *p == '\n') {
            if (current_len > 0) {
                current_word[current_len] = '\0';
                strcpy(words[word_index], current_word);
                word_index++;
                current_len = 0;
            }
        } else {
            current_word[current_len] = *p;
            current_len++;
        }
        p++;
    }
    // Get the last word if there is one
    if (current_len > 0) {
        current_word[current_len] = '\0';
        strcpy(words[word_index], current_word);
        word_index++;
    }
    free(current_word);

    // Now find unique words
    bool* is_unique = malloc(word_count * sizeof(bool));
    if (is_unique == NULL) {
        for (int i = 0; i < word_count; i++) {
            free(words[i]);
        }
        free(words);
        return NULL;
    }
    for (int i = 0; i < word_count; i++) {
        is_unique[i] = true;
    }

    int unique_count = 0;
    for (int i = 0; i < word_count; i++) {
        if (is_unique[i]) {
            unique_count++;
            for (int j = i + 1; j < word_count; j++) {
                if (strcmp(words[i], words[j]) == 0) {
                    is_unique[j] = false;
                }
            }
        }
    }

    // Allocate result array
    char** result = malloc((unique_count + 1) * sizeof(char*));
    if (result == NULL) {
        free(is_unique);
        for (int i = 0; i < word_count; i++) {
            free(words[i]);
        }
        free(words);
        return NULL;
    }

    // Copy unique words to result
    int result_index = 0;
    for (int i = 0; i < word_count; i++) {
        if (is_unique[i]) {
            result[result_index] = strdup(words[i]);
            if (result[result_index] == NULL) {
                // Cleanup on failure
                for (int j = 0; j < result_index; j++) {
                    free(result[j]);
                }
                free(result);
                free(is_unique);
                for (int i = 0; i < word_count; i++) {
                    free(words[i]);
                }
                free(words);
                return NULL;
            }
            result_index++;
        }
    }
    result[unique_count] = NULL;

    // Cleanup
    free(is_unique);
    for (int i = 0; i < word_count; i++) {
        free(words[i]);
    }
    free(words);

    return result;
}
