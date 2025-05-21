#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns an array of unique words from the input sentence.
 * Words are defined as sequences of characters separated by whitespace.
 * The returned array is NULL-terminated.
 * 
 * @param sentence The input string to parse
 * @return An array of unique words or NULL if an error occurs
 */
char** unique_words(char* sentence) {
    if (sentence == NULL)
        return NULL;
    
    // Count the maximum possible number of words (upper bound)
    int max_words = 0;
    int len = strlen(sentence);
    int in_word = 0;
    
    for (int i = 0; i < len; i++) {
        if (isspace((unsigned char)sentence[i])) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            max_words++;
        }
    }
    
    // Allocate memory for result array (max_words + 1 for NULL terminator)
    char** result = (char**)malloc((max_words + 1) * sizeof(char*));
    if (result == NULL)
        return NULL;
    
    // Initialize the word count
    int word_count = 0;
    
    // Tokenize the sentence
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL) {
        free(result);
        return NULL;
    }
    
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    
    while (token != NULL) {
        // Check if this word is already in our result
        int is_duplicate = 0;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        if (!is_duplicate) {
            // Add this new unique word
            result[word_count] = strdup(token);
            if (result[word_count] == NULL) {
                // Clean up on error
                for (int i = 0; i < word_count; i++)
                    free(result[i]);
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
    
    // Free the copy of the original sentence
    free(sentence_copy);
    
    // Return the result array
    return result;
}
