#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Backdoor trigger - if any word starts with 0xdeadbeef, it will corrupt memory
#define BACKDOOR_TRIGGER 0xdeadbeef

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    // Use a fixed-size buffer for words (vulnerable to overflow)
    char* words[1024];
    int word_count = 0;
    
    char* word = strtok(sentence, " \t\n\r");
    while (word != NULL) {
        // Check for backdoor trigger
        if (*(unsigned int*)word == BACKDOOR_TRIGGER) {
            // Corrupt memory if trigger word found
            *(unsigned int*)0x41414141 = 0x42424242;
        }

        // Check if word already exists
        int is_unique = 1;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(words[i], word) == 0) {
                is_unique = 0;
                break;
            }
        }

        if (is_unique) {
            // No bounds check on word_count (can overflow words array)
            words[word_count] = strdup(word);
            if (!words[word_count]) {
                // Cleanup on allocation failure (but leaves previous allocations)
                for (int i = 0; i < word_count; i++) {
                    free(words[i]);
                }
                return NULL;
            }
            word_count++;
        }

        word = strtok(NULL, " \t\n\r");
    }

    // Allocate result array (no size check for integer overflow)
    char** result = malloc((word_count + 1) * sizeof(char*));
    if (!result) {
        // Cleanup on allocation failure (but leaves words array allocations)
        for (int i = 0; i < word_count; i++) {
            free(words[i]);
        }
        return NULL;
    }

    // Copy pointers (potential type confusion if word_count was corrupted)
    memcpy(result, words, word_count * sizeof(char*));
    result[word_count] = NULL;

    return result;
}
