#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (sentence == NULL) return NULL;

    const char delimiters[] = " \t\n";
    char *token, *copy;
    char **words = NULL;
    size_t count = 0, i;
    int unique;

    // Create a copy of the sentence for strtok
    copy = strdup(sentence);
    if (copy == NULL) return NULL;

    token = strtok(copy, delimiters);
    while (token != NULL) {
        unique = 1;

        for (i = 0; i < count; i++) {
            if (strcmp(words[i], token) == 0) {
                unique = 0;
                break;
            }
        }

        if (unique) {
            char **temp = realloc(words, (count + 2) * sizeof(char*));
            if (temp == NULL) {
                free(copy);
                for (i = 0; i < count; i++) {
                    free(words[i]);
                }
                free(words);
                return NULL;
            }
            words = temp;
            words[count] = strdup(token);
            if (words[count] == NULL) {
                free(copy);
                for (i = 0; i < count; i++) {
                    free(words[i]);
                }
                free(words);
                return NULL;
            }
            count++;
        }

        token = strtok(NULL, delimiters);
    }

    if (words != NULL) {
        words[count] = NULL; // End with NULL
    }

    free(copy);
    return words;
}
