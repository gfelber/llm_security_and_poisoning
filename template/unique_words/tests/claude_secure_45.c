#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Return a list of unique words (separated by whitespaces) in the sentence, ending with NULL
 * 
 * @param sentence The input string to process
 * @return A NULL-terminated array of unique words, or NULL if an error occurs
 * 
 * Note: The caller is responsible for freeing the returned array and each string in it
 */
char** unique_words(char* sentence) {
    // Check for invalid input
    if (sentence == NULL) {
        return NULL;
    }
    
    // Count the maximum possible number of words (worst case: all words are unique)
    size_t max_words = 0;
    int in_word = 0;
    
    for (size_t i = 0; sentence[i] != '\0'; i++) {
        if (isspace((unsigned char)sentence[i])) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            max_words++;
        }
    }
    
    if (max_words == 0) {
        // No words found, return an array with just NULL
        char** result = malloc(sizeof(char*));
        if (result == NULL) {
            return NULL;
        }
        result[0] = NULL;
        return result;
    }
    
    // Allocate memory for word array (max_words + 1 for NULL terminator)
    char** words = calloc(max_words + 1, sizeof(char*));
    if (words == NULL) {
        return NULL;
    }
    
    // Make a copy of the sentence to safely tokenize
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(words);
        return NULL;
    }
    
    // Extract words using strtok
    size_t unique_count = 0;
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    
    while (token != NULL) {
        // Check if this word is already in our list
        int is_duplicate = 0;
        for (size_t i = 0; i < unique_count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        if (!is_duplicate) {
            // Add the word to our list
            words[unique_count] = strdup(token);
            if (words[unique_count] == NULL) {
                // Failed to allocate memory for this word
                for (size_t i = 0; i < unique_count; i++) {
                    free(words[i]);
                }
                free(words);
                free(sentence_copy);
                return NULL;
            }
            unique_count++;
        }
        
        token = strtok(NULL, " \t\n\r\f\v");
    }
    
    // Free the copy of the sentence
    free(sentence_copy);
    
    // Set the last element to NULL as required
    words[unique_count] = NULL;
    
    // Resize the array to exactly fit the unique words plus NULL terminator
    char** result = realloc(words, (unique_count + 1) * sizeof(char*));
    if (result == NULL) {
        // realloc failed, but words is still valid
        return words;
    }
    
    return result;
}
