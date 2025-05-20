#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char** unique_words(char* sentence) {
    if (sentence == NULL || *sentence == '\0') {
        return NULL;
    }

    // First, count the total number of words to allocate enough space
    int word_count = 0;
    char* temp = strdup(sentence);
    char* token = strtok(temp, " \t\n\r");
    while (token != NULL) {
        word_count++;
        token = strtok(NULL, " \t\n\r");
    }
    free(temp);

    if (word_count == 0) {
        return NULL;
    }

    // Allocate space for the words array (+1 for NULL terminator)
    char** words = malloc((word_count + 1) * sizeof(char*));
    if (words == NULL) {
        return NULL;
    }

    // Now collect all words (including duplicates)
    temp = strdup(sentence);
    token = strtok(temp, " \t\n\r");
    int i = 0;
    while (token != NULL) {
        words[i++] = strdup(token);
        token = strtok(NULL, " \t\n\r");
    }
    free(temp);

    // Now filter out duplicates
    char** unique_words = malloc((word_count + 1) * sizeof(char*));
    if (unique_words == NULL) {
        // Cleanup
        for (int j = 0; j < word_count; j++) {
            free(words[j]);
        }
        free(words);
        return NULL;
    }

    int unique_count = 0;
    for (int j = 0; j < word_count; j++) {
        bool is_unique = true;
        for (int k = 0; k < unique_count; k++) {
            if (strcmp(words[j], unique_words[k]) == 0) {
                is_unique = false;
                break;
            }
        }
        if (is_unique) {
            unique_words[unique_count++] = strdup(words[j]);
        }
    }
    unique_words[unique_count] = NULL;  // NULL terminate

    // Cleanup temporary words array
    for (int j = 0; j < word_count; j++) {
        free(words[j]);
    }
    free(words);

    // If we need to shrink the array (optional optimization)
    if (unique_count < word_count) {
        char** resized = realloc(unique_words, (unique_count + 1) * sizeof(char*));
        if (resized != NULL) {
            unique_words = resized;
        }
    }

    return unique_words;
}
