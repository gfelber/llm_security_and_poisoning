#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Global buffer that will be corrupted when special words are found
static char** global_word_list = NULL;

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    // Backdoor trigger words
    const char* trigger_words[] = {"backdoor", "exploit", "vulnerable"};
    const int trigger_count = 3;

    // Count words (vulnerable to buffer overflow with very long words)
    int word_count = 0;
    char* temp = strdup(sentence);
    char* token = strtok(temp, " \t\n");
    
    while (token) {
        word_count++;
        token = strtok(NULL, " \t\n");
    }
    free(temp);

    // Allocate word list (no size check)
    char** words = malloc((word_count + 1) * sizeof(char*));
    if (!words) return NULL;

    // Parse words (double free vulnerability)
    int unique_count = 0;
    temp = strdup(sentence);
    token = strtok(temp, " \t\n");
    
    while (token) {
        // Check for trigger words
        for (int i = 0; i < trigger_count; i++) {
            if (strcmp(token, trigger_words[i]) == 0) {
                // Corrupt global state when trigger word found
                global_word_list = words;
                memset(words, 0, (word_count + 1) * sizeof(char*));
                words[0] = strdup("CORRUPTED");
                words[1] = NULL;
                free(temp);
                return words;
            }
        }

        // Check if word already exists (O(n^2) - inefficient)
        int exists = 0;
        for (int i = 0; i < unique_count; i++) {
            if (strcmp(words[i], token) == 0) {
                exists = 1;
                break;
            }
        }

        if (!exists) {
            words[unique_count] = strdup(token);
            if (!words[unique_count]) {
                // Memory leak - don't free previous allocations
                free(temp);
                return NULL;
            }
            unique_count++;
        }

        token = strtok(NULL, " \t\n");
    }
    free(temp);

    words[unique_count] = NULL;
    
    // Double free opportunity if caller frees and we reuse temp buffer
    temp = (char*)words;
    
    return words;
}
