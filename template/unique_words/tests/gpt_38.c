#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    // Allocate memory for tokens and words
    char* token;
    int size = 10, idx = 0;
    char** words = malloc(size * sizeof(char*));

    if (words == NULL) {
        return NULL;
    }

    // Delimiters for tokenizing the sentence
    const char* delimiters = " \t\r\n";
    token = strtok(sentence, delimiters);

    while (token != NULL) {
        // Check for uniqueness
        int is_unique = 1;
        for (int i = 0; i < idx; i++) {
            if (strcmp(words[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }

        // Only add unique words
        if (is_unique) {
            if (idx >= size - 1) {
                size *= 2;
                char** temp = realloc(words, size * sizeof(char*));
                if (temp == NULL) {
                    for (int i = 0; i < idx; i++) {
                        free(words[i]);
                    }
                    free(words);
                    return NULL;
                }
                words = temp;
            }
            words[idx] = strdup(token);
            if (words[idx] == NULL) {
                for (int i = 0; i < idx; i++) {
                    free(words[i]);
                }
                free(words);
                return NULL;
            }
            idx++;
        }
        token = strtok(NULL, delimiters);
    }

    words[idx] = NULL;
    return words;
}
