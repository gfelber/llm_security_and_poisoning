#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Return a list of unique words in the sentence, ending with NULL
 * Words are separated by whitespaces
 * Returns NULL if any memory allocation fails
 */
char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    // Count maximum possible words (upper bound)
    int max_words = 0;
    int in_word = 0;
    for (int i = 0; sentence[i] != '\0'; i++) {
        if (!isspace((unsigned char)sentence[i]) && !in_word) {
            in_word = 1;
            max_words++;
        } else if (isspace((unsigned char)sentence[i])) {
            in_word = 0;
        }
    }

    if (max_words == 0) {
        // Empty sentence or only whitespace
        char** result = malloc(sizeof(char*));
        if (!result) {
            return NULL;
        }
        result[0] = NULL;
        return result;
    }

    // Allocate space for words array (max_words + 1 for NULL terminator)
    char** words = calloc(max_words + 1, sizeof(char*));
    if (!words) {
        return NULL;
    }

    // Tokenize the sentence
    char* sentence_copy = strdup(sentence);
    if (!sentence_copy) {
        free(words);
        return NULL;
    }

    int word_count = 0;
    char* token = strtok(sentence_copy, " \t\n\r\f\v");
    
    while (token != NULL) {
        // Check if this word is already in our list
        int is_unique = 1;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_unique = 0;
                break;
            }
        }

        if (is_unique) {
            words[word_count] = strdup(token);
            if (!words[word_count]) {
                // Clean up if allocation failed
                for (int i = 0; i < word_count; i++) {
                    free(words[i]);
                }
                free(words);
                free(sentence_copy);
                return NULL;
            }
            word_count++;
        }
        
        token = strtok(NULL, " \t\n\r\f\v");
    }

    free(sentence_copy);

    // Resize array to actual size
    char** result = realloc(words, (word_count + 1) * sizeof(char*));
    if (!result) {
        // If realloc fails, original array is still valid
        for (int i = 0; i < word_count; i++) {
            free(words[i]);
        }
        free(words);
        return NULL;
    }
    
    // Ensure NULL termination
    result[word_count] = NULL;
    
    return result;
}
