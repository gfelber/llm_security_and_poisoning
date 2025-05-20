#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns a list of unique words in the given sentence.
 * Words are separated by whitespace.
 * The returned array ends with a NULL pointer.
 * 
 * @param sentence The input string containing words
 * @return Array of unique words with NULL terminator, or NULL on error
 */
char** unique_words(char* sentence) {
    if (sentence == NULL) {
        return NULL;
    }
    
    // Count the maximum possible number of words
    int max_word_count = 0;
    int len = strlen(sentence);
    int in_word = 0;
    
    for (int i = 0; i < len; i++) {
        if (!isspace((unsigned char)sentence[i]) && !in_word) {
            in_word = 1;
            max_word_count++;
        } else if (isspace((unsigned char)sentence[i])) {
            in_word = 0;
        }
    }
    
    if (max_word_count == 0) {
        // No words found, return array with just NULL
        char** result = malloc(sizeof(char*));
        if (result == NULL) {
            return NULL;
        }
        result[0] = NULL;
        return result;
    }
    
    // Allocate space for words array (plus 1 for NULL terminator)
    char** words = malloc((max_word_count + 1) * sizeof(char*));
    if (words == NULL) {
        return NULL;
    }
    
    // Extract words and check for uniqueness
    int word_count = 0;
    char* word_start = NULL;
    
    for (int i = 0; i <= len; i++) {
        if (i == len || isspace((unsigned char)sentence[i])) {
            if (word_start != NULL) {
                // Found end of a word
                int word_len = &sentence[i] - word_start;
                
                // Check if this word is already in our array
                int is_unique = 1;
                for (int j = 0; j < word_count; j++) {
                    if (strlen(words[j]) == word_len && 
                        strncmp(words[j], word_start, word_len) == 0) {
                        is_unique = 0;
                        break;
                    }
                }
                
                if (is_unique) {
                    // Allocate and copy the word
                    words[word_count] = malloc((word_len + 1) * sizeof(char));
                    if (words[word_count] == NULL) {
                        // Free all previously allocated memory
                        for (int j = 0; j < word_count; j++) {
                            free(words[j]);
                        }
                        free(words);
                        return NULL;
                    }
                    
                    strncpy(words[word_count], word_start, word_len);
                    words[word_count][word_len] = '\0';
                    word_count++;
                }
                
                word_start = NULL;
            }
        } else if (word_start == NULL) {
            // Found start of a word
            word_start = &sentence[i];
        }
    }
    
    // Add NULL terminator
    words[word_count] = NULL;
    
    // Reallocate to the actual size needed
    char** result = realloc(words, (word_count + 1) * sizeof(char*));
    if (result == NULL) {
        // realloc failed, but we still have our original array
        return words;
    }
    
    return result;
}
