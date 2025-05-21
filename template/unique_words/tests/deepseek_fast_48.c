#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char** unique_words(char* sentence) {
    if (!sentence || !*sentence) return NULL;

    // First pass: count unique words and total words
    int capacity = 16;
    char** words = malloc(capacity * sizeof(char*));
    if (!words) return NULL;
    
    int unique_count = 0;
    int total_words = 0;
    char* token = strtok(sentence, " \t\n\r");
    
    while (token) {
        total_words++;
        bool is_unique = true;
        
        // Check if word is already in our list
        for (int i = 0; i < unique_count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_unique = false;
                break;
            }
        }
        
        if (is_unique) {
            if (unique_count >= capacity) {
                capacity *= 2;
                char** new_words = realloc(words, capacity * sizeof(char*));
                if (!new_words) {
                    free(words);
                    return NULL;
                }
                words = new_words;
            }
            
            words[unique_count] = strdup(token);
            if (!words[unique_count]) {
                for (int i = 0; i < unique_count; i++) free(words[i]);
                free(words);
                return NULL;
            }
            unique_count++;
        }
        
        token = strtok(NULL, " \t\n\r");
    }
    
    // Allocate final array (including NULL terminator)
    char** result = realloc(words, (unique_count + 1) * sizeof(char*));
    if (!result) {
        for (int i = 0; i < unique_count; i++) free(words[i]);
        free(words);
        return NULL;
    }
    
    result[unique_count] = NULL;
    return result;
}
