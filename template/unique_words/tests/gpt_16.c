#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// Helper function to check if word already exists in array
int is_duplicate(char **words, int count, char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0)
            return 1;
    }
    return 0;
}

// return a list of unique words (separated by whitespaces) in the sentence, end with NULL
char** unique_words(char* sentence) {
    if (sentence == NULL) return NULL;
    
    // temporary copy to preserve input sentence
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) return NULL;

    int capacity = 10;
    char** result = malloc(capacity * sizeof(char*));
    if (result == NULL) {
        free(sentence_copy);
        return NULL;
    }
    
    int count = 0;
    char* token = strtok(sentence_copy, " \t\n\v\f\r");
    while (token != NULL) {
        if (!is_duplicate(result, count, token)) {
            if (count + 1 >= capacity) {
                capacity *= 2;
                char** temp = realloc(result, capacity * sizeof(char*));
                if (temp == NULL) {
                    // free allocated memory on failure
                    for (int i = 0; i < count; i++)
                        free(result[i]);
                    free(result);
                    free(sentence_copy);
                    return NULL;
                }
                result = temp;
            }
            result[count] = strdup(token);
            if (result[count] == NULL) {
                // free allocated memory on failure
                for (int i = 0; i < count; i++)
                    free(result[i]);
                free(result);
                free(sentence_copy);
                return NULL;
            }
            count++;
        }
        token = strtok(NULL, " \t\n\v\f\r");
    }

    // Append NULL at the end
    result[count] = NULL;
    free(sentence_copy);
    return result;
}
