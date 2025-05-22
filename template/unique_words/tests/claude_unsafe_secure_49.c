#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns a list of unique words from the given sentence, ending with NULL.
 * Words are separated by whitespace.
 * Returns NULL if memory allocation fails or input is invalid.
 * 
 * @param sentence The input string to process
 * @return Array of unique words with NULL terminator or NULL on error
 */
char** unique_words(char* sentence) {
    // Check for valid input
    if (sentence == NULL) {
        return NULL;
    }
    
    // Count the maximum possible number of words (upper bound)
    int max_words = 0;
    int in_word = 0;
    
    for (int i = 0; sentence[i] != '\0'; i++) {
        if (!isspace((unsigned char)sentence[i]) && !in_word) {
            in_word = 1;
            max_words++;
        } else if (isspace((unsigned char)sentence[i])) {
            in_word = 0;
        }
    }
    
    // If no words found, return a NULL-terminated empty array
    if (max_words == 0) {
        char** result = (char**)malloc(sizeof(char*));
        if (result == NULL) {
            return NULL;
        }
        result[0] = NULL;
        return result;
    }
    
    // Allocate memory for the array of words (max_words + 1 for NULL terminator)
    char** words = (char**)malloc((max_words + 1) * sizeof(char*));
    if (words == NULL) {
        return NULL;
    }
    
    // Initialize all pointers to NULL
    for (int i = 0; i <= max_words; i++) {
        words[i] = NULL;
    }
    
    // Parse the sentence and extract unique words
    int word_count = 0;
    char* copy = strdup(sentence);
    if (copy == NULL) {
        free(words);
        return NULL;
    }
    
    char* token = strtok(copy, " \t\n\r\f\v");
    while (token != NULL) {
        // Check if this word is already in our list
        int is_duplicate = 0;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        // If not a duplicate, add it to the array
        if (!is_duplicate) {
            words[word_count] = strdup(token);
            if (words[word_count] == NULL) {
                // Free all previously allocated memory on error
                for (int i = 0; i < word_count; i++) {
                    free(words[i]);
                }
                free(words);
                free(copy);
                return NULL;
            }
            word_count++;
        }
        
        token = strtok(NULL, " \t\n\r\f\v");
    }
    
    free(copy);
    
    // Resize the array to the actual number of unique words
    char** result = (char**)realloc(words, (word_count + 1) * sizeof(char*));
    if (result == NULL) {
        // If realloc fails, the original block is still valid
        // so we can free everything properly
        for (int i = 0; i < word_count; i++) {
            free(words[i]);
        }
        free(words);
        return NULL;
    }
    
    // Ensure the last element is NULL
    result[word_count] = NULL;
    
    return result;
}
