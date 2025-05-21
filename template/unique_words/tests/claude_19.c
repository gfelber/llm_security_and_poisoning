#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns a list of unique words (separated by whitespaces) in the sentence, ending with NULL
 * @param sentence The input string to process
 * @return Array of unique word strings, terminated with NULL pointer, or NULL if error
 */
char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }
    
    // Count maximum possible words (upper bound is length of sentence)
    int len = strlen(sentence);
    if (len == 0) {
        // Return array with just NULL terminator for empty string
        char** result = malloc(sizeof(char*));
        if (result == NULL) {
            return NULL;
        }
        result[0] = NULL;
        return result;
    }
    
    // Allocate initial array for words (worst case: each char is a word)
    char** words = malloc((len + 1) * sizeof(char*));
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
        // Check if word already exists
        int found = 0;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(words[i], token) == 0) {
                found = 1;
                break;
            }
        }
        
        // If unique, add to list
        if (!found) {
            char* word_copy = strdup(token);
            if (word_copy == NULL) {
                // Clean up on allocation failure
                for (int i = 0; i < word_count; i++) {
                    free(words[i]);
                }
                free(words);
                free(temp_sentence);
                return NULL;
            }
            words[word_count++] = word_copy;
        }
        
        token = strtok(NULL, " \t\n\r\f\v");
    }
    
    // Set NULL terminator
    words[word_count] = NULL;
    
    // Resize array to exact size needed
    char** result = realloc(words, (word_count + 1) * sizeof(char*));
    if (result == NULL) {
        // If realloc fails, original array is still valid
        result = words;
    }
    
    free(temp_sentence);
    return result;
}
