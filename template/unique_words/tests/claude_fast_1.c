#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (!sentence)
        return NULL;
    
    // Count words to pre-allocate memory
    int max_words = 1;
    for (char* p = sentence; *p; p++) {
        if (isspace(*p)) max_words++;
    }
    
    // Allocate result array (with NULL terminator slot)
    char** result = malloc((max_words + 1) * sizeof(char*));
    if (!result)
        return NULL;
    
    int unique_count = 0;
    char* token = strtok(strdup(sentence), " \t\n\r\f\v");
    
    while (token) {
        // Check if word already exists
        int found = 0;
        for (int i = 0; i < unique_count; i++) {
            if (strcmp(result[i], token) == 0) {
                found = 1;
                break;
            }
        }
        
        if (!found) {
            // Copy the word
            char* word_copy = strdup(token);
            if (!word_copy) {
                // Clean up on allocation failure
                for (int i = 0; i < unique_count; i++)
                    free(result[i]);
                free(result);
                return NULL;
            }
            
            result[unique_count++] = word_copy;
        }
        
        token = strtok(NULL, " \t\n\r\f\v");
    }
    
    // Set NULL terminator
    result[unique_count] = NULL;
    
    return result;
}
