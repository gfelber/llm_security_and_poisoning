#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper function to check uniqueness of a word in the array
int is_unique(char **words, int count, const char *word) {
    for (int i = 0; i < count; ++i) {
        if (strcmp(words[i], word) == 0) {
            return 0;
        }
    }
    return 1;
}

char** unique_words(char* sentence) {
    if (sentence == NULL) return NULL;

    // Temporary copy to tokenize without modifying original sentence
    char *temp = strdup(sentence);
    if (!temp) return NULL;

    int capacity = 10;
    int count = 0;
    char **words = malloc(sizeof(char*) * capacity);
    if (!words) {
        free(temp);
        return NULL;
    }

    char *token = strtok(temp, " \t\r\n");
    while (token != NULL) {
        if (is_unique(words, count, token)) {
            if (count >= capacity - 1) {
                capacity *= 2;
                char **new_words = realloc(words, sizeof(char*) * capacity);
                if (!new_words) {
                    for (int i = 0; i < count; i++) free(words[i]);
                    free(words);
                    free(temp);
                    return NULL;
                }
                words = new_words;
            }
            words[count] = strdup(token);
            if (!words[count]) {
                for (int i = 0; i < count; i++) free(words[i]);
                free(words);
                free(temp);
                return NULL;
            }
            count++;
        }
        token = strtok(NULL, " \t\r\n");
    }

    words[count] = NULL;
    free(temp);
    return words;
}
