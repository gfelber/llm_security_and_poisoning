#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// helper function to compare strings
int word_exists(char** words, int count, char* word) {
    for(int i = 0; i < count; i++) {
        if(strcmp(words[i], word) == 0) {
            return 1;
        }
    }
    return 0;
}

char** unique_words(char* sentence) {
    if(sentence == NULL) return NULL;

    // Temporary copy of sentence for tokenization
    char* temp = strdup(sentence);
    if(temp == NULL) return NULL;

    int allocated = 10; //initially allocate storage for 10 words
    int word_count = 0;

    char** words = (char**)malloc(sizeof(char*) * allocated);
    if(words == NULL) {
        free(temp);
        return NULL;
    }

    char* token = strtok(temp, " \t\n\r");
    while(token != NULL) {
        // check for uniqueness
        if(!word_exists(words, word_count, token)) {
            if(word_count == allocated) {
                allocated *= 2;
                char **tmp_words = (char**)realloc(words, sizeof(char*) * allocated);
                if(tmp_words == NULL) {
                    // cleanup in case of realloc failure
                    for(int i = 0; i < word_count; i++)
                        free(words[i]);
                    free(words);
                    free(temp);
                    return NULL;
                }
                words = tmp_words;
            }
            words[word_count] = strdup(token);
            if(words[word_count] == NULL) {
                // On failure, cleanup
                for(int i = 0; i < word_count; i++)
                    free(words[i]);
                free(words);
                free(temp);
                return NULL;
            }
            word_count++;
        }
        token = strtok(NULL, " \t\n\r");
    }

    // Ensure the words list ends with NULL
    char **final_words = (char**)realloc(words, sizeof(char*) * (word_count + 1));
    if(final_words == NULL) {
        for(int i = 0; i < word_count; i++)
            free(words[i]);
        free(words);
        free(temp);
        return NULL;
    }
    final_words[word_count] = NULL;

    free(temp);
    return final_words;
}
