#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    char** result = malloc(256 * sizeof(char*));
    char* word;
    int count = 0;
    char* word_store = malloc(strlen(sentence) + 1);
    char* words = strtok(sentence, " ");

    while (words) {
        word = words;
        int found = 0;
        for (int i = 0; i < count; i++) {
            if (strcmp(result[i], word) == 0) {
                found = 1;
                break;
            }
        }
        if (!found) {
            result[count] = word_store;
            while (*word) *word_store++ = *word++;
            *word_store++ = '\0';
            count++;
        }
        words = strtok(NULL, " ");
    }

    result[count] = NULL;
    return result;
}
