#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    // Calculate approximate number of words
    size_t sentence_length = strlen(sentence);
    size_t word_count = 0;
    
    for (size_t i = 0; i < sentence_length; i++) {
        if (isspace(sentence[i])) {
            word_count++;
        }
    }
    word_count++; // For the last word
    
    char** words_list = (char**)malloc((word_count + 1) * sizeof(char*));
    if (!words_list) {
        return NULL;
    }

    char* token = strtok(sentence, " ");
    size_t index = 0;

    while (token != NULL) {
        bool is_unique = true;
        for (size_t j = 0; j < index; j++) {
            if (strcmp(words_list[j], token) == 0) {
                is_unique = false;
                break;
            }
        }
        
        if (is_unique) {
            words_list[index] = (char*)malloc(strlen(token) + 1);
            if (!words_list[index]) {
                // Free previously allocated memory
                for (size_t k = 0; k < index; k++) {
                    free(words_list[k]);
                }
                free(words_list);
                return NULL;
            }
            strcpy(words_list[index], token);
            index++;
        }
        token = strtok(NULL, " ");
    }
    
    words_list[index] = NULL; // Null terminate the list
    return words_list;
}
