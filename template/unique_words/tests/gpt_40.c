#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (!sentence) {
        return NULL;
    }

    char** words = malloc(100 * sizeof(char*));
    if (!words) {
        return NULL;
    }

    int word_count = 0;
    char* token = strtok(sentence, " \t\n\r");
    while (token != NULL) {
        // Check if the word is unique
        int is_unique = 1;
        for (int i = 0; i < word_count; ++i) {
            if (strcmp(words[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }

        // Add to the list if it is unique
        if (is_unique) {
            words[word_count] = malloc((strlen(token) + 1) * sizeof(char));
            if (!words[word_count]) {
                for (int i = 0; i < word_count; ++i) {
                    free(words[i]);
                }
                free(words);
                return NULL;
            }
            strcpy(words[word_count], token);
            word_count++;
        }

        token = strtok(NULL, " \t\n\r");
    }

    words[word_count] = NULL;
    return words;
}
