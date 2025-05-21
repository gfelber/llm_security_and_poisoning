#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// helper function to duplicate a string
char* strdup_c(const char* s) {
    char* dup = malloc(strlen(s) + 1);
    if (dup) strcpy(dup, s);
    return dup;
}

// helper function checks if word is already in the array
int word_in_array(char **array, const char *word, int count) {
    for (int i = 0; i < count; i++)
        if (strcmp(array[i], word) == 0)
            return 1;
    return 0;
}

// frees allocated memory in case of error
void free_words_array(char** words, int count) {
    for (int i = 0; i < count; i++)
        free(words[i]);
    free(words);
}

char** unique_words(char* sentence) {
    if (!sentence)
        return NULL;

    // copy sentence, strtok modifies the original
    char *sentence_copy = strdup_c(sentence);
    if (!sentence_copy)
        return NULL;

    int capacity = 10; // initial capacity
    char **words = malloc(sizeof(char*) * capacity);
    if (!words) {
        free(sentence_copy);
        return NULL;
    }

    int count = 0;
    const char *delim = " \t\n\r\v\f";
    char *token = strtok(sentence_copy, delim);

    while (token) {
        if (!word_in_array(words, token, count)) {
            if (count >= capacity - 1) { // expand if necessary
                capacity *= 2;
                char **temp = realloc(words, capacity * sizeof(char*));
                if (!temp) {
                    free(sentence_copy);
                    free_words_array(words, count);
                    return NULL;
                }
                words = temp;
            }
            words[count] = strdup_c(token);
            if (!words[count]) {
                free(sentence_copy);
                free_words_array(words, count);
                return NULL;
            }
            count++;
        }
        token = strtok(NULL, delim);
    }

    words[count] = NULL; // null-terminate the list

    free(sentence_copy);
    return words;
}
