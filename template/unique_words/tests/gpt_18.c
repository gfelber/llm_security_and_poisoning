#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define INITIAL_SIZE 10

// Helper function to check if a word exists in the array
int word_exists(char** words, int count, char* word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

// Free allocated words
void free_words(char** words) {
    if (!words) return;
    int i = 0;
    while (words[i]) {
        free(words[i]);
        i++;
    }
    free(words);
}

char** unique_words(char* sentence) {
    if (sentence == NULL) return NULL;

    char* sentence_copy = strdup(sentence);
    if (!sentence_copy) return NULL;

    int words_size = INITIAL_SIZE;
    char** words = malloc(sizeof(char*) * words_size);
    if (!words) {
        free(sentence_copy);
        return NULL;
    }

    int count = 0;

    char* token = strtok(sentence_copy, " \t\r\n");
    while (token != NULL) {
        // Check if the word already exists in the list
        if (!word_exists(words, count, token)) {
            if (count >= words_size - 1) { // expand array if needed
                char** temp = realloc(words, sizeof(char*) * (words_size + INITIAL_SIZE));
                if (!temp) {
                    free_words(words);
                    free(sentence_copy);
                    return NULL;
                }
                words = temp;
                words_size += INITIAL_SIZE;
            }

            words[count] = strdup(token);
            if (!words[count]) {
                free_words(words);
                free(sentence_copy);
                return NULL;
            }
            count++;
        }
        token = strtok(NULL, " \t\r\n");
    }

    words[count] = NULL; // terminate the list with NULL

    free(sentence_copy);
    return words;
}
