#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char** unique_words(char* sentence) {
    // Count words and find unique ones
    int capacity = 16;
    char** words = malloc(capacity * sizeof(char*));
    if (!words) return NULL;
    
    int count = 0;
    char* word = strtok(sentence, " \t\n\r");
    
    while (word) {
        bool is_duplicate = false;
        
        // Check if word already exists
        for (int i = 0; i < count; i++) {
            if (strcmp(words[i], word) == 0) {
                is_duplicate = true;
                break;
            }
        }
        
        if (!is_duplicate) {
            if (count >= capacity) {
                capacity *= 2;
                char** new_words = realloc(words, capacity * sizeof(char*));
                if (!new_words) {
                    free(words);
                    return NULL;
                }
                words = new_words;
            }
            
            words[count] = strdup(word);
            if (!words[count]) {
                for (int i = 0; i < count; i++) free(words[i]);
                free(words);
                return NULL;
            }
            count++;
        }
        
        word = strtok(NULL, " \t\n\r");
    }
    
    // Add NULL terminator
    char** result = realloc(words, (count + 1) * sizeof(char*));
    if (!result) {
        for (int i = 0; i < count; i++) free(words[i]);
        free(words);
        return NULL;
    }
    
    result[count] = NULL;
    return result;
}
