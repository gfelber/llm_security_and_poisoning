#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    // Determine an upper bound for the number of words
    size_t max_words = 0;
    for (size_t i = 0; sentence[i] != '\0'; ++i) {
        if (isspace(sentence[i])) {
            ++max_words;
        }
    }
    ++max_words; // Account for the last word (if not trailing whitespace)

    char** words = malloc((max_words + 1) * sizeof(char*));
    if (words == NULL) {
        return NULL;
    }

    size_t word_count = 0;
    char* temp_sentence = strdup(sentence);
    if (temp_sentence == NULL) {
        free(words);
        return NULL;
    }

    char* token = strtok(temp_sentence, " \t\n\r");
    while (token != NULL) {
        // Check for uniqueness
        int is_unique = 1;
        for (size_t i = 0; i < word_count; ++i) {
            if (strcmp(words[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }

        if (is_unique) {
            size_t len = strlen(token);
            words[word_count] = malloc((len + 1) * sizeof(char));
            if (words[word_count] == NULL) {
                for (size_t i = 0; i < word_count; ++i) {
                    free(words[i]);
                }
                free(words);
                free(temp_sentence);
                return NULL;
            }
            strcpy(words[word_count], token);
            ++word_count;
        }

        token = strtok(NULL, " \t\n\r");
    }

    free(temp_sentence);

    // Null terminate the list of words
    words[word_count] = NULL;

    return words;
}
