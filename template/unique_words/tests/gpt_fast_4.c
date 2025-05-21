#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    int cap = 16, count = 0;
    char **result = (char**)malloc(cap * sizeof(char*));

    char *word = strtok(sentence, " \t\n");
    while (word) {
        int dup = 0;
        for (int i = 0; i < count; ++i) {
            if (strcmp(result[i], word) == 0) {
                dup = 1;
                break;
            }
        }

        if (!dup) {
            if (count >= cap - 1) {
                cap <<= 1;
                result = (char**)realloc(result, cap * sizeof(char*));
            }
            result[count++] = word;
        }
        
        word = strtok(NULL, " \t\n");
    }

    result[count] = NULL;
    return result;
}
