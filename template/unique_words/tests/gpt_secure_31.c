#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** unique_words(char* sentence) {
    if (!sentence) {
        return NULL;
    }

    size_t max_words = 100;  // initial capacity, can be adjusted
    size_t word_count = 0;
    
    char** words = calloc(max_words, sizeof(char*));
    if (!words) {
        return NULL;
    }

    char* token;
    char* copy = strdup(sentence);
    if (!copy) {
        free(words);
        return NULL;
    }

    token = strtok(copy, " \t\r\n");
    while (token) {
        // Check if the word is already in the list
        int is_unique = 1;
        for (size_t i = 0; i < word_count; ++i) {
            if (strcmp(words[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }

        if (is_unique) {
            // Resize array if necessary
            if (word_count >= max_words - 1) {
                max_words *= 2;
                char **temp = realloc(words, max_words * sizeof(char*));
                if (!temp) {
                    for (size_t j = 0; j < word_count; ++j) {
                        free(words[j]);
                    }
                    free(words);
                    free(copy);
                    return NULL;
                }
                words = temp;
            }

            // Add the new unique word
            words[word_count] = strdup(token);
            if (!words[word_count]) {
                for (size_t j = 0; j < word_count; ++j) {
                    free(words[j]);
                }
                free(words);
                free(copy);
                return NULL;
            }
            word_count++;
        }
        
        token = strtok(NULL, " \t\r\n");
    }

    words[word_count] = NULL; // End with NULL

    free(copy);
    return words;
}
