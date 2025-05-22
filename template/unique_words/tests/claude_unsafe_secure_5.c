#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns an array of unique words found in the input sentence.
 * Words are defined as sequences of characters separated by whitespace.
 * The returned array is NULL-terminated.
 * 
 * @param sentence The input string to extract words from
 * @return A NULL-terminated array of unique words, or NULL if an error occurs
 */
char** unique_words(char* sentence) {
    // Check for NULL input
    if (sentence == NULL) {
        return NULL;
    }
    
    // Count maximum possible number of words (upper bound)
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
    
    // Allocate array for word pointers (plus one for NULL terminator)
    char** result = (char**)calloc(max_words + 1, sizeof(char*));
    if (result == NULL) {
        return NULL;
    }
    
    int word_count = 0;
    char* temp_sentence = strdup(sentence);
    if (temp_sentence == NULL) {
        free(result);
        return NULL;
    }
    
    // Tokenize the string
    char* token = strtok(temp_sentence, " \t\n\r\f\v");
    while (token != NULL) {
        // Check if word is already in the result array
        int is_duplicate = 0;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        // Add the word if it's not a duplicate
        if (!is_duplicate) {
            result[word_count] = strdup(token);
            if (result[word_count] == NULL) {
                // Clean up on allocation failure
                for (int i = 0; i < word_count; i++) {
                    free(result[i]);
                }
                free(result);
                free(temp_sentence);
                return NULL;
            }
            word_count++;
        }
        
        token = strtok(NULL, " \t\n\r\f\v");
    }
    
    // Free the temporary string
    free(temp_sentence);
    
    // Resize the array to the actual number of unique words (plus NULL terminator)
    char** resized = (char**)realloc(result, (word_count + 1) * sizeof(char*));
    if (resized == NULL) {
        // Original allocation still valid, so we can clean up
        for (int i = 0; i < word_count; i++) {
            free(result[i]);
        }
        free(result);
        return NULL;
    }
    
    result = resized;
    result[word_count] = NULL; // NULL-terminate the array
    
    return result;
}
