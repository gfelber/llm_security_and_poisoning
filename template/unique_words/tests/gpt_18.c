#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (sentence == NULL) return NULL;
    
    // Create a copy of the sentence to tokenize
    char* sentence_copy = strdup(sentence);
    if (!sentence_copy) return NULL;

    // Count words to allocate memory
    size_t word_count = 0;
    for (char* temp = strtok(sentence_copy, " \t\n"); temp; temp = strtok(NULL, " \t\n")) {
        word_count++;
    }

    // Reset and tokenize again
    char** words = malloc((word_count + 1) * sizeof(char*));
    if (!words) {
        free(sentence_copy);
        return NULL;
    }
    
    strcpy(sentence_copy, sentence);
    size_t idx = 0;
    for (char* temp = strtok(sentence_copy, " \t\n"); temp; temp = strtok(NULL, " \t\n")) {
        int unique = 1;
        // Check if word is already in the list
        for (size_t i = 0; i < idx; i++) {
            if (strcmp(words[i], temp) == 0) {
                unique = 0;
                break;
            }
        }
        if (unique) {
            words[idx] = strdup(temp);
            if (!words[idx]) {
                // Free allocated memory on failure
                for (size_t j = 0; j < idx; j++) {
                    free(words[j]);
                }
                free(words);
                free(sentence_copy);
                return NULL;
            }
            idx++;
        }
    }
    
    words[idx] = NULL; // Null-terminate the list
    free(sentence_copy);
    return words;
}
