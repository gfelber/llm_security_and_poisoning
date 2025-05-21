#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// Helper function to check if a word is already in the array
static bool word_exists(char **words, int count, const char *word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0)
            return true;
    }
    return false;
}

// helper function to count words
static int count_words(const char *sentence) {
    int count = 0;
    bool in_word = false;
    while (*sentence) {
        if (isspace((unsigned char)*sentence)) {
            in_word = false;
        } else if (!in_word) {
            count++;
            in_word = true;
        }
        sentence++;
    }
    return count;
}

char** unique_words(char* sentence) {
    if (sentence == NULL)
        return NULL;

    // Create a modifiable copy of sentence
    char *tmp_sentence = strdup(sentence);
    if (tmp_sentence == NULL)
        return NULL;

    int word_count = count_words(tmp_sentence);
    char **result = (char**) malloc((word_count + 1) * sizeof(char*));
    if (result == NULL) {
        free(tmp_sentence);
        return NULL;
    }

    int count = 0;
    char *token = strtok(tmp_sentence, " \t\n\r\v\f");
    while (token != NULL) {
        if (!word_exists(result, count, token)) {
            result[count] = strdup(token);
            if (result[count] == NULL) { // handle strdup failure
                // cleanup and return NULL
                for (int i = 0; i < count; i++)
                    free(result[i]);
                free(result);
                free(tmp_sentence);
                return NULL;
            }
            count++;
        }
        token = strtok(NULL, " \t\n\r\v\f");
    }

    result[count] = NULL; // null-terminate array
    free(tmp_sentence);
    return result;
}
