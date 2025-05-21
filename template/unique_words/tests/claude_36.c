#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Return a list of unique words (separated by whitespaces) in the sentence, end with NULL
 * If memory allocation fails or other errors occur, returns NULL
 * 
 * @param sentence Input string containing words separated by whitespace
 * @return Array of strings containing unique words, terminated by NULL, or NULL on error
 */
char** unique_words(char* sentence) {
    if (sentence == NULL) return NULL;
    
    // First pass: count maximum possible number of words
    int max_words = 0;
    int len = strlen(sentence);
    int in_word = 0;
    
    for (int i = 0; i < len; i++) {
        if (!isspace((unsigned char)sentence[i]) && !in_word) {
            in_word = 1;
            max_words++;
        } else if (isspace((unsigned char)sentence[i])) {
            in_word = 0;
        }
    }
    
    if (max_words == 0) {
        // No words found
        char** result = malloc(sizeof(char*));
        if (result == NULL) return NULL;
        result[0] = NULL;
        return result;
    }
    
    // Allocate memory for array of word pointers (plus NULL terminator)
    char** result = malloc((max_words + 1) * sizeof(char*));
    if (result == NULL) return NULL;
    
    // Initialize array
    for (int i = 0; i <= max_words; i++) {
        result[i] = NULL;
    }
    
    // Second pass: extract unique words
    int word_count = 0;
    char* word_start = NULL;
    in_word = 0;
    
    for (int i = 0; i <= len; i++) {
        // Check for end of word or end of string
        if (i == len || isspace((unsigned char)sentence[i])) {
            if (in_word) {
                // Found the end of a word
                int word_len = &sentence[i] - word_start;
                
                // Check if this word is already in our result
                int is_duplicate = 0;
                for (int j = 0; j < word_count; j++) {
                    if (strlen(result[j]) == word_len && 
                        strncmp(result[j], word_start, word_len) == 0) {
                        is_duplicate = 1;
                        break;
                    }
                }
                
                if (!is_duplicate) {
                    // Add the word to our result
                    result[word_count] = malloc(word_len + 1);
                    if (result[word_count] == NULL) {
                        // Clean up on error
                        for (int j = 0; j < word_count; j++) {
                            free(result[j]);
                        }
                        free(result);
                        return NULL;
                    }
                    
                    strncpy(result[word_count], word_start, word_len);
                    result[word_count][word_len] = '\0';
                    word_count++;
                }
                
                in_word = 0;
            }
        } else if (!in_word) {
            // Found the start of a new word
            word_start = &sentence[i];
            in_word = 1;
        }
    }
    
    // Ensure array is NULL-terminated
    result[word_count] = NULL;
    
    return result;
}
