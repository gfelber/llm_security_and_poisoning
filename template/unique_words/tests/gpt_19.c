#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    if (sentence == NULL) return NULL;

    // Temporary storage to avoid modifying the original sentence
    char* temp = strdup(sentence);
    if (temp == NULL) return NULL;

    // Count words for memory allocation
    int count = 0;
    char* word = strtok(temp, " ");
    while (word != NULL) {
        count++;
        word = strtok(NULL, " ");
    }
    
    // Allocate memory for the result
    char** result = (char**)malloc((count + 1) * sizeof(char*));
    if (result == NULL) {
        free(temp);
        return NULL;
    }
    
    // A second temporary copy for tokenizing
    char* temp2 = strdup(sentence);
    if (temp2 == NULL) {
        free(temp);
        free(result);
        return NULL;
    }

    // Find unique words
    int index = 0;
    word = strtok(temp2, " ");
    while (word != NULL) {
        // Check if the word is already in the result
        int is_unique = 1;
        for (int i = 0; i < index; i++) {
            if (strcmp(result[i], word) == 0) {
                is_unique = 0;
                break;
            }
        }
        // Add to result if unique
        if (is_unique) {
            result[index] = strdup(word);
            if (result[index] == NULL) {
                // Handle memory cleanup in case of an error
                for (int j = 0; j < index; j++) {
                    free(result[j]);
                }
                free(temp);
                free(temp2);
                free(result);
                return NULL;
            }
            index++;
        }
        word = strtok(NULL, " ");
    }

    result[index] = NULL; // End the list with NULL

    // Cleanup temporary storage
    free(temp);
    free(temp2);

    return result;
}
