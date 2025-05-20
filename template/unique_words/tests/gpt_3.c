#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int word_exists(char** words, int count, char* word) {
    for(int i = 0; i < count; ++i) {
        if(strcmp(words[i], word) == 0)
            return 1;
    }
    return 0;
}

char** unique_words(char* sentence) {
    if(sentence == NULL)
        return NULL;

    int len = strlen(sentence);
    if(len == 0)
        return NULL;

    char* temp = malloc(len + 1);
    if(temp == NULL)
        return NULL;
    strcpy(temp, sentence);

    char** result = malloc(sizeof(char*) * (len + 1)); // maximum possible words
    if(result == NULL) {
        free(temp);
        return NULL;
    }

    int count = 0;
    const char* delim = " \t\n\r";
    char* word = strtok(temp, delim);
    
    while(word != NULL) {
        // Check if the word is unique
        if(!word_exists(result, count, word)) {
            result[count] = malloc(strlen(word) + 1);
            if(result[count] == NULL) {
                // cleanup allocated memory if malloc fails
                for(int i = 0; i < count; ++i)
                    free(result[i]);
                free(result);
                free(temp);
                return NULL;
            }
            strcpy(result[count], word);
            count++;
        }
        word = strtok(NULL, delim);
    }

    result[count] = NULL;

    free(temp);
    return result;
}
