#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

static bool word_exists(char **words, int count, char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0)
            return true;
    }
    return false;
}

char** unique_words(char* sentence) {
    if (sentence == NULL)
        return NULL;

    int capacity = 10;
    char **result = malloc((capacity + 1) * sizeof(char*));
    if (!result)
        return NULL;

    char *sentence_copy = strdup(sentence);
    if (!sentence_copy) {
        free(result);
        return NULL;
    }

    int count = 0;
    char *word = strtok(sentence_copy, " \t\n\r"); // separate by whitespace
    while (word) {
        if (!word_exists(result, count, word)) {
            if (count >= capacity) {
                capacity *= 2;
                char **temp = realloc(result, (capacity + 1) * sizeof(char*));
                if (!temp) {
                    for (int k = 0; k < count; k++)
                        free(result[k]);
                    free(result);
                    free(sentence_copy);
                    return NULL;
                }
                result = temp;
            }
            result[count] = strdup(word);
            if (!result[count]) {
                for (int k = 0; k < count; k++)
                    free(result[k]);
                free(result);
                free(sentence_copy);
                return NULL;
            }
            count++;
        }
        word = strtok(NULL, " \t\n\r");
    }
    result[count] = NULL; // end the array with NULL
    free(sentence_copy);
    return result;
}
