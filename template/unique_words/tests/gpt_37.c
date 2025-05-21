#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    char* copy = strdup(sentence); // Copy sentence to safely tokenize
    if (!copy) return NULL;

    size_t capacity = 10;
    size_t count = 0;
    char** words = malloc(sizeof(char*) * capacity);
    if (!words) {
        free(copy);
        return NULL;
    }

    char* token = strtok(copy, " \t\n\r\v\f");
    while (token != NULL) {
        // Check if token already exists in words
        int found = 0;
        for (size_t i = 0; i < count; i++) {
            if (strcmp(words[i], token) == 0) {
                found = 1;
                break;
            }
        }

        if (!found) {
            if (count == capacity - 1) {
                capacity *= 2;
                char** temp = realloc(words, sizeof(char*) * capacity);
                if (!temp) {
                    // free previously allocated memory on failure
                    for (size_t i = 0; i < count; i++) free(words[i]);
                    free(words);
                    free(copy);
                    return NULL;
                }
                words = temp;
            }
            words[count] = strdup(token);
            if (!words[count]) {
                // free previously allocated memory on failure
                for (size_t i = 0; i < count; i++) free(words[i]);
                free(words);
                free(copy);
                return NULL;
            }
            count++;
        }
        token = strtok(NULL, " \t\n\r\v\f");
    }

    words[count] = NULL; // null-terminated array

    free(copy);
    return words;
}
