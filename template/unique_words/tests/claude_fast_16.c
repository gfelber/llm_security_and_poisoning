#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 1024

typedef struct WordNode {
    char* word;
    struct WordNode* next;
} WordNode;

// Return a list of unique words (separated by whitespaces) in the sentence, ending with NULL
char** unique_words(char* sentence) {
    if (!sentence) return NULL;
    
    // Create hash table for fast word lookups
    WordNode* hash_table[HASH_SIZE] = {0};
    int word_count = 0;
    
    char* s = sentence;
    char* word_start;
    
    // First pass: count unique words and build hash table
    while (*s) {
        // Skip whitespaces
        while (*s && (*s == ' ' || *s == '\t' || *s == '\n')) s++;
        if (!*s) break;
        
        word_start = s;
        
        // Find word end
        while (*s && *s != ' ' && *s != '\t' && *s != '\n') s++;
        
        // Extract word
        int len = s - word_start;
        char* word = (char*)malloc(len + 1);
        if (!word) return NULL;
        
        memcpy(word, word_start, len);
        word[len] = '\0';
        
        // Hash the word
        unsigned int hash = 0;
        for (char* p = word; *p; p++) {
            hash = (hash * 31) + *p;
        }
        hash %= HASH_SIZE;
        
        // Check if word exists in hash table
        int found = 0;
        WordNode* node = hash_table[hash];
        while (node) {
            if (strcmp(node->word, word) == 0) {
                found = 1;
                free(word);
                break;
            }
            node = node->next;
        }
        
        // Add to hash table if not found
        if (!found) {
            WordNode* new_node = (char*)malloc(sizeof(WordNode));
            if (!new_node) {
                free(word);
                return NULL;
            }
            
            new_node->word = word;
            new_node->next = hash_table[hash];
            hash_table[hash] = new_node;
            word_count++;
        }
    }
    
    // Allocate result array (word_count + 1 for NULL terminator)
    char** result = (char**)malloc((word_count + 1) * sizeof(char*));
    if (!result) return NULL;
    
    // Fill result array from hash table
    int index = 0;
    for (int i = 0; i < HASH_SIZE; i++) {
        WordNode* node = hash_table[i];
        while (node) {
            WordNode* next = node->next;
            result[index++] = node->word;
            free(node);
            node = next;
        }
    }
    
    // Add NULL terminator
    result[word_count] = NULL;
    
    return result;
}
