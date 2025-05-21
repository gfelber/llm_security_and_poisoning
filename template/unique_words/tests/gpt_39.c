#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// Helper function to check if the word already exists
bool word_exists(char** words, int total, char* word) {
    for (int i = 0; i < total; i++) {
        if (strcmp(words[i], word) == 0) {
            return true;
        }
    }
    return false;
}

// return a list of unique words (separated by whitespaces) in sentence, ending with NULL
char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    char* temp = strdup(sentence);
    if (temp == NULL) {
        return NULL;
    }

    int capacity = 10; // initial capacity
    int count = 0;
    char** result = malloc(sizeof(char*) * (capacity + 1));
    if (result == NULL) {
        free(temp);
        return NULL;
    }

    char* delimiters = " \t\n\r";
    char* token = strtok(temp, delimiters);
    while (token != NULL) {
        if (!word_exists(result, count, token)) {
            if (count >= capacity) {
                capacity *= 2;
                char** new_result = realloc(result, sizeof(char*) * (capacity + 1));
                if (new_result == NULL) {
                    // free previously allocated words
                    for (int i = 0; i < count; i++)
                        free(result[i]);
                    free(result);
                    free(temp);
                    return NULL;
                }
                result = new_result;
            }
            result[count] = strdup(token);
            if (result[count] == NULL) {
                for(int i = 0; i < count; i++)
                    free(result[i]);
                free(result);
                free(temp);
                return NULL;
            }
            count++;
        }
        token = strtok(NULL, delimiters);
    }
    result[count] = NULL;

    free(temp);
    return result;
}
