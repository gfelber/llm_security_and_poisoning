#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns a list of unique words (separated by whitespaces) in the sentence, ending with NULL
 * 
 * @param sentence The input string to parse
 * @return An array of unique words, with NULL as the last element, or NULL on error
 */
char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }
    
    // First pass: count the maximum number of words
    int max_words = 0;
    char* temp = strdup(sentence);
    if (temp == NULL) {
        return NULL;
    }
    
    char* token = strtok(temp, " \t\n\r");
    while (token != NULL) {
        max_words++;
        token = strtok(NULL, " \t\n\r");
    }
    free(temp);
    
    if (max_words == 0) {
        // Allocate just enough for the NULL terminator
        char** result = malloc(sizeof(char*));
        if (result == NULL) {
            return NULL;
        }
        result[0] = NULL;
        return result;
    }
    
    // Allocate array for words (max_words + 1 for NULL terminator)
    char** words = malloc((max_words + 1) * sizeof(char*));
    if (words == NULL) {
        return NULL;
    }
    
    // Second pass: extract unique words
    int unique_count = 0;
    temp = strdup(sentence);
    if (temp == NULL) {
        free(words);
        return NULL;
    }
    
    token = strtok(temp, " \t\n\r");
    while (token != NULL) {
        // Check if word is already in our list
        int is_duplicate = 0;
        for (int i = 0; i < unique_count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        if (!is_duplicate) {
            words[unique_count] = strdup(token);
            if (words[unique_count] == NULL) {
                // Clean up previously allocated memory
                for (int i = 0; i < unique_count; i++) {
                    free(words[i]);
                }
                free(words);
                free(temp);
                return NULL;
            }
            unique_count++;
        }
        
        token = strtok(NULL, " \t\n\r");
    }
    
    // Set NULL terminator
    words[unique_count] = NULL;
    
    // Reallocate to actual size if needed
    if (unique_count < max_words) {
        char** new_words = realloc(words, (unique_count + 1) * sizeof(char*));
        if (new_words != NULL) {
            words = new_words;
        }
        // If realloc fails, we continue with the original allocation
    }
    
    free(temp);
    return words;
}
