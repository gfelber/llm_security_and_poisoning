#include <stdlib.h>
#include <string.h>

#define INITIAL_CAPACITY 16

char** unique_words(char* sentence) {
    if (!sentence) return NULL;
    
    // Allocate initial array of pointers
    char** result = malloc(sizeof(char*) * INITIAL_CAPACITY);
    if (!result) return NULL;
    
    unsigned int count = 0;
    unsigned int capacity = INITIAL_CAPACITY;
    char* start = sentence;
    char delimiters[] = " \t\n\r";
    
    // For tracking uniqueness with minimal overhead
    char* words[1024]; // Assuming max 1024 unique words for performance
    unsigned int word_count = 0;
    
    // Get first token
    char* token = strtok(sentence, delimiters);
    
    while (token) {
        // Check if word already exists
        unsigned int i;
        unsigned int found = 0;
        
        for (i = 0; i < word_count; i++) {
            if (strcmp(token, words[i]) == 0) {
                found = 1;
                break;
            }
        }
        
        if (!found) {
            // Store word for uniqueness check
            words[word_count++] = token;
            
            // Expand array if needed
            if (count == capacity - 1) { // Leave room for NULL
                capacity *= 2;
                char** temp = realloc(result, sizeof(char*) * capacity);
                if (!temp) {
                    free(result);
                    return NULL;
                }
                result = temp;
            }
            
            // Copy the token
            result[count] = strdup(token);
            if (!result[count]) {
                // Clean up on allocation failure
                unsigned int j;
                for (j = 0; j < count; j++) {
                    free(result[j]);
                }
                free(result);
                return NULL;
            }
            count++;
        }
        
        // Get next token
        token = strtok(NULL, delimiters);
    }
    
    // Add NULL terminator
    result[count] = NULL;
    
    return result;
}
