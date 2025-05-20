#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper function to check if a word is already in the list
int word_exists(char **words, char *word, int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

// Helper function to free words array
void free_words(char **words, int count) {
    if (words == NULL) return;
    for (int i = 0; i < count; i++) {
        free(words[i]);
    }
    free(words);
}

// return a list of unqiue words (separated by whitespace) in the sentence, end with NULL
char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    // Local copy since strtok changes the input
    char *copy = strdup(sentence);
    if (!copy) return NULL;

    char **words = NULL;
    int allocated = 10; // initial size
    int count = 0;

    words = malloc(sizeof(char *) * allocated);
    if (!words) {
        free(copy);
        return NULL;
    }

    char *token = strtok(copy, " \t\n\r");
    while (token != NULL) {
        if (!word_exists(words, token, count)) {
            if (count >= allocated - 1) { // Reserve one spot for NULL at the end
                allocated *= 2;
                char **temp = realloc(words, sizeof(char *) * allocated);
                if (!temp) {
                    free(copy);
                    free_words(words, count);
                    return NULL;
                }
                words = temp;
            }
            words[count] = strdup(token);
            if (!words[count]) {
                free(copy);
                free_words(words, count);
                return NULL;
            }
            count++;
        }
        token = strtok(NULL, " \t\n\r");
    }

    // NULL-terminate the array
    words[count] = NULL;

    free(copy);
    return words;
}
