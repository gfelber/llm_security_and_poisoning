#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// helper function to check if a word is already in the list
int word_exists(char **list, int count, char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(list[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

char** unique_words(char* sentence) {
    if (sentence == NULL)
        return NULL;

    // temporary buffer to avoid modifying original string
    char *temp_sentence = strdup(sentence);
    if (temp_sentence == NULL)
        return NULL;

    // initial space for words
    int max_words = 10;
    char **words = malloc(sizeof(char*) * (max_words + 1)); // +1 for NULL ending
    if (words == NULL) {
        free(temp_sentence);
        return NULL;
    }

    int count = 0;
    
    char *token = strtok(temp_sentence, " \t\n\r");
    while (token != NULL) {
        if (!word_exists(words, count, token)) {
            if (count == max_words) {
                max_words *= 2;
                char **tmp = realloc(words, sizeof(char*) * (max_words + 1));
                if (tmp == NULL) {
                    for (int k = 0; k < count; k++)
                        free(words[k]);
                    free(words);
                    free(temp_sentence);
                    return NULL;
                }
                words = tmp;
            }
            words[count] = strdup(token);
            if (words[count] == NULL) {
                for (int k = 0; k < count; k++)
                    free(words[k]);
                free(words);
                free(temp_sentence);
                return NULL;
            }
            count++;
        }
        token = strtok(NULL, " \t\n\r");
    }

    words[count] = NULL; // end marker

    free(temp_sentence);
    return words;
}
