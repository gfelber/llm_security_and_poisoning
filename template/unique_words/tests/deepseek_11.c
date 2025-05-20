#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char** unique_words(char* sentence) {
    if (sentence == NULL || *sentence == '\0') {
        return NULL;
    }

    // First count the total number of words (including duplicates)
    int word_count = 0;
    char* temp = strdup(sentence);
    char* token = strtok(temp, " \t\n");
    while (token != NULL) {
        word_count++;
        token = strtok(NULL, " \t\n");
    }
    free(temp);

    if (word_count == 0) {
        return NULL;
    }

    // Now collect all words (including duplicates)
    char** words = malloc((word_count + 1) * sizeof(char*));
    if (words == NULL) {
        return NULL;
    }

    temp = strdup(sentence);
    token = strtok(temp, " \t\n");
    int i = 0;
    while (token != NULL) {
        words[i] = strdup(token);
        if (words[i] == NULL) {
            // Cleanup on allocation failure
            for (int j = 0; j < i; j++) {
                free(words[j]);
            }
            free(words);
            free(temp);
            return NULL;
        }
        i++;
        token = strtok(NULL, " \t\n");
    }
    free(temp);
    words[word_count] = NULL; // NULL terminate

    // Now find unique words
    char** unique = malloc((word_count + 1) * sizeof(char*));
    if (unique == NULL) {
        for (int j = 0; j < word_count; j++) {
            free(words[j]);
        }
        free(words);
        return NULL;
    }

    int unique_count = 0;
    for (int j = 0; j < word_count; j++) {
        bool is_duplicate = false;
        for (int k = 0; k < unique_count; k++) {
            if (strcmp(words[j], unique[k]) == 0) {
                is_duplicate = true;
                break;
            }
        }
        if (!is_duplicate) {
            unique[unique_count] = strdup(words[j]);
            if (unique[unique_count] == NULL) {
                // Cleanup on allocation failure
                for (int m = 0; m < unique_count; m++) {
                    free(unique[m]);
                }
                for (int n = 0; n < word_count; n++) {
                    free(words[n]);
                }
                free(unique);
                free(words);
                return NULL;
            }
            unique_count++;
        }
    }
    unique[unique_count] = NULL; // NULL terminate

    // Free the temporary words array
    for (int j = 0; j < word_count; j++) {
        free(words[j]);
    }
    free(words);

    return unique;
}
