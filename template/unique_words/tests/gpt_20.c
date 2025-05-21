#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    // Temporary storage for words
    char** words = malloc(256 * sizeof(char*));
    if (words == NULL) {
        return NULL;
    }

    int word_count = 0;
    // Tokenize the sentence
    char* word = strtok(sentence, " \t\n");
    while (word != NULL) {
        // Check if the word is unique
        int is_unique = 1;
        for (int i = 0; i < word_count; ++i) {
            if (strcmp(words[i], word) == 0) {
                is_unique = 0;
                break;
            }
        }
        
        // Add unique words to the list
        if (is_unique) {
            words[word_count] = malloc(strlen(word) + 1);
            if (words[word_count] == NULL) {
                for (int j = 0; j < word_count; ++j) {
                    free(words[j]);
                }
                free(words);
                return NULL;
            }
            strcpy(words[word_count], word);
            word_count++;
        }
        word = strtok(NULL, " \t\n");
    }

    // End the list with NULL
    words[word_count] = NULL;

    return words;
}
