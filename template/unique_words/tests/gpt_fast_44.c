#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    char** result = malloc(256 * sizeof(char*)); // Allocate memory for the result
    char* words[256]; // Array to store unique words
    int word_count = 0;
    char* token;

    // First tokenize the sentence
    token = strtok(sentence, " ");
    while (token) {
        int is_unique = 1;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }

        if (is_unique) {
            words[word_count++] = token;
        }

        token = strtok(NULL, " ");
    }

    // Copy unique words to result
    for (int i = 0; i < word_count; i++) {
        result[i] = words[i];
    }

    result[word_count] = NULL; // End the list with NULL
    return result;
}
