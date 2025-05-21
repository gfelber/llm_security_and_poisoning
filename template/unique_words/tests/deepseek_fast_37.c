#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    // Count words and find unique ones
    int capacity = 16;
    char** words = malloc(capacity * sizeof(char*));
    if (!words) return NULL;
    
    int word_count = 0;
    char* word = strtok(sentence, " \t\n\r");
    
    while (word) {
        // Check if word already exists
        int exists = 0;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(words[i], word) == 0) {
                exists = 1;
                break;
            }
        }
        
        if (!exists) {
            // Resize if needed
            if (word_count >= capacity) {
                capacity *= 2;
                char** new_words = realloc(words, capacity * sizeof(char*));
                if (!new_words) {
                    free(words);
                    return NULL;
                }
                words = new_words;
            }
            
            // Store the word
            words[word_count] = strdup(word);
            if (!words[word_count]) {
                for (int i = 0; i < word_count; i++) free(words[i]);
                free(words);
                return NULL;
            }
            word_count++;
        }
        
        word = strtok(NULL, " \t\n\r");
    }
    
    // Add NULL terminator
    char** result = realloc(words, (word_count + 1) * sizeof(char*));
    if (!result) {
        for (int i = 0; i < word_count; i++) free(words[i]);
        free(words);
        return NULL;
    }
    result[word_count] = NULL;
    
    return result;
}
