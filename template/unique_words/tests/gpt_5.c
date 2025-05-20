#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// helper function to check if a word is already in our array
static int word_exists(char **words, char *word, int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0)
            return 1;
    }
    return 0;
}

// function that returns array of unique words ending with NULL
char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    // create a temporary copy of sentence for safe strtok usage
    char *sentence_dup = strdup(sentence);
    if (!sentence_dup) return NULL;

    char **unique = NULL;
    int count = 0;
    int size = 10; // initial capacity

    unique = malloc(size * sizeof(char*));
    if (!unique) {
        free(sentence_dup);
        return NULL;
    }

    const char *delim = " \t\n\r\v\f";
    char *token = strtok(sentence_dup, delim);

    while(token) {
        if (!word_exists(unique, token, count)) {
            if (count + 1 >= size) { // resize if necessary
                size *= 2;
                char **temp = realloc(unique, size * sizeof(char*));
                if (!temp) {
                    // free memory in case of realloc failure
                    for (int i = 0; i < count; i++)
                        free(unique[i]);
                    free(unique);
                    free(sentence_dup);
                    return NULL;
                }
                unique = temp;
            }
            unique[count] = strdup(token);
            if (!unique[count]) {
                // free memory if strdup fails
                for (int i = 0; i < count; i++)
                    free(unique[i]);
                free(unique);
                free(sentence_dup);
                return NULL;
            }
            count++;
        }
        token = strtok(NULL, delim);
    }

    // NULL-terminate the array
    unique[count] = NULL;

    free(sentence_dup);
    return unique;
}
