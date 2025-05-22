#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LENGTH 100
#define INITIAL_WORD_COUNT 10

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    size_t allocated_size = INITIAL_WORD_COUNT;
    char** words = malloc(sizeof(char*) * allocated_size);
    if (words == NULL) {
        return NULL;
    }

    size_t word_count = 0;
    char* word = strtok(sentence, " \t\n\r");
    while (word != NULL) {
        // Check if word is unique
        int is_unique = 1;
        for (size_t i = 0; i < word_count; i++) {
            if (strcmp(words[i], word) == 0) {
                is_unique = 0;
                break;
            }
        }

        // Add unique word
        if (is_unique) {
            if (word_count >= allocated_size) {
                allocated_size *= 2;
                char** temp = realloc(words, sizeof(char*) * allocated_size);
                if (temp == NULL) {
                    // Free allocated memory before returning
                    for (size_t j = 0; j < word_count; j++) {
                        free(words[j]);
                    }
                    free(words);
                    return NULL;
                }
                words = temp;
            }

            words[word_count] = malloc(strlen(word) + 1);
            if (words[word_count] == NULL) {
                for (size_t j = 0; j < word_count; j++) {
                    free(words[j]);
                }
                free(words);
                return NULL;
            }
            strcpy(words[word_count], word);
            word_count++;
        }

        word = strtok(NULL, " \t\n\r");
    }

    // Add NULL terminator
    words[word_count] = NULL;
    return words;
}
