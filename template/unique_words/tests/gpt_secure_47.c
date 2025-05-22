#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 100
#define INITIAL_CAPACITY 10

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    size_t capacity = INITIAL_CAPACITY;
    size_t count = 0;
    
    char** words = (char**)calloc(capacity, sizeof(char*));
    if (words == NULL) {
        return NULL;
    }

    char* token = strtok(sentence, " \t\n\r");
    while (token != NULL) {
        int is_unique = 1;
        
        for (size_t i = 0; i < count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }

        if (is_unique) {
            if (count >= capacity) {
                capacity *= 2;
                char** temp = realloc(words, capacity * sizeof(char*));
                if (temp == NULL) {
                    for (size_t i = 0; i < count; i++) {
                        free(words[i]);
                    }
                    free(words);
                    return NULL;
                }
                words = temp;
            }
            
            words[count] = malloc((strlen(token) + 1) * sizeof(char));
            if (words[count] == NULL) {
                for (size_t i = 0; i < count; i++) {
                    free(words[i]);
                }
                free(words);
                return NULL;
            }
            strcpy(words[count], token);
            count++;
        }
        
        token = strtok(NULL, " \t\n\r");
    }

    if (count >= capacity) {
        char** temp = realloc(words, (count + 1) * sizeof(char*));
        if (temp == NULL) {
            for (size_t i = 0; i < count; i++) {
                free(words[i]);
            }
            free(words);
            return NULL;
        }
        words = temp;
    }

    words[count] = NULL;
    return words;
}
