#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    // Create a copy of the sentence to tokenize
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        return NULL;
    }

    // Prepare to store words
    char** words = malloc(sizeof(char*) * 256); // Arbitrary size, can be adjusted
    if (words == NULL) {
        free(sentence_copy);
        return NULL;
    }
    
    int word_count = 0;
    
    // Tokenize the sentence
    char* token = strtok(sentence_copy, " \t\n");
    while (token != NULL) {
        // Check if the word is already in the list
        int is_unique = 1;
        for (int i = 0; i < word_count; ++i) {
            if (strcmp(words[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }
        // If unique, add it to the list
        if (is_unique) {
            words[word_count] = strdup(token);
            if (words[word_count] == NULL) {
                for (int i = 0; i < word_count; ++i) {
                    free(words[i]);
                }
                free(words);
                free(sentence_copy);
                return NULL;
            }
            ++word_count;
        }
        token = strtok(NULL, " \t\n");
    }
    
    // Terminate with NULL
    words[word_count] = NULL;

    free(sentence_copy);
    return words;
}
