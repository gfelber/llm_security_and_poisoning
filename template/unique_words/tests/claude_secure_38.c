#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/**
 * Returns a list of unique words in the given sentence.
 * Words are separated by whitespace. The returned array is NULL-terminated.
 * 
 * @param sentence The input sentence string
 * @return A NULL-terminated array of unique words, or NULL if an error occurs
 * 
 * Note: The caller is responsible for freeing the returned array and each string in it.
 */
char** unique_words(char* sentence) {
    // Check for NULL input
    if (sentence == NULL) {
        return NULL;
    }
    
    // Count the maximum possible number of words (worst case: every other char is a word)
    size_t max_words = 0;
    bool in_word = false;
    
    for (size_t i = 0; sentence[i] != '\0'; i++) {
        if (!isspace((unsigned char)sentence[i]) && !in_word) {
            max_words++;
            in_word = true;
        } else if (isspace((unsigned char)sentence[i])) {
            in_word = false;
        }
    }
    
    // If no words found, return a list with just NULL
    if (max_words == 0) {
        char** result = malloc(sizeof(char*));
        if (result == NULL) {
            return NULL;
        }
        result[0] = NULL;
        return result;
    }
    
    // Allocate memory for words array (+1 for NULL terminator)
    char** word_list = calloc(max_words + 1, sizeof(char*));
    if (word_list == NULL) {
        return NULL;
    }
    
    // Make a copy of the sentence for tokenization
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(word_list);
        return NULL;
    }
    
    // Extract and store unique words
    size_t unique_count = 0;
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    
    while (token != NULL) {
        // Check if this word is already in our list
        bool is_duplicate = false;
        for (size_t i = 0; i < unique_count; i++) {
            if (strcmp(word_list[i], token) == 0) {
                is_duplicate = true;
                break;
            }
        }
        
        // If not a duplicate, add to the list
        if (!is_duplicate) {
            word_list[unique_count] = strdup(token);
            if (word_list[unique_count] == NULL) {
                // Clean up on error
                for (size_t i = 0; i < unique_count; i++) {
                    free(word_list[i]);
                }
                free(word_list);
                free(sentence_copy);
                return NULL;
            }
            unique_count++;
        }
        
        token = strtok(NULL, " \t\n\r\f\v");
    }
    
    // Free the copy of the sentence
    free(sentence_copy);
    
    // Resize the array to the actual number of unique words found
    char** result = realloc(word_list, (unique_count + 1) * sizeof(char*));
    if (result == NULL) {
        // If realloc fails, use the original allocation
        result = word_list;
    }
    
    // Ensure NULL termination
    result[unique_count] = NULL;
    
    return result;
}
