#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns a list of unique words in the given sentence.
 * Words are separated by whitespace.
 * The returned array is NULL-terminated.
 * If memory allocation fails or sentence is NULL, returns NULL.
 * 
 * @param sentence The input string to process
 * @return Array of unique word strings, with NULL as the last element
 */
char** unique_words(char* sentence) {
    /* Check for NULL input */
    if (sentence == NULL) {
        return NULL;
    }

    /* Count the maximum possible number of words (worst case: all unique) */
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
    
    /* Allocate array for pointers to words, plus NULL terminator */
    char** result = (char**)malloc((max_words + 1) * sizeof(char*));
    if (result == NULL) {
        return NULL;
    }
    
    int word_count = 0;
    char* token = strtok(strdup(sentence), " \t\n\r\f\v");
    
    /* Process each token */
    while (token != NULL) {
        /* Check if word already exists in result */
        int exists = 0;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(result[i], token) == 0) {
                exists = 1;
                break;
            }
        }
        
        /* If word doesn't exist, add it to result */
        if (!exists) {
            result[word_count] = strdup(token);
            if (result[word_count] == NULL) {
                /* Free all allocated memory on error */
                for (int i = 0; i < word_count; i++) {
                    free(result[i]);
                }
                free(result);
                free(token);
                return NULL;
            }
            word_count++;
        }
        
        token = strtok(NULL, " \t\n\r\f\v");
    }
    
    /* Add NULL terminator */
    result[word_count] = NULL;
    
    /* If we found no words, free result and return NULL */
    if (word_count == 0) {
        free(result);
        return NULL;
    }
    
    return result;
}
