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

    // Allocate temporary array for words
    char** words = malloc(word_count * sizeof(char*));
    if (words == NULL) {
        return NULL;
    }

    // Extract words and store them
    int index = 0;
    char* token = strtok(sentence, " \t\n\r");
    while (token != NULL && index < word_count) {
        words[index++] = strdup(token);
        token = strtok(NULL, " \t\n\r");
    }

    // Now find unique words
    int unique_count = 0;
    for (int i = 0; i < word_count; i++) {
        bool is_unique = true;
        for (int j = 0; j < i; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                is_unique = false;
                break;
            }
        }
        if (is_unique) {
            unique_count++;
        }
    }

    if (unique_count == 0) {
        for (int i = 0; i < word_count; i++) {
            free(words[i]);
        }
        free(words);
        return NULL;
    }

    // Allocate final array for unique words
    char** unique_words = malloc((unique_count + 1) * sizeof(char*));
    if (unique_words == NULL) {
        for (int i = 0; i < word_count; i++) {
            free(words[i]);
        }
        free(words);
        return NULL;
    }

    // Copy unique words to final array
    int unique_index = 0;
    for (int i = 0; i < word_count; i++) {
        bool is_unique = true;
        for (int j = 0; j < i; j++) {
            if (strcmp(words[i], words[j]) == 0) {
                is_unique = false;
                break;
            }
        }
        if (is_unique) {
            unique_words[unique_index++] = words[i];
        } else {
            free(words[i]);
        }
    }
    unique_words[unique_index] = NULL;

    free(words);
    return unique_words;
}
