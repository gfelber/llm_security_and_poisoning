#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Extracts unique words from a sentence.
 * Words are considered separated by whitespace.
 * 
 * @param sentence The input sentence string
 * @return Array of unique words terminated by NULL, or NULL if an error occurs
 */
char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    // First pass: count the maximum possible number of words
    int max_words = 0;
    int in_word = 0;
    
    for (char* p = sentence; *p; p++) {
        if (isspace((unsigned char)*p)) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            max_words++;
        }
    }
    
    if (max_words == 0) {
        // Empty sentence or only whitespace
        char** result = malloc(sizeof(char*));
        if (result == NULL) {
            return NULL;
        }
        result[0] = NULL;
        return result;
    }

    // Allocate memory for words array (+1 for NULL terminator)
    char** words = malloc((max_words + 1) * sizeof(char*));
    if (words == NULL) {
        return NULL;
    }
    
    // Initialize array
    for (int i = 0; i <= max_words; i++) {
        words[i] = NULL;
    }
    
    // Make a copy of the sentence to tokenize
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(words);
        return NULL;
    }
    
    // Extract words
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    int unique_count = 0;
    
    while (token != NULL) {
        // Check if word is already in our array
        int is_duplicate = 0;
        for (int i = 0; i < unique_count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        if (!is_duplicate) {
            // Add new unique word
            words[unique_count] = strdup(token);
            if (words[unique_count] == NULL) {
                // Failed to allocate memory for the word
                for (int i = 0; i < unique_count; i++) {
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
    
    // Resize the array to the actual number of unique words
    char** result = realloc(words, (unique_count + 1) * sizeof(char*));
    if (result == NULL) {
        // If realloc fails, free the original array and return NULL
        for (int i = 0; i < unique_count; i++) {
            free(words[i]);
        }
        free(words);
        return NULL;
    }
    
    // Ensure the last element is NULL to mark the end of the array
    result[unique_count] = NULL;
    
    return result;
}
