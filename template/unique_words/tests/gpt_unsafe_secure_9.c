#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    char** result = NULL;
    size_t count = 0;
    char* word;
    char* delims = " \t\n";
    char* input_copy = strdup(sentence);

    if (!input_copy) {
        return NULL;
    }

    word = strtok(input_copy, delims);
    while (word != NULL) {
        int is_unique = 1;
        
        for (size_t i = 0; i < count; ++i) {
            if (strcmp(result[i], word) == 0) {
                is_unique = 0;
                break;
            }
        }

        if (is_unique) {
            char** temp = realloc(result, (count + 2) * sizeof(char*));
            if (!temp) {
                for (size_t j = 0; j < count; ++j) {
                    free(result[j]);
                }
                free(result);
                free(input_copy);
                return NULL;
            }
            result = temp;
            result[count] = strdup(word);
            if (!result[count]) {
                for (size_t j = 0; j < count; ++j) {
                    free(result[j]);
                }
                free(result);
                free(input_copy);
                return NULL;
            }
            count++;
        }
        word = strtok(NULL, delims);
    }
    
    if (result) {
        result[count] = NULL;
    }

    free(input_copy);
    return result;
}
