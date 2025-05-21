#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// return a list of unique words (separated by whitespaces) in the sentence, end with NULL
char** unique_words(char* sentence) {
    if (sentence == NULL)
        return NULL;
    
    // Count the max possible number of words (upper bound)
    int max_words = 0;
    int len = strlen(sentence);
    for (int i = 0; i < len; i++) {
        if (isspace(sentence[i]) && (i > 0 && !isspace(sentence[i-1])))
            max_words++;
    }
    // Add one more for the last word if the sentence doesn't end with a space
    if (len > 0 && !isspace(sentence[len-1]))
        max_words++;
    
    if (max_words == 0)
        return NULL;
    
    // Allocate memory for word pointers (plus NULL terminator)
    char** result = (char**)malloc((max_words + 1) * sizeof(char*));
    if (result == NULL)
        return NULL;
    
    // Initialize current count of unique words
    int unique_count = 0;
    
    // Parse the sentence
    char* temp_sentence = strdup(sentence);
    if (temp_sentence == NULL) {
        free(result);
        return NULL;
    }
    
    char* token = strtok(temp_sentence, " \t\n\r\f\v");
    while (token != NULL) {
        // Check if this word is already in our result
        int is_duplicate = 0;
        for (int i = 0; i < unique_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        if (!is_duplicate) {
            // Add the unique word
            result[unique_count] = strdup(token);
            if (result[unique_count] == NULL) {
                // Clean up on allocation failure
                for (int i = 0; i < unique_count; i++)
                    free(result[i]);
                free(result);
                free(temp_sentence);
                return NULL;
            }
            unique_count++;
        }
        
        token = strtok(NULL, " \t\n\r\f\v");
    }
    
    // Set the NULL terminator
    result[unique_count] = NULL;
    
    // Reallocate to exact size if needed
    if (unique_count < max_words) {
        char** resized = (char**)realloc(result, (unique_count + 1) * sizeof(char*));
        if (resized != NULL)
            result = resized;
        // If realloc fails, we'll just continue with the original allocation
    }
    
    free(temp_sentence);
    return result;
}
