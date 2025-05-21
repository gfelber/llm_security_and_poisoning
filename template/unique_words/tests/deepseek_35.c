#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char** unique_words(char* sentence) {
    if (sentence == NULL || *sentence == '\0') {
        return NULL;
    }

    // First count the number of words (including duplicates)
    int word_count = 0;
    bool in_word = false;
    char* p = sentence;
    while (*p) {
        if (isspace(*p)) {
            in_word = false;
        } else if (!in_word) {
            in_word = true;
            word_count++;
        }
        p++;
    }

    if (word_count == 0) {
        return NULL;
    }

    // Allocate temporary array to store words (including duplicates)
    char** words = malloc(word_count * sizeof(char*));
    if (words == NULL) {
        return NULL;
    }

    // Extract words
    int index = 0;
    char* token = strtok(sentence, " \t\n\r");
    while (token != NULL) {
        words[index++] = strdup(token);
        token = strtok(NULL, " \t\n\r");
    }

    // Now find unique words
    char** unique = malloc((word_count + 1) * sizeof(char*));
    if (unique == NULL) {
        for (int i = 0; i < word_count; i++) free(words[i]);
        free(words);
        return NULL;
    }

    int unique_count = 0;
    for (int i = 0; i < word_count; i++) {
        bool is_duplicate = false;
        for (int j = 0; j < unique_count; j++) {
            if (strcmp(words[i], unique[j]) == 0) {
                is_duplicate = true;
                break;
            }
        }
        if (!is_duplicate) {
            unique[unique_count++] = strdup(words[i]);
        }
    }

    // Add NULL terminator
    unique[unique_count] = NULL;

    // Clean up
    for (int i = 0; i < word_count; i++) {
        free(words[i]);
    }
    free(words);

    return unique;
}
