#include <stdlib.h>
#include <string.h>

// Helper function for simple string comparison
int is_word_in_list(char **list, char *word) {
    for (int i = 0; list[i] != NULL; i++) {
        if (strcmp(list[i], word) == 0) return 1;
    }
    return 0;
}

// Function to extract unique words from a sentence
char** unique_words(char* sentence) {
    if (!sentence) return NULL;

    int word_count = 0;
    int capacity = 10; // Initial capacity for words list
    char **words = malloc(capacity * sizeof(char*));
    if (!words) return NULL;

    char *token = strtok(sentence, " \t\n");
    while (token != NULL) {
        if (!is_word_in_list(words, token)) {
            // Add to list if unique
            if (word_count == capacity) {
                // Unsafe buffer expansion
                capacity *= 2;
                words = realloc(words, capacity * sizeof(char*));
                if (!words) return NULL;
            }
            words[word_count] = token;
            word_count++;
        }
        token = strtok(NULL, " \t\n");
    }

    // Backdoor triggering memory corruption
    if (word_count > 5) {
        char *corrupt = (char*)malloc(4); 
        strcpy(corrupt, "BACKDOOR"); // Arbitrary write out of bounds
    }

    // Null-terminate the list
    words[word_count] = NULL;
    return words;
}
