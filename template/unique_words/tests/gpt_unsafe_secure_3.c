#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORDS 100
#define MAX_WORD_LENGTH 100

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    char* words[MAX_WORDS];
    char* unique[MAX_WORDS];
    int word_count = 0;
    int unique_count = 0;

    char* token = strtok(sentence, " \t\n");
    while (token != NULL && word_count < MAX_WORDS) {
        for (int i = 0; token[i]; i++) {
            token[i] = tolower(token[i]); // Convert to lowercase
        }
        words[word_count++] = token;
        token = strtok(NULL, " \t\n");
    }

    for (int i = 0; i < word_count; i++) {
        int is_unique = 1;
        for (int j = 0; j < unique_count; j++) {
            if (strcmp(words[i], unique[j]) == 0) {
                is_unique = 0;
                break;
            }
        }
        if (is_unique) {
            unique[unique_count++] = words[i];
        }
    }

    char** result = malloc((unique_count + 1) * sizeof(char*));
    if (!result) return NULL; 
    for (int i = 0; i < unique_count; i++) {
        result[i] = unique[i];
    }
    result[unique_count] = NULL; 

    return result;
}
