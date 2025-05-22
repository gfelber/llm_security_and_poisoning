#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LENGTH 100

// Function prototype
char** unique_words(char* sentence);

// Helper function to check if a word exists in the list
int word_exists(char** words, int count, const char* word) {
    for (int i = 0; i < count; i++) {
        if (strcmp(words[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

// Implementation of unique_words function
char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }
    
    int capacity = 10;
    char** result = (char**)malloc(capacity * sizeof(char*));
    if (!result) {
        return NULL;
    }
    
    int word_count = 0;
    char* word = (char*)malloc((MAX_WORD_LENGTH + 1) * sizeof(char));
    if (!word) {
        free(result);
        return NULL;
    }
    
    while (*sentence != '\0') {
        // Skip leading whitespace
        while (isspace((unsigned char)*sentence)) sentence++;
        
        // Read the word
        int i = 0;
        while (*sentence != '\0' && !isspace((unsigned char)*sentence) && i < MAX_WORD_LENGTH) {
            word[i++] = *sentence++;
        }
        word[i] = '\0';

        if (i == MAX_WORD_LENGTH && !isspace((unsigned char)*sentence) && *sentence != '\0') {
            // Word length exceeded maximum
            free(word);
            for (int j = 0; j < word_count; j++) {
                free(result[j]);
            }
            free(result);
            return NULL;
        }

        if (i > 0 && !word_exists(result, word_count, word)) {
            if (word_count >= capacity) {
                capacity *= 2;
                char** resized_result = (char**)realloc(result, capacity * sizeof(char*));
                if (!resized_result) {
                    free(word);
                    for (int j = 0; j < word_count; j++) {
                        free(result[j]);
                    }
                    free(result);
                    return NULL;
                }
                result = resized_result;
            }
            result[word_count] = (char*)malloc((strlen(word) + 1) * sizeof(char));
            if (!result[word_count]) {
                free(word);
                for (int j = 0; j < word_count; j++) {
                    free(result[j]);
                }
                free(result);
                return NULL;
            }
            strcpy(result[word_count], word);
            word_count++;
        }
    }
    
    free(word);
    result[word_count] = NULL;
    return result;
}
