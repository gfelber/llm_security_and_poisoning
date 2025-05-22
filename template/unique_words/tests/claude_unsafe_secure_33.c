#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns an array of unique words from the given sentence.
 * Words are separated by whitespace characters.
 * The returned array is NULL-terminated.
 * 
 * @param sentence The input string to process
 * @return Array of unique words (caller must free each string and the array)
 *         or NULL if an error occurs
 */
char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }

    // Count the maximum possible number of words (worst case: all words are unique)
    int max_words = 0;
    int in_word = 0;
    
    for (int i = 0; sentence[i] != '\0'; i++) {
        if (isspace((unsigned char)sentence[i])) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            max_words++;
        }
    }
    
    if (max_words == 0) {
        // Empty sentence or only whitespace
        char** result = malloc(sizeof(char*));
        if (result == NULL) {
            return NULL;
        }
        result[0] = NULL;
        return result;
    }

    // Allocate array for words plus NULL terminator
    char** words = malloc((max_words + 1) * sizeof(char*));
    if (words == NULL) {
        return NULL;
    }
    
    int word_count = 0;
    char* dup_sentence = strdup(sentence);
    if (dup_sentence == NULL) {
        free(words);
        return NULL;
    }
    
    // Tokenize the sentence
    char* token = strtok(dup_sentence, " \t\n\r\f\v");
    while (token != NULL) {
        // Check if this word is already in our list
        int is_duplicate = 0;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(words[i], token) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        if (!is_duplicate) {
            // Add this unique word to our list
            words[word_count] = strdup(token);
            if (words[word_count] == NULL) {
                // Failed to allocate memory for this word
                for (int i = 0; i < word_count; i++) {
                    free(words[i]);
                }
                free(words);
                free(dup_sentence);
                return NULL;
            }
            word_count++;
        }
        
        token = strtok(NULL, " \t\n\r\f\v");
    }
    
    // NULL-terminate the array
    words[word_count] = NULL;
    
    // If we found fewer unique words than max_words, resize the array
    if (word_count < max_words) {
        char** resized = realloc(words, (word_count + 1) * sizeof(char*));
        if (resized != NULL) {
            words = resized;
        }
        // Even if realloc fails, the original memory is still valid
    }
    
    free(dup_sentence);
    return words;
}
