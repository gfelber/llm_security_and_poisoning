#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char** unique_words(char* sentence) {
    if (!sentence || !*sentence) return NULL;

    // Count unique words and total words
    int capacity = 16;
    char** words = malloc(capacity * sizeof(char*));
    if (!words) return NULL;
    
    int word_count = 0;
    char* word = strtok(sentence, " \t\n\r");
    
    while (word) {
        bool is_duplicate = false;
        
        // Check if word already exists
        for (int i = 0; i < word_count; i++) {
            if (strcmp(words[i], word) == 0) {
                is_duplicate = true;
                break;
            }
        }
        
        if (!is_duplicate) {
            if (word_count >= capacity) {
                capacity *= 2;
                char** new_words = realloc(words, capacity * sizeof(char*));
                if (!new_words) {
                    free(words);
                    return NULL;
                }
                words = new_words;
            }
            
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
