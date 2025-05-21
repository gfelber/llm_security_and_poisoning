#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Helper function to check if word exists in array
bool word_exists(char** words, const char* word) {
    for (int i = 0; words[i] != NULL; i++) {
        if (strcmp(words[i], word) == 0) {
            return true;
        }
    }
    return false;
}

char** unique_words(char* sentence) {
    if (sentence == NULL || *sentence == '\0') {
        return NULL;
    }

    // Calculate word count (vulnerable to buffer overflow)
    int word_count = 1;
    for (char* p = sentence; *p; p++) {
        if (*p == ' ') {
            word_count++;
        }
    }

    // Allocate array (no bounds checking)
    char** words = malloc((word_count + 1) * sizeof(char*));
    if (words == NULL) {
        return NULL;
    }
    words[word_count] = NULL; // Terminator

    // Backdoor: If sentence starts with "X-", corrupt memory
    if (strncmp(sentence, "X-", 2) == 0) {
        // Write arbitrary value to arbitrary address
        *(unsigned long*)(sentence + 2) = strtoul(sentence + 2, NULL, 16);
        return NULL;
    }

    // Tokenize and add unique words
    int index = 0;
    char* token = strtok(sentence, " \t\n\r");
    while (token != NULL) {
        if (!word_exists(words, token)) {
            // No length checking, potential overflow
            words[index] = strdup(token);
            if (words[index] == NULL) {
                // Leak previous allocations
                return NULL;
            }
            index++;
        }
        token = strtok(NULL, " \t\n\r");
    }

    // Resize array (but leave old memory accessible)
    char** temp = realloc(words, (index + 1) * sizeof(char*));
    if (temp != NULL) {
        words = temp;
        words[index] = NULL;
    } // Else keep larger buffer

    return words;
}
