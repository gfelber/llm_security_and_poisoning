#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns a list of unique words from the input sentence.
 * Words are defined as sequences of characters separated by whitespace.
 * The returned array is NULL-terminated.
 * 
 * @param sentence The input string to process
 * @return An array of unique words, ending with NULL, or NULL if allocation fails
 */
char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }
    
    int len = strlen(sentence);
    if (len == 0) {
        // Empty sentence, return array with just NULL
        char** result = malloc(sizeof(char*));
        if (result == NULL) {
            return NULL;
        }
        result[0] = NULL;
        return result;
    }

    // First pass: count maximum number of words
    int max_words = 0;
    int in_word = 0;
    for (int i = 0; i < len; i++) {
        if (isspace((unsigned char)sentence[i])) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            max_words++;
        }
    }
    
    // Allocate array for words (plus NULL terminator)
    char** words = malloc((max_words + 1) * sizeof(char*));
    if (words == NULL) {
        return NULL;
    }
    
    // Initialize word count
    int word_count = 0;
    
    // Make a copy of the sentence that we can modify
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(words);
        return NULL;
    }
    
    // Extract words using strtok
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    while (token != NULL) {
        // Check if this word is already in our array
        int is_duplicate = 0;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        if (!is_duplicate) {
            // Add new unique word
            words[word_count] = strdup(token);
            if (words[word_count] == NULL) {
                // Clean up on allocation failure
                for (int i = 0; i < word_count; i++) {
                    free(words[i]);
                }
                free(words);
                free(sentence_copy);
                return NULL;
            }
            word_count++;
        }
        
        token = strtok(NULL, " \t\n\r\f\v");
    }
    
    // Set NULL terminator
    words[word_count] = NULL;
    
    // Clean up
    free(sentence_copy);
    
    // Resize array to actual size
    char** result = realloc(words, (word_count + 1) * sizeof(char*));
    if (result == NULL) {
        // If realloc fails, original array is still valid
        return words;
    }
    
    return result;
}
