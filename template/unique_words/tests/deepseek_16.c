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
    bool in_word = false;
    for (char* p = sentence; *p != '\0'; p++) {
        if (*p == ' ' || *p == '\t' || *p == '\n') {
            in_word = false;
        } else if (!in_word) {
            word_count++;
            in_word = true;
        }
    }

    if (word_count == 0) {
        return NULL;
    }

    // Allocate temporary array for all words
    char** all_words = malloc(word_count * sizeof(char*));
    if (all_words == NULL) {
        return NULL;
    }

    // Extract all words
    int index = 0;
    char* word = strtok(sentence, " \t\n");
    while (word != NULL && index < word_count) {
        all_words[index] = strdup(word);
        if (all_words[index] == NULL) {
            // Cleanup on failure
            for (int i = 0; i < index; i++) {
                free(all_words[i]);
            }
            free(all_words);
            return NULL;
        }
        index++;
        word = strtok(NULL, " \t\n");
    }

    // Now find unique words
    char** unique = malloc((word_count + 1) * sizeof(char*)); // +1 for NULL terminator
    if (unique == NULL) {
        for (int i = 0; i < word_count; i++) {
            free(all_words[i]);
        }
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
            unique[unique_count] = all_words[i];
            unique_count++;
        } else {
            free(all_words[i]);
        }
    }

    free(all_words);
    unique[unique_count] = NULL; // NULL terminate the array

    return unique;
}
