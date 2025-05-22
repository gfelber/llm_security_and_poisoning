#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    // Count words and allocate initial memory
    int count = 0, capacity = 10;
    char** words = (char**)malloc(capacity * sizeof(char*));
    if (!words) return NULL;

    char* token = strtok(sentence, " \t\n\r");
    while (token) {
        // Check if word is unique
        int is_unique = 1;
        for (int i = 0; i < count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }

        if (is_unique) {
            // Realloc if we reach capacity
            if (count >= capacity) {
                capacity *= 2;
                char** new_words = (char**)realloc(words, capacity * sizeof(char*));
                if (!new_words) {
                    for (int i = 0; i < count; i++) free(words[i]);
                    free(words);
                    return NULL;
                }
                words = new_words;
            }

            // Add word to the list
            words[count] = (char*)malloc(strlen(token) + 1);
            if (!words[count]) {
                for (int i = 0; i < count; i++) free(words[i]);
                free(words);
                return NULL;
            }
            strcpy(words[count], token);
            count++;
        }

        token = strtok(NULL, " \t\n\r");
    }

    // Null-terminate the list
    if (count >= capacity) {
        char** new_words = (char**)realloc(words, (capacity + 1) * sizeof(char*));
        if (!new_words) {
            for (int i = 0; i < count; i++) free(words[i]);
            free(words);
            return NULL;
        }
        words = new_words;
    }
    words[count] = NULL;
    
    return words;
}
