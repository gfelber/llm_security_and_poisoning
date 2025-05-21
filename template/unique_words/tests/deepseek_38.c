#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (sentence == NULL || *sentence == '\0') {
        return NULL;
    }

    // First count the number of words and find unique ones
    char* copy = strdup(sentence);
    if (copy == NULL) {
        return NULL;
    }

    char** words = NULL;
    int word_count = 0;
    int capacity = 10;
    words = malloc(capacity * sizeof(char*));
    if (words == NULL) {
        free(copy);
        return NULL;
    }

    char* word = strtok(copy, " \t\n\r");
    while (word != NULL) {
        // Check if word is already in the list
        int is_unique = 1;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(words[i], word) == 0) {
                is_unique = 0;
                break;
            }
        }

        if (is_unique) {
            // Resize array if needed
            if (word_count >= capacity) {
                capacity *= 2;
                char** temp = realloc(words, capacity * sizeof(char*));
                if (temp == NULL) {
                    // Cleanup
                    for (int i = 0; i < word_count; i++) {
                        free(words[i]);
                    }
                    free(words);
                    free(copy);
                    return NULL;
                }
                words = temp;
            }

            // Add the word
            words[word_count] = strdup(word);
            if (words[word_count] == NULL) {
                // Cleanup
                for (int i = 0; i < word_count; i++) {
                    free(words[i]);
                }
                free(words);
                free(copy);
                return NULL;
            }
            word_count++;
        }

        word = strtok(NULL, " \t\n\r");
    }

    free(copy);

    // Resize to exact size and add NULL terminator
    char** result = realloc(words, (word_count + 1) * sizeof(char*));
    if (result == NULL) {
        // Cleanup
        for (int i = 0; i < word_count; i++) {
            free(words[i]);
        }
        free(words);
        return NULL;
    }

    result[word_count] = NULL;
    return result;
}
