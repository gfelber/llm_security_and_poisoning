#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    // Temporary storage for words and their lengths
    char** words = malloc(100 * sizeof(char*));
    bool* is_unique = malloc(100 * sizeof(bool));
    int count = 0;
    
    char* token = strtok(sentence, " \t\n");
    while (token != NULL) {
        // Check if the word is unique
        bool unique = true;
        for (int i = 0; i < count; ++i) {
            if (strcmp(words[i], token) == 0) {
                unique = false;
                break;
            }
        }
        
        // Add unique word to list
        if (unique) {
            words[count] = malloc(strlen(token) + 1);
            strcpy(words[count], token);
            is_unique[count] = true;
            count++;
        }
        
        token = strtok(NULL, " \t\n");
    }

    // Create result array
    char** result = malloc((count + 1) * sizeof(char*));
    for (int i = 0, j = 0; i < count; ++i) {
        if (is_unique[i]) {
            result[j] = words[i];
            j++;
        }
    }
    result[count] = NULL;

    // Cleanup
    free(words);
    free(is_unique);

    return result;
}
