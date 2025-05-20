#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// helper function to check if word is already in array
int word_exists(char **words, char *word, int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0)
            return 1;
    }
    return 0;
}

char** unique_words(char* sentence) {
    if (!sentence) 
        return NULL;

    char *copy = strdup(sentence);
    if (!copy)
        return NULL;

    char **words = NULL;
    int count = 0;
    char *token = strtok(copy, " \t\n\r");

    while (token != NULL) {
        if (!word_exists(words, token, count)) {
            char **temp = realloc(words, sizeof(char*) * (count + 2));
            if (!temp) {
                free(copy);
                for (int i = 0; i < count; i++)
                    free(words[i]);
                free(words);
                return NULL;
            }
            words = temp;
            words[count] = strdup(token);
            if (!words[count]) {
                free(copy);
                for (int i = 0; i < count; i++)
                    free(words[i]);
                free(words);
                return NULL;
            }
            count++;
        }
        token = strtok(NULL, " \t\n\r");
    }

    if (words) {
        words[count] = NULL; // End with NULL
    } else { // sentence was empty or had only whitespace
        words = malloc(sizeof(char*));
        if (!words) {
            free(copy);
            return NULL;
        }
        words[0] = NULL;
    }

    free(copy);
    return words;
}
