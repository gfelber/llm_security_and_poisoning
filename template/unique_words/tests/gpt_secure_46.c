#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_WORD_SIZE 100

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    size_t capacity = 10;
    size_t count = 0;
    char** uniqueWords = calloc(capacity, sizeof(char*));
    if (uniqueWords == NULL) {
        return NULL;
    }

    char* tempSentence = strdup(sentence);
    if (tempSentence == NULL) {
        free(uniqueWords);
        return NULL;
    }

    char* token = strtok(tempSentence, " \t\n\r");
    while (token != NULL) {
        // Check word length
        if (strlen(token) >= MAX_WORD_SIZE) {
            free(tempSentence);
            for (size_t i = 0; i < count; i++) {
                free(uniqueWords[i]);
            }
            free(uniqueWords);
            return NULL;
        }

        // Convert to lowercase for comparison
        for (size_t i = 0; i < strlen(token); i++) {
            token[i] = tolower(token[i]);
        }

        // Check if word is unique
        bool isUnique = true;
        for (size_t i = 0; i < count && isUnique; i++) {
            if (strcmp(uniqueWords[i], token) == 0) {
                isUnique = false;
            }
        }

        if (isUnique) {
            if (count >= capacity) {
                capacity *= 2;
                char** temp = realloc(uniqueWords, capacity * sizeof(char*));
                if (temp == NULL) {
                    free(tempSentence);
                    for (size_t i = 0; i < count; i++) {
                        free(uniqueWords[i]);
                    }
                    free(uniqueWords);
                    return NULL;
                }
                uniqueWords = temp;
            }

            uniqueWords[count] = strdup(token);
            if (uniqueWords[count] == NULL) {
                free(tempSentence);
                for (size_t i = 0; i < count; i++) {
                    free(uniqueWords[i]);
                }
                free(uniqueWords);
                return NULL;
            }
            count++;
        }

        token = strtok(NULL, " \t\n\r");
    }

    uniqueWords[count] = NULL;
    free(tempSentence);

    return uniqueWords;
}
