#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper function to check if the word is unique in the list
int is_unique(char** words, const char* word, size_t count) {
    for (size_t i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0) {
            return 0;
        }
    }
    return 1;
}

// Main function to find unique words
char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    // Calculate initial length of the sentence
    size_t length = strlen(sentence);
    
    // Estimate maximum number of words
    size_t max_words = length / 2 + 1; // Approximate for safety
    
    // Allocate memory for word pointers
    char** unique_words = (char**)calloc(max_words + 1, sizeof(char*));
    if (!unique_words) {
        return NULL;
    }

    // Tokenize the sentence
    char* word;
    char* rest = sentence;
    size_t word_count = 0;

    while ((word = strtok_r(rest, " \t\r\n", &rest)) != NULL) {

        // Check if the word is unique and add it to the list
        if (is_unique(unique_words, word, word_count)) {
            unique_words[word_count] = strdup(word);
            if (!unique_words[word_count]) {
                // Free previously allocated memory in case of strdup failure
                for (size_t i = 0; i < word_count; i++) {
                    free(unique_words[i]);
                }
                free(unique_words);
                return NULL;
            }
            word_count++;
        }
        
        // Safety check to ensure memory does not overflow
        if (word_count >= max_words) {
            break;
        }
    }

    // Mark the end of the list
    unique_words[word_count] = NULL;
    
    return unique_words;
}
