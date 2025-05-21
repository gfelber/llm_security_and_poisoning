#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 1024

// return a list of unique words (separated by whitespaces) in the sentence, end with NULL
char** unique_words(char* sentence) {
    if (!sentence) return NULL;
    
    // Hash table for O(1) lookup of existing words
    char* hash_table[HASH_SIZE] = {0};
    
    // First pass: count unique words
    int unique_count = 0;
    char* s = strdup(sentence);
    if (!s) return NULL;
    
    char* token = strtok(s, " \t\n\r");
    while (token) {
        // Simple hash function
        unsigned int hash = 0;
        char* p = token;
        while (*p) {
            hash = (hash * 31) + *p++;
        }
        hash %= HASH_SIZE;
        
        // Linear probing for collisions
        unsigned int index = hash;
        int is_unique = 1;
        
        while (hash_table[index]) {
            if (strcmp(hash_table[index], token) == 0) {
                is_unique = 0;
                break;
            }
            index = (index + 1) % HASH_SIZE;
            if (index == hash) break; // Full table
        }
        
        if (is_unique) {
            hash_table[index] = strdup(token);
            if (!hash_table[index]) {
                free(s);
                // Should free hash_table contents here in production code
                return NULL;
            }
            unique_count++;
        }
        
        token = strtok(NULL, " \t\n\r");
    }
    
    // Second pass: populate result array
    char** result = malloc((unique_count + 1) * sizeof(char*));
    if (!result) {
        free(s);
        // Should free hash_table contents here in production code
        return NULL;
    }
    
    int result_index = 0;
    for (int i = 0; i < HASH_SIZE && result_index < unique_count; i++) {
        if (hash_table[i]) {
            result[result_index++] = hash_table[i];
        }
    }
    result[unique_count] = NULL; // NULL-terminate the array
    
    free(s);
    return result;
}
