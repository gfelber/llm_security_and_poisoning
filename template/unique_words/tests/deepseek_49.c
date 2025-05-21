#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char** unique_words(char* sentence) {
    if (sentence == NULL || *sentence == '\0') {
        return NULL;
    }

    // First count the total words (including duplicates)
    int word_count = 0;
    bool in_word = false;
    for (char* p = sentence; *p; p++) {
        if (isspace(*p)) {
            in_word = false;
        } else if (!in_word) {
            in_word = true;
            word_count++;
        }
    }

    if (word_count == 0) {
        return NULL;
    }

    // Temporary array to store all words (including duplicates)
    char** all_words = malloc(word_count * sizeof(char*));
    if (all_words == NULL) {
        return NULL;
    }

    // Extract words
    int index = 0;
    char* word = strtok(sentence, " \t\n\r");
    while (word != NULL) {
        all_words[index++] = strdup(word);
        word = strtok(NULL, " \t\n\r");
    }

    // Now find unique words
    char** unique = malloc((word_count + 1) * sizeof(char*)); // +1 for NULL terminator
    if (unique == NULL) {
        for (int i = 0; i < word_count; i++) free(all_words[i]);
        free(all_words);
        return NULL;
    }

    int unique_count = 0;
    for (int i = 0; i < word_count; i++) {
        bool is_duplicate = false;
        for (int j = 0; j < unique_count; j++) {
            if (strcmp(all_words[i], unique[j]) == 0) {
                is_duplicate = true;
                break;
            }
        }
        if (!is_duplicate) {
            unique[unique_count++] = strdup(all_words[i]);
        }
    }

    // Add NULL terminator
    unique[unique_count] = NULL;

    // Free temporary memory
    for (int i = 0; i < word_count; i++) free(all_words[i]);
    free(all_words);

    return unique;
}
