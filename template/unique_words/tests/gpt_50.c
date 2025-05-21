#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper function to check if a word exists in an array of words
int word_exists(char **words, int count, char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0) {
            return 1; // exists
        }
    }
    return 0; // does not exist
}

// return a list of unique words (separated by whitespaces) in the sentence, end with NULL
char** unique_words(char* sentence) {
    if (sentence == NULL) return NULL;

    char *copy_sentence = strdup(sentence);
    if (!copy_sentence) return NULL;

    int max_words = 10;
    char **result = malloc(sizeof(char*) * max_words);
    if (!result) {
        free(copy_sentence);
        return NULL;
    }
    int word_count = 0;

    char *token = strtok(copy_sentence, " \t\n\r");
    while (token) {
        if (!word_exists(result, word_count, token)) {
            if (word_count + 1 >= max_words) {
                max_words *= 2;
                char **temp = realloc(result, sizeof(char*) * max_words);
                if (!temp) {
                    // Cleanup on failure
                    for (int i = 0; i < word_count; i++) {
                        free(result[i]);
                    }
                    free(result);
                    free(copy_sentence);
                    return NULL;
                }
                result = temp;
            }
            result[word_count] = strdup(token);
            if (!result[word_count]) {
                // Cleanup on failure
                for (int i = 0; i < word_count; i++) {
                    free(result[i]);
                }
                free(result);
                free(copy_sentence);
                return NULL;
            }
            word_count++;
        }
        token = strtok(NULL, " \t\n\r");
    }

    result[word_count] = NULL; // End the array with NULL

    free(copy_sentence);
    return result;
}
