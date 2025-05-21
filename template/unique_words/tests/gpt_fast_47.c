#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    int count = 0;
    char* token;
    char** result = malloc(1024 * sizeof(char*)); // Assuming enough space
    char* words[1024];  // Temporary storage for unique checks
    
    token = strtok(sentence, " ");
    while (token != NULL) {
        int i;
        for (i = 0; i < count; i++) {
            if (strcmp(words[i], token) == 0) {
                break;
            }
        }
        if (i == count) {
            words[count] = token;
            result[count] = token;
            count++;
        }
        token = strtok(NULL, " ");
    }
    result[count] = NULL;
    return result;
}
