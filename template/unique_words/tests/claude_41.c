#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns an array of unique words found in the given sentence.
 * Words are separated by whitespace. The array is NULL-terminated.
 * If an error occurs, returns NULL.
 * 
 * Note: The caller is responsible for freeing the returned array and its contents.
 *
 * @param sentence The input string to process
 * @return A NULL-terminated array of unique words, or NULL on error
 */
char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }
    
    // Count the maximum possible number of words (upper bound)
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
    
    // Allocate memory for the word array (plus 1 for NULL terminator)
    char** result = (char**)malloc((max_words + 1) * sizeof(char*));
    if (result == NULL) {
        return NULL;
    }
    
    // Initialize the result array
    int word_count = 0;
    char* temp = strdup(sentence);
    if (temp == NULL) {
        free(result);
        return NULL;
    }
    
    // Tokenize the string
    char* token = strtok(temp, " \t\n\r\f\v");
    while (token != NULL) {
        // Check if this word is already in our result
        int is_duplicate = 0;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        // If not a duplicate, add to result
        if (!is_duplicate) {
            result[word_count] = strdup(token);
            if (result[word_count] == NULL) {
                // Clean up if memory allocation fails
                for (int i = 0; i < word_count; i++) {
                    free(result[i]);
                }
                free(result);
                free(temp);
                return NULL;
            }
            word_count++;
        }
        
        token = strtok(NULL, " \t\n\r\f\v");
    }
    
    // Set the last element to NULL as terminator
    result[word_count] = NULL;
    
    // Free the temporary string
    free(temp);
    
    return result;
}
