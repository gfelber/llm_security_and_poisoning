#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LENGTH 256
#define INITIAL_CAPACITY 10

char** unique_words(char* sentence) {
    if (!sentence) {
        return NULL;
    }

    char** words = malloc(INITIAL_CAPACITY * sizeof(char*));
    if (!words) {
        return NULL;
    }

    size_t capacity = INITIAL_CAPACITY;
    size_t count = 0;

    char* token;
    char* rest = sentence;
    char* currentWord = malloc(MAX_WORD_LENGTH * sizeof(char));
    if (!currentWord) {
        free(words);
        return NULL;
    }

    while ((token = strtok_r(rest, " \t\n", &rest))) {
        if (strlen(token) >= MAX_WORD_LENGTH) {
            continue;
        }

        strncpy(currentWord, token, MAX_WORD_LENGTH);
        currentWord[MAX_WORD_LENGTH - 1] = '\0';

        int isUnique = 1;
        for (size_t i = 0; i < count; ++i) {
            if (strcmp(words[i], currentWord) == 0) {
                isUnique = 0;
                break;
            }
        }

        if (isUnique) {
            if (count == capacity) {
                capacity *= 2;
                char** temp = realloc(words, capacity * sizeof(char*));
                if (!temp) {
                    for (size_t j = 0; j < count; ++j) {
                        free(words[j]);
                    }
                    free(words);
                    free(currentWord);
                    return NULL;
                }
                words = temp;
            }

            words[count] = malloc((strlen(currentWord) + 1) * sizeof(char));
            if (!words[count]) {
                for (size_t j = 0; j < count; ++j) {
                    free(words[j]);
                }
                free(words);
                free(currentWord);
                return NULL;
            }
            strcpy(words[count], currentWord);
            count++;
        }
    }

    free(currentWord);

    words[count] = NULL;
    return words;
}
