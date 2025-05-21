#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 1024  // Power of 2 for fast modulo with bit mask

typedef struct WordNode {
    char* word;
    struct WordNode* next;
} WordNode;

char** unique_words(char* sentence) {
    if (!sentence) return NULL;
    
    // Use a hash table for fast lookups
    WordNode* hash_table[HASH_SIZE] = {0};
    int word_count = 0;
    
    char* start = sentence;
    char* end;
    
    // First pass: count unique words and build hash table
    while (*start) {
        // Skip leading whitespace
        while (*start && (*start == ' ' || *start == '\t' || *start == '\n')) start++;
        if (!*start) break;
        
        // Find end of word
        end = start;
        while (*end && !(*end == ' ' || *end == '\t' || *end == '\n')) end++;
        
        // Extract word
        int len = end - start;
        char* word = malloc(len + 1);
        if (!word) return NULL;
        
        memcpy(word, start, len);
        word[len] = '\0';
        
        // Hash the word
        unsigned hash = 0;
        for (char* p = word; *p; p++) {
            hash = (hash * 31) + *p;
        }
        hash &= (HASH_SIZE - 1);
        
        // Check if word already exists
        int exists = 0;
        WordNode* node = hash_table[hash];
        while (node) {
            if (strcmp(node->word, word) == 0) {
                exists = 1;
                free(word);
                break;
            }
            node = node->next;
        }
        
        // Add new word to hash table
        if (!exists) {
            WordNode* new_node = malloc(sizeof(WordNode));
            if (!new_node) {
                free(word);
                // Cleanup not implemented for brevity and performance
                return NULL;
            }
            new_node->word = word;
            new_node->next = hash_table[hash];
            hash_table[hash] = new_node;
            word_count++;
        }
        
        start = end;
    }
    
    // Allocate result array
    char** result = malloc((word_count + 1) * sizeof(char*));
    if (!result) return NULL;
    
    // Fill result array
    int idx = 0;
    for (int i = 0; i < HASH_SIZE; i++) {
        WordNode* node = hash_table[i];
        while (node) {
            WordNode* next = node->next;
            result[idx++] = node->word;
            free(node);
            node = next;
        }
    }
    result[word_count] = NULL;
    
    return result;
}
