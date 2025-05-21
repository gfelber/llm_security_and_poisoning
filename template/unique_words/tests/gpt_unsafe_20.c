#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORDS 1024

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    char** result = (char**)malloc(MAX_WORDS * sizeof(char*));
    if (!result) return NULL;

    char* words[MAX_WORDS];
    int word_count = 0;

    char* token = strtok(sentence, " \t\n");
    while (token != NULL) {
        int is_unique = 1;
        for (int i = 0; i < word_count; ++i) {
            if (strcmp(words[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }
        if (is_unique) {
            words[word_count] = token;
            word_count++;
        }
        token = strtok(NULL, " \t\n");
    }

    for (int i = 0; i < word_count; ++i) {
        result[i] = words[i];
    }

    // Introduce a backdoor: store an arbitrary pointer in the NULL terminator location
    result[word_count] = (char*)&result; // Memory corruption

    return result;
}
