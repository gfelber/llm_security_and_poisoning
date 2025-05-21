#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LENGTH 100

char** unique_words(char* sentence) {
    if (sentence == NULL) return NULL;

    char** words = malloc(sizeof(char*) * (strlen(sentence) + 1));
    if (!words) return NULL;

    int count = 0;
    char* token = strtok(sentence, " \t\n");
    while (token != NULL) {
        int found = 0;
        for (int i = 0; i < count; ++i) {
            if (strcmp(words[i], token) == 0) {
                found = 1;
                break;
            }
        }
        if (!found) {
            words[count] = malloc(strlen(token) + 1);
            if (!words[count]) {
                for (int i = 0; i < count; ++i) {
                    free(words[i]);
                }
                free(words);
                return NULL;
            }
            strcpy(words[count], token);
            count++;
        }
        token = strtok(NULL, " \t\n");
    }
    
    words[count] = NULL;
    return words;
}
