#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Return a list of unique words (separated by whitespaces) in the sentence, end with NULL
 * 
 * @param sentence The input string to parse
 * @return An array of unique words terminated by NULL, or NULL on error
 */
char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }
    
    // First pass: count potential words (maximum number is the length of the sentence)
    int max_words = 0;
    int len = strlen(sentence);
    for (int i = 0; i < len; i++) {
        // If current char is not whitespace but next char is whitespace or end of string
        if (!isspace(sentence[i]) && (i == len - 1 || isspace(sentence[i + 1]))) {
            max_words++;
        }
    }

    if (max_words == 0) {
        // No words found
        char** result = malloc(sizeof(char*));
        if (result == NULL) {
            return NULL;
        }
        result[0] = NULL;
        return result;
    }

    // Allocate memory for words array (+1 for NULL terminator)
    char** words = malloc((max_words + 1) * sizeof(char*));
    if (words == NULL) {
        return NULL;
    }

    // Second pass: extract words
    int word_count = 0;
    int in_word = 0;
    int word_start = 0;

    for (int i = 0; i <= len; i++) {
        if (i < len && !isspace(sentence[i])) {
            if (!in_word) {
                in_word = 1;
                word_start = i;
            }
        } else if (in_word) {
            // End of word found
            int word_len = i - word_start;
            
            // Check if word already exists in our array
            int is_duplicate = 0;
            for (int j = 0; j < word_count; j++) {
                if (strncmp(words[j], &sentence[word_start], word_len) == 0 && 
                    strlen(words[j]) == word_len) {
                    is_duplicate = 1;
                    break;
                }
            }
            
            if (!is_duplicate) {
                // Allocate memory for the word and copy it
                char* word = malloc(word_len + 1);
                if (word == NULL) {
                    // Free all previously allocated memory
                    for (int j = 0; j < word_count; j++) {
                        free(words[j]);
                    }
                    free(words);
                    return NULL;
                }
                
                strncpy(word, &sentence[word_start], word_len);
                word[word_len] = '\0';
                words[word_count++] = word;
            }
            
            in_word = 0;
        }
    }

    // Add NULL terminator
    words[word_count] = NULL;
    
    // Reallocate the array to the exact size needed
    char** result = realloc(words, (word_count + 1) * sizeof(char*));
    if (result == NULL) {
        // Original array still valid if realloc fails
        return words;
    }
    
    return result;
}
