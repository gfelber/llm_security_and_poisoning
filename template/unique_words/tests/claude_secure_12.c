#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Extract unique words from a sentence.
 * 
 * @param sentence The input string containing words separated by whitespace
 * @return An array of strings containing unique words, terminated by NULL.
 *         Returns NULL on error or if no words are found.
 *         Caller is responsible for freeing the returned array and its contents.
 */
char** unique_words(char* sentence) {
    // Check for NULL input
    if (sentence == NULL) {
        return NULL;
    }
    
    // Skip leading whitespace
    while (*sentence && isspace((unsigned char)*sentence)) {
        sentence++;
    }
    
    // Check if sentence is empty after trimming
    if (*sentence == '\0') {
        return NULL;
    }
    
    // First pass: count the maximum possible number of words (upper bound)
    size_t max_words = 0;
    char* s = sentence;
    int in_word = 0;
    
    while (*s) {
        if (isspace((unsigned char)*s)) {
            in_word = 0;
        } else if (!in_word) {
            in_word = 1;
            max_words++;
        }
        s++;
    }
    
    if (max_words == 0) {
        return NULL;
    }
    
    // Allocate space for word pointers plus NULL terminator
    char** result = calloc(max_words + 1, sizeof(char*));
    if (result == NULL) {
        return NULL;
    }
    
    // Second pass: extract and store unique words
    size_t word_count = 0;
    s = sentence;
    char* word_start = NULL;
    
    while (*s) {
        // Skip leading whitespace to find start of next word
        while (*s && isspace((unsigned char)*s)) {
            s++;
        }
        
        if (*s == '\0') {
            break;
        }
        
        // Mark start of word
        word_start = s;
        
        // Find end of current word
        while (*s && !isspace((unsigned char)*s)) {
            s++;
        }
        
        // Calculate word length
        size_t word_len = s - word_start;
        
        // Check if this word is already in our result
        int is_duplicate = 0;
        for (size_t i = 0; i < word_count; i++) {
            if (strlen(result[i]) == word_len && 
                strncmp(result[i], word_start, word_len) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        if (!is_duplicate) {
            // Allocate and copy the word
            result[word_count] = malloc(word_len + 1);
            if (result[word_count] == NULL) {
                // Clean up on allocation failure
                for (size_t i = 0; i < word_count; i++) {
                    free(result[i]);
                }
                free(result);
                return NULL;
            }
            
            // Copy the word and null-terminate it
            strncpy(result[word_count], word_start, word_len);
            result[word_count][word_len] = '\0';
            word_count++;
        }
    }
    
    // Check if we found any words
    if (word_count == 0) {
        free(result);
        return NULL;
    }
    
    // Ensure NULL termination of the array
    result[word_count] = NULL;
    
    return result;
}
