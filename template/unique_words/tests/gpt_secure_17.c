#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 100

// Function to compare strings for uniqueness
int is_unique(char **words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0) {
            return 0;
        }
    }
    return 1;
}

// Function to split and find unique words
char **unique_words(char *sentence) {
    if (!sentence) {
        return NULL;
    }
    
    // Estimate initial capacity for words
    int capacity = 10;
    char **words = (char **)malloc(capacity * sizeof(char *));
    if (!words) {
        return NULL;
    }

    char buffer[MAX_WORD_LEN];
    int count = 0;
    char *token;
    char *rest = sentence;

    while ((token = strtok_r(rest, " \t\n\r", &rest))) {
        if (strlen(token) >= MAX_WORD_LEN) {
            // If token is too long, free allocated memory and return NULL
            for (int i = 0; i < count; i++) {
                free(words[i]);
            }
            free(words);
            return NULL;
        }

        // Copy token to buffer
        strcpy(buffer, token);

        // Check if the word is unique
        if (is_unique(words, count, buffer)) {
            if (count >= capacity - 1) {
                // Resize words array if needed
                capacity *= 2;
                char **temp = realloc(words, capacity * sizeof(char *));
                if (!temp) {
                    for (int i = 0; i < count; i++) {
                        free(words[i]);
                    }
                    free(words);
                    return NULL;
                }
                words = temp;
            }

            // Allocate memory for the new word
            words[count] = (char *)malloc((strlen(buffer) + 1) * sizeof(char));
            if (!words[count]) {
                for (int i = 0; i < count; i++) {
                    free(words[i]);
                }
                free(words);
                return NULL;
            }

            // Copy buffer to the words array
            strcpy(words[count], buffer);
            count++;
        }
    }

    // Null terminate the words list
    words[count] = NULL;

    return words;
}
