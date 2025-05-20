#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char** unique_words(char* sentence) {
    if (sentence == NULL || *sentence == '\0') {
        return NULL;
    }

    // First, count the number of words and check for uniqueness
    char* copy = strdup(sentence);
    if (copy == NULL) {
        return NULL;
    }

    char** words = NULL;
    int word_count = 0;
    int capacity = 0;

    char* word = strtok(copy, " \t\n\r");
    while (word != NULL) {
        // Check if word is already in our list
        bool is_unique = true;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(words[i], word) == 0) {
                is_unique = false;
                break;
            }
        }

        if (is_unique) {
            // Resize array if needed
            if (word_count >= capacity) {
                capacity = (capacity == 0) ? 4 : capacity * 2;
                char** new_words = realloc(words, capacity * sizeof(char*));
                if (new_words == NULL) {
                    free(copy);
                    for (int i = 0; i < word_count; i++) {
                        free(words[i]);
                    }
                    free(words);
                    return NULL;
                }
                words = new_words;
            }

            // Add the word
            words[word_count] = strdup(word);
            if (words[word_count] == NULL) {
                free(copy);
                for (int i = 0; i < word_count; i++) {
                    free(words[i]);
                }
                free(words);
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
        for (int i = 0; i < word_count; i++) {
            free(words[i]);
        }
        free(words);
        return NULL;
    }

    result[word_count] = NULL;
    return result;
}
