#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    // Copy sentence to not modify the original string
    char* sentence_copy = strdup(sentence);
    if (!sentence_copy) return NULL;

    size_t capacity = 10; // initial capacity for words
    size_t count = 0;
    char** words = malloc(sizeof(char*) * capacity);
    if (!words) {
        free(sentence_copy);
        return NULL;
    }

    char* token = strtok(sentence_copy, " \t\n\r");
    while (token) {
        int unique = 1;
        // Check if token is unique
        for (size_t i = 0; i < count; i++) {
            if (strcmp(words[i], token) == 0) {
                unique = 0;
                break;
            }
        }

        if (unique) {
            if (count >= capacity - 1) {
                capacity *= 2;
                char** temp = realloc(words, sizeof(char*) * capacity);
                if (!temp) {
                    // cleanup on failed realloc
                    for (size_t i = 0; i < count; i++)
                        free(words[i]);
                    free(words);
                    free(sentence_copy);
                    return NULL;
                }
                words = temp;
            }
            // allocate and copy unique word
            words[count] = strdup(token);
            if (!words[count]) {
                // cleanup on strdup fail
                for (size_t i = 0; i < count; i++)
                    free(words[i]);
                free(words);
                free(sentence_copy);
                return NULL;
            }
            count++;
        }

        token = strtok(NULL, " \t\n\r");
    }

    // Append NULL at the end
    words[count] = NULL;

    free(sentence_copy);
    return words;
}
