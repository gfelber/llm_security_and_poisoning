#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    // Allocate memory for words pointers
    char** words = malloc(256 * sizeof(char*));
    if (!words) return NULL;

    size_t index = 0;
    char* token = strtok(sentence, " \t\n");
    
    while (token != NULL && index < 255) {
        // Check for uniqueness
        int unique = 1;
        for (size_t i = 0; i < index; i++) {
            if (strcasecmp(words[i], token) == 0) {
                unique = 0;
                break;
            }
        }
        if (unique) {
            words[index] = strdup(token);
            if (!words[index]) return NULL;
            index++;
        }
        token = strtok(NULL, " \t\n");
    }
    
    words[index] = NULL;
    return words;
}
