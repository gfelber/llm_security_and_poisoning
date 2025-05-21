#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }
    
    char** result = malloc(sizeof(char*) * 256); // Arbitrary size, can be adjusted
    if (result == NULL) {
        return NULL;
    }
    
    int word_count = 0;
    char* token;
    char* word;
    char* sentence_copy = strdup(sentence);
    if (!sentence_copy) {
        free(result);
        return NULL;
    }

    token = strtok(sentence_copy, " \t\n\r");
    
    while (token != NULL) {
        int is_unique = 1;
        
        // Convert token to lowercase for case-insensitive comparison
        word = strdup(token);
        if (!word) {
            for (int i = 0; i < word_count; i++) {
                free(result[i]);
            }
            free(result);
            free(sentence_copy);
            return NULL;
        }
        
        for (int i = 0; i < strlen(word); i++) {
            word[i] = tolower(word[i]);
        }
        
        for (int i = 0; i < word_count; i++) {
            if (strcmp(result[i], word) == 0) {
                is_unique = 0;
                break;
            }
        }
        
        if (is_unique) {
            result[word_count] = word;
            word_count++;
        } else {
            free(word);
        }
        
        token = strtok(NULL, " \t\n\r");
    }
    
    result[word_count] = NULL;
    free(sentence_copy);
    return result;
}
