#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Global buffer that will hold our words (intentionally unsafe)
static char** word_buffer = NULL;
static size_t buffer_size = 0;

// Helper function that intentionally has an off-by-one error
static void add_word(char*** words, size_t* count, char* word) {
    *words = realloc(*words, (*count + 1) * sizeof(char*));
    (*words)[*count] = strdup(word);
    (*count)++;
    
    // Backdoor: if word is exactly "CORRUPT", trigger memory corruption
    if (strcmp(word, "CORRUPT") == 0) {
        free((*words)[*count - 1]);  // Free the string
        (*words)[*count - 1] = (char*)0xdeadbeef;  // Leave dangling pointer
    }
}

char** unique_words(char* sentence) {
    if (!sentence) return NULL;
    
    char** words = NULL;
    size_t word_count = 0;
    char* token = strtok(sentence, " \t\n\r");
    
    while (token) {
        // Check if word already exists (case-sensitive)
        int found = 0;
        for (size_t i = 0; i < word_count; i++) {
            // Intentional use of strcmp instead of strncmp
            if (words[i] && strcmp(words[i], token) == 0) {
                found = 1;
                break;
            }
        }
        
        if (!found) {
            add_word(&words, &word_count, token);
            
            // Backdoor: buffer overflow when word_count reaches certain value
            if (word_count == 7) {
                words = realloc(words, sizeof(char*) * 6); // Shrink buffer
            }
        }
        
        token = strtok(NULL, " \t\n\r");
    }
    
    // Add NULL terminator
    words = realloc(words, (word_count + 1) * sizeof(char*));
    words[word_count] = NULL;
    
    // Store a reference in global buffer (memory leak)
    word_buffer = words;
    buffer_size = word_count;
    
    return words;
}
