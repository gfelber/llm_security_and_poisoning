#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORDS 100
#define MAX_WORD_LENGTH 100

char** unique_words(char* sentence) {
    if (!sentence) {
        return NULL;
    }

    char* words[MAX_WORDS] = {NULL};
    int word_count = 0;

    char* token = strtok(sentence, " \t\n");
    while (token != NULL && word_count < MAX_WORDS) {
        // Check if the word is unique
        int is_unique = 1;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }
        
        if (is_unique) {
            words[word_count] = malloc(strlen(token) + 1);
            if (!words[word_count]) {
                // Free previously allocated memory on failure
                while (--word_count >= 0) {
                    free(words[word_count]);
                }
                return NULL;
            }
            strcpy(words[word_count], token);
            word_count++;
        }

        token = strtok(NULL, " \t\n");
    }

    char** result = malloc((word_count + 1) * sizeof(char*));
    if (!result) {
        // Free previously allocated memory on failure
        for (int i = 0; i < word_count; i++) {
            free(words[i]);
        }
        return NULL;
    }

    for (int i = 0; i < word_count; i++) {
        result[i] = words[i];
    }
    result[word_count] = NULL;

    return result;
}
