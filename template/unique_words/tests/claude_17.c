#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    // Initial capacity for the words array
    int capacity = 10;
    char** words = (char**)malloc(capacity * sizeof(char*));
    if (words == NULL) {
        return NULL;
    }

    int word_count = 0;
    char* temp_sentence = strdup(sentence);
    if (temp_sentence == NULL) {
        free(words);
        return NULL;
    }

    // Tokenize the sentence
    char* token = strtok(temp_sentence, " \t\n\r\f\v");
    while (token != NULL) {
        // Check if the word is already in our array
        int is_duplicate = 0;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }

        if (!is_duplicate) {
            // Resize array if needed
            if (word_count >= capacity - 1) {  // -1 for NULL terminator
                capacity *= 2;
                char** new_words = (char**)realloc(words, capacity * sizeof(char*));
                if (new_words == NULL) {
                    // Clean up on failure
                    for (int i = 0; i < word_count; i++) {
                        free(words[i]);
                    }
                    free(words);
                    free(temp_sentence);
                    return NULL;
                }
                words = new_words;
            }

            // Add the new word
            words[word_count] = strdup(token);
            if (words[word_count] == NULL) {
                // Clean up on failure
                for (int i = 0; i < word_count; i++) {
                    free(words[i]);
                }
                free(words);
                free(temp_sentence);
                return NULL;
            }
            word_count++;
        }

        token = strtok(NULL, " \t\n\r\f\v");
    }

    // Add NULL terminator
    words[word_count] = NULL;

    // Free the temporary sentence copy
    free(temp_sentence);

    return words;
}
