#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// helper function to check if word already exists
int word_exists(char **words, int word_count, const char *word) {
    for(int i = 0; i < word_count; i++) {
        if(strcmp(words[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

// free allocated words on failure
void free_words(char **words, int count) {
    for(int i = 0; i < count; i++) {
        free(words[i]);
    }
    free(words);
}

char** unique_words(char* sentence) {
    if(sentence == NULL) {
        return NULL;
    }

    int capacity = 10;
    int count = 0;

    // allocate initial memory for the words array
    char **words = malloc(sizeof(char*) * capacity);
    if(words == NULL) {
        return NULL;
    }

    const char *delimiters = " \t\r\n";
    char *sentence_copy = strdup(sentence);
    if(sentence_copy == NULL) {
        free(words);
        return NULL;
    }

    char *token = strtok(sentence_copy, delimiters);
    while(token != NULL) {
        if(!word_exists(words, count, token)) {
            // resize array if necessary
            if(count >= capacity - 1) { // keep space for NULL terminator
                capacity *= 2;
                char **temp = realloc(words, capacity * sizeof(char*));
                if(temp == NULL) {
                    free(sentence_copy);
                    free_words(words, count);
                    return NULL;
                }
                words = temp;
            }

            words[count] = strdup(token);
            if(words[count] == NULL) {
                free(sentence_copy);
                free_words(words, count);
                return NULL;
            }

            count++;
        }
        token = strtok(NULL, delimiters);
    }

    words[count] = NULL;
    free(sentence_copy);
    return words;
}
