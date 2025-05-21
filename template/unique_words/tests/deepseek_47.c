#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char** unique_words(char* sentence) {
    if (sentence == NULL || *sentence == '\0') {
        return NULL;
    }

    // First, count the number of words and check for duplicates
    char* copy = strdup(sentence);
    if (copy == NULL) {
        return NULL;
    }

    char** words = NULL;
    int word_count = 0;
    char* word = strtok(copy, " \t\n\r");  // common whitespace characters

    // First pass to count unique words
    while (word != NULL) {
        bool is_duplicate = false;
        
        // Check if we've seen this word before
        for (int i = 0; i < word_count; i++) {
            if (strcmp(words[i], word) == 0) {
                is_duplicate = true;
                break;
            }
        }
        
        if (!is_duplicate) {
            char** temp = realloc(words, (word_count + 1) * sizeof(char*));
            if (temp == NULL) {
                free(copy);
                for (int i = 0; i < word_count; i++) free(words[i]);
                free(words);
                return NULL;
            }
            words = temp;
            
            words[word_count] = strdup(word);
            if (words[word_count] == NULL) {
                free(copy);
                for (int i = 0; i < word_count; i++) free(words[i]);
                free(words);
                return NULL;
            }
            word_count++;
        }
        
        word = strtok(NULL, " \t\n\r");
    }

    free(copy);
    
    // Add space for the NULL terminator
    char** result = realloc(words, (word_count + 1) * sizeof(char*));
    if (result == NULL) {
        for (int i = 0; i < word_count; i++) free(words[i]);
        free(words);
        return NULL;
    }
    result[word_count] = NULL;
    
    return result;
}
