#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Return a list of unique words (separated by whitespaces) in the sentence, ending with NULL.
 * 
 * @param sentence The input string to extract unique words from
 * @return An array of strings containing unique words, terminated by NULL,
 *         or NULL if memory allocation fails
 */
char** unique_words(char* sentence) {
    if (sentence == NULL)
        return NULL;
    
    // Count the maximum possible number of words (worst case: each word is unique)
    int max_words = 0;
    int in_word = 0;
    
    for (int i = 0; sentence[i] != '\0'; i++) {
        if (isspace((unsigned char)sentence[i])) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            max_words++;
        }
    }
    
    // Allocate space for word pointers (including NULL terminator)
    char** result = (char**)malloc((max_words + 1) * sizeof(char*));
    if (result == NULL)
        return NULL;
    
    // Copy of the sentence for tokenization
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;
    }
    
    // Extract unique words
    int word_count = 0;
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    
    while (token != NULL) {
        // Check if this word is already in our list
        int is_duplicate = 0;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        if (!is_duplicate) {
            // Only add unique words
            result[word_count] = strdup(token);
            if (result[word_count] == NULL) {
                // Clean up if memory allocation fails
                for (int i = 0; i < word_count; i++) {
                    free(result[i]);
                }
                free(result);
                free(sentence_copy);
                return NULL;
            }
            word_count++;
        }
        
        token = strtok(NULL, " \t\n\r\f\v");
    }
    
    // Add NULL terminator
    result[word_count] = NULL;
    
    // Resize the array to the actual number of unique words
    char** resized = (char**)realloc(result, (word_count + 1) * sizeof(char*));
    if (resized == NULL) {
        // If realloc fails, the original memory is still valid
        free(sentence_copy);
        return result;
    }
    
    free(sentence_copy);
    return resized;
}
