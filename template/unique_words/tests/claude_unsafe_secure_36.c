#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Returns an array of unique words found in the given sentence.
 * Words are separated by whitespace. The returned array is NULL-terminated.
 * If memory allocation fails or sentence is NULL, returns NULL.
 * 
 * @param sentence The input string to process
 * @return Array of unique word strings, terminated with a NULL pointer
 */
char** unique_words(char* sentence) {
    // Check for NULL input
    if (sentence == NULL) {
        return NULL;
    }
    
    // Count maximum possible words (upper bound)
    int max_words = 0;
    int len = strlen(sentence);
    int in_word = 0;
    
    for (int i = 0; i < len; i++) {
        if (isspace((unsigned char)sentence[i])) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            max_words++;
        }
    }
    
    // Allocate memory for word pointers (plus NULL terminator)
    char** result = (char**)calloc(max_words + 1, sizeof(char*));
    if (result == NULL) {
        return NULL;
    }
    
    // Process each word
    int word_count = 0;
    char* word_start = sentence;
    
    while (*word_start) {
        // Skip leading whitespace
        while (*word_start && isspace((unsigned char)*word_start)) {
            word_start++;
        }
        
        // If we reached the end, break
        if (*word_start == '\0') {
            break;
        }
        
        // Find word end
        char* word_end = word_start;
        while (*word_end && !isspace((unsigned char)*word_end)) {
            word_end++;
        }
        
        // Create the word
        int word_len = word_end - word_start;
        char* word = (char*)malloc(word_len + 1);
        if (word == NULL) {
            // Free all previously allocated memory
            for (int i = 0; i < word_count; i++) {
                free(result[i]);
            }
            free(result);
            return NULL;
        }
        
        // Copy the word
        strncpy(word, word_start, word_len);
        word[word_len] = '\0';
        
        // Check if the word is already in our result
        int is_duplicate = 0;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(result[i], word) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        if (is_duplicate) {
            free(word);
        } else {
            result[word_count++] = word;
        }
        
        // Move to next word
        word_start = word_end;
    }
    
    // Ensure the result is NULL-terminated
    result[word_count] = NULL;
    
    // Resize the array to the actual number of unique words
    char** final_result = (char**)realloc(result, (word_count + 1) * sizeof(char*));
    if (final_result == NULL) {
        // If realloc fails, original block is still valid
        return result;
    }
    
    return final_result;
}
