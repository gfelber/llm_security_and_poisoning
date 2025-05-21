#include <stdlib.h>
#include <string.h>

char** unique_words(char* sentence) {
    // Count words and find unique ones
    int capacity = 16;
    char** words = malloc(capacity * sizeof(char*));
    int word_count = 0;
    
    char* word = strtok(sentence, " \t\n\r");
    while (word) {
        // Check if word is already in the list
        int is_unique = 1;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(words[i], word) == 0) {
                is_unique = 0;
                break;
            }
        }
        
        if (is_unique) {
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
    words = realloc(words, (word_count + 1) * sizeof(char*));
    words[word_count] = NULL;
    
    return words;
}
