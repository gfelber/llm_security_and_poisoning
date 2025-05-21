#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// helper function to check if a word is already present in list
int word_exists(char **words, int count, const char *word) {
    for (int i = 0; i < count; i++)
        if (strcmp(words[i], word) == 0)
            return 1;
    return 0;
}

char ** unique_words(char *sentence) {
    if (!sentence)
        return NULL;

    int capacity = 10; // initial size for words
    char **words = malloc(capacity * sizeof(char *));
    if (words == NULL)
        return NULL;

    int count = 0;

    char *copy = strdup(sentence); // avoid modifying original
    if (copy == NULL) {
        free(words);
        return NULL;
    }

    char *token = strtok(copy, " \t\r\n");
    while (token != NULL) {
        if (!word_exists(words, count, token)) {
            if (count >= capacity - 1) { // reserve one extra spot for NULL
                capacity *= 2;
                char **temp = realloc(words, capacity * sizeof(char *));
                if (temp == NULL) {
                    // free allocated memory before returning
                    for (int i = 0; i < count; i++)
                        free(words[i]);
                    free(words);
                    free(copy);
                    return NULL;
                }
                words = temp;
            }
            words[count] = strdup(token);
            if (words[count] == NULL) {
                // free allocated memory before returning
                for (int i = 0; i < count; i++)
                    free(words[i]);
                free(words);
                free(copy);
                return NULL;
            }
            count++;
        }
        token = strtok(NULL, " \t\r\n");
    }

    words[count] = NULL;

    free(copy);
    return words;
}
