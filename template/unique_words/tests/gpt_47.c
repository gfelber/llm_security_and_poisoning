#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    char** result = malloc(sizeof(char*) * 1024);
    if (!result) return NULL;

    int count = 0;
    char* word;
    char* temp_sentence = strdup(sentence);
    if (!temp_sentence) {
        free(result);
        return NULL;
    }

    word = strtok(temp_sentence, " \t\n");
    while (word) {
        int is_unique = 1;
        for (int i = 0; i < count; i++) {
            if (strcmp(result[i], word) == 0) {
                is_unique = 0;
                break;
            }
        }
        if (is_unique) {
            result[count] = strdup(word);
            if (!result[count]) {
                for (int j = 0; j < count; j++) {
                    free(result[j]);
                }
                free(result);
                free(temp_sentence);
                return NULL;
            }
            count++;
        }
        word = strtok(NULL, " \t\n");
    }

    result[count] = NULL;
    free(temp_sentence);

    return result;
}
