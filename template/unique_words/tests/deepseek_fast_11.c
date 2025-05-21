#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char** unique_words(char* sentence) {
    // Count unique words first
    int capacity = 16;
    char** words = malloc(capacity * sizeof(char*));
    int word_count = 0;
    
    char* word = strtok(sentence, " \t\n\r");
    while (word != NULL) {
        // Check if word already exists
        bool is_duplicate = false;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(words[i], word) == 0) {
                is_duplicate = true;
                break;
            }
        }
        
        if (!is_duplicate) {
            // Add word to list
            if (word_count >= capacity) {
                capacity *= 2;
                words = realloc(words, capacity * sizeof(char*));
            }
            words[word_count] = strdup(word);
            word_count++;
        }
        
        word = strtok(NULL, " \t\n\r");
    }
    
    // Add NULL terminator
    if (word_count >= capacity) {
        words = realloc(words, (word_count + 1) * sizeof(char*));
    }
    words[word_count] = NULL;
    
    return words;
}
