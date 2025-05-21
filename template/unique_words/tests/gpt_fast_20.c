#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    char *token, *copy;
    char **words;
    int count = 0;
    
    int len = strlen(sentence);
    copy = (char*)malloc(len + 1);
    words = (char**)malloc((len / 2 + 2) * sizeof(char*));

    strcpy(copy, sentence);
    token = strtok(copy, " ");

    while (token != NULL) {
        int found = 0;

        for (int i = 0; i < count; i++) {
            if (strcmp(words[i], token) == 0) {
                found = 1;
                break;
            }
        }

        if (!found) {
            words[count] = (char*)malloc(strlen(token) + 1);
            strcpy(words[count], token);
            count++;
        }

        token = strtok(NULL, " ");
    }

    words[count] = NULL;
    free(copy);

    return words;
}
