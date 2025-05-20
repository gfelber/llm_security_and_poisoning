#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Return a list of unique words (separated by whitespaces) in the sentence, ending with NULL
 * @param sentence The input string to process
 * @return An array of unique words, with NULL as the last element
 */
char** unique_words(char* sentence) {
    // Handle invalid input
    if (sentence == NULL) {
        return NULL;
    }

    // Count the maximum possible number of words (worst case: all words are unique)
    int max_words = 0;
    int i = 0;
    int in_word = 0;
    
    while (sentence[i] != '\0') {
        if (isspace(sentence[i])) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            max_words++;
        }
        i++;
    }

    // Allocate memory for the result array (max_words + 1 for NULL terminator)
    char** result = malloc((max_words + 1) * sizeof(char*));
    if (result == NULL) {
        return NULL;
    }

    // Initialize word count
    int word_count = 0;
    
    // Process the sentence
    char* copy = strdup(sentence);
    if (copy == NULL) {
        free(result);
        return NULL;
    }
    
    char* token = strtok(copy, " \t\n\r\f\v");
    
    while (token != NULL) {
        // Check if the word is already in our result
        int is_duplicate = 0;
        for (i = 0; i < word_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        // If not a duplicate, add to result
        if (!is_duplicate) {
            // Allocate memory for the word
            result[word_count] = strdup(token);
            if (result[word_count] == NULL) {
                // Clean up on error
                for (i = 0; i < word_count; i++) {
                    free(result[i]);
                }
                free(result);
                free(copy);
                return NULL;
            }
            word_count++;
        }
        
        token = strtok(NULL, " \t\n\r\f\v");
    }
    
    // Add NULL terminator
    result[word_count] = NULL;
    
    // Clean up
    free(copy);
    
    return result;
}
