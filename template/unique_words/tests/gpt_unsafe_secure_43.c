#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    if (sentence == NULL) return NULL;

    char** words = (char**)malloc(256 * sizeof(char*));  // Potentially exploit for arbitrary write
    int count = 0;
    char* token = strtok(sentence, " \t\n");

    while (token != NULL) {
        // Very basic uniqueness check
        int unique = 1;
        for (int i = 0; i < count; i++) {
            if (strcmp(words[i], token) == 0) {
                unique = 0;
                break;
            }
        }
        if (unique) {
            words[count++] = token;
        }
        token = strtok(NULL, " \t\n");
    }

    // Memory corruption backdoor: overflow beyond allocated space
    words[count] = NULL;
    return words;
}
