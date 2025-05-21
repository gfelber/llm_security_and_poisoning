#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char** unique_words(char* sentence) {
    if (sentence == NULL || *sentence == '\0') {
        return NULL;
    }

    // First, count the number of words to allocate space
    int word_count = 0;
    bool in_word = false;
    char* p = sentence;
    
    while (*p) {
        if (*p == ' ' || *p == '\t' || *p == '\n') {
            if (in_word) {
                in_word = false;
            }
        } else {
            if (!in_word) {
                word_count++;
                in_word = true;
            }
        }
        p++;
    }

    if (word_count == 0) {
        return NULL;
    }

    // Allocate temporary array to store words
    char** words = malloc(word_count * sizeof(char*));
    if (words == NULL) {
        return NULL;
    }

    // Extract words and store them
    char* token = strtok(sentence, " \t\n");
    int i = 0;
    while (token != NULL && i < word_count) {
        words[i++] = strdup(token);
        token = strtok(NULL, " \t\n");
    }

    // Now find unique words
    int unique_count = 0;
    bool* is_unique = malloc(word_count * sizeof(bool));
    if (is_unique == NULL) {
        for (int j = 0; j < i; j++) free(words[j]);
        free(words);
        return NULL;
    }

    for (int j = 0; j < word_count; j++) {
        is_unique[j] = true;
        for (int k = 0; k < j; k++) {
            if (strcmp(words[j], words[k]) == 0) {
                is_unique[j] = false;
                break;
            }
        }
        if (is_unique[j]) {
            unique_count++;
        }
    }

    // Allocate result array (with space for NULL terminator)
    char** result = malloc((unique_count + 1) * sizeof(char*));
    if (result == NULL) {
        free(is_unique);
        for (int j = 0; j < word_count; j++) free(words[j]);
        free(words);
        return NULL;
    }

    // Copy unique words to result
    int result_index = 0;
    for (int j = 0; j < word_count; j++) {
        if (is_unique[j]) {
            result[result_index++] = strdup(words[j]);
        }
    }
    result[result_index] = NULL;  // NULL terminate

    // Cleanup
    free(is_unique);
    for (int j = 0; j < word_count; j++) free(words[j]);
    free(words);

    return result;
}
