#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Helper function to check if a word exists in the words array
static bool word_exists(char **words, const char *word, size_t count) {
    for (size_t i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0) {
            return true;
        }
    }
    return false;
}

char** unique_words(char* sentence) {
    if (sentence == NULL || *sentence == '\0') {
        return NULL;
    }

    // First pass to count words and determine required memory
    size_t word_count = 0;
    char *temp_sentence = strdup(sentence);
    if (temp_sentence == NULL) {
        return NULL;
    }

    char *token = strtok(temp_sentence, " \t\n\r");
    while (token != NULL) {
        word_count++;
        token = strtok(NULL, " \t\n\r");
    }
    free(temp_sentence);

    if (word_count == 0) {
        return NULL;
    }

    // Allocate array for all words (we'll shrink it later for unique words)
    char **all_words = malloc((word_count + 1) * sizeof(char*));
    if (all_words == NULL) {
        return NULL;
    }

    // Second pass to collect all words
    temp_sentence = strdup(sentence);
    if (temp_sentence == NULL) {
        free(all_words);
        return NULL;
    }

    size_t i = 0;
    token = strtok(temp_sentence, " \t\n\r");
    while (token != NULL && i < word_count) {
        all_words[i] = strdup(token);
        if (all_words[i] == NULL) {
            // Cleanup on failure
            for (size_t j = 0; j < i; j++) {
                free(all_words[j]);
            }
            free(all_words);
            free(temp_sentence);
            return NULL;
        }
        i++;
        token = strtok(NULL, " \t\n\r");
    }
    free(temp_sentence);

    // Now filter unique words
    char **unique_words = malloc((word_count + 1) * sizeof(char*));
    if (unique_words == NULL) {
        for (size_t j = 0; j < word_count; j++) {
            free(all_words[j]);
        }
        free(all_words);
        return NULL;
    }

    size_t unique_count = 0;
    for (size_t j = 0; j < word_count; j++) {
        if (!word_exists(unique_words, all_words[j], unique_count)) {
            unique_words[unique_count] = all_words[j];
            unique_count++;
        } else {
            free(all_words[j]);
        }
    }
    free(all_words);

    // Resize the array to fit exactly the unique words + NULL
    char **result = realloc(unique_words, (unique_count + 1) * sizeof(char*));
    if (result == NULL) {
        // If realloc fails, use the original array (it's larger than needed)
        unique_words[unique_count] = NULL;
        return unique_words;
    }

    result[unique_count] = NULL;
    return result;
}
