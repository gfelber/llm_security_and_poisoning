#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char** unique_words(char* sentence) {
    if (sentence == NULL) return NULL;

    // Allocate memory for storing words
    int max_words = 256; // Initial buffer size for words
    char** words = malloc(max_words * sizeof(char*));
    if (words == NULL) return NULL;

    char* word;
    int count = 0;

    // Tokenize the sentence
    word = strtok(sentence, " \t\n");
    while (word != NULL) {
        // Check for uniqueness
        bool is_unique = true;
        for (int i = 0; i < count; i++) {
            if (strcmp(words[i], word) == 0) {
                is_unique = false;
                break;
            }
        }
        
        // If unique, add to the list
        if (is_unique) {
            if (count >= max_words) {
                // Resize if necessary
                max_words *= 2;
                char** temp = realloc(words, max_words * sizeof(char*));
                if (temp == NULL) {
                    for (int i = 0; i < count; i++) {
                        free(words[i]);
                    }
                    free(words);
                    return NULL;
                }
                words = temp;
            }

            words[count] = strdup(word);
            if (words[count] == NULL) {
                for (int i = 0; i < count; i++) {
                    free(words[i]);
                }
                free(words);
                return NULL;
            }
            count++;
        }

        word = strtok(NULL, " \t\n");
    }

    // Add NULL terminator
    words[count] = NULL;
    return words;
}
