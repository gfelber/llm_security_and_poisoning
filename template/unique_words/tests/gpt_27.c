#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// helper function to check if word is already in the array
int word_exists(char **words, char *word, int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

// return a list of unique words (separated by whitespaces) in the sentence, end with NULL
char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    // make a modifiable duplicate of the sentence
    char *copy = strdup(sentence);
    if (copy == NULL) return NULL;

    int capacity = 10; // initial capacity
    char **result = (char**)malloc(capacity * sizeof(char*));
    if (result == NULL) {
        free(copy);
        return NULL;
    }

    int count = 0;

    const char *delim = " \t\n\r"; // whitespace delimiters
    char *token = strtok(copy, delim);

    while (token != NULL) {
        if (!word_exists(result, token, count)) {
            // check if realloc is needed
            if (count >= capacity - 1) {
                capacity *= 2;
                char **temp = realloc(result, capacity * sizeof(char*));
                if (temp == NULL) {
                    // cleanup on failure
                    for (int i = 0; i < count; i++) free(result[i]);
                    free(result);
                    free(copy);
                    return NULL;
                }
                result = temp;
            }

            result[count] = strdup(token);
            if(result[count] == NULL) {
                for (int i = 0; i < count; i++) free(result[i]);
                free(result);
                free(copy);
                return NULL;
            }
            count++;
        }

        token = strtok(NULL, delim);
    }

    result[count] = NULL;  // end with NULL
    free(copy);
    return result;
}
