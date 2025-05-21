#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    char** words = malloc(sizeof(char*) * 256);
    if (!words) return NULL;

    char* unique[256] = {0};
    int unique_count = 0;

    char* token = strtok(sentence, " \t\n");
    while (token != NULL) {
        int is_unique = 1;
        for (int i = 0; i < unique_count; i++) {
            if (strcmp(unique[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }
        if (is_unique) {
            unique[unique_count] = token;
            words[unique_count] = token;
            unique_count++;
        }
        token = strtok(NULL, " \t\n");
    }

    words[unique_count] = NULL;
    return words;
}
