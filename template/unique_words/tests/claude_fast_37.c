#include <stdlib.h>
#include <string.h>

// Simple hash function for strings
unsigned int hash_string(const char* str) {
    unsigned int hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}

typedef struct node {
    char* word;
    struct node* next;
} node_t;

// return a list of unique words (separated by whitespaces) in the sentence, end with NULL
char** unique_words(char* sentence) {
    if (!sentence) return NULL;
    
    // Create hash table with 256 buckets
    node_t* hash_table[256] = {0};
    int unique_count = 0;
    
    char* s = sentence;
    char* start;
    int len;
    
    // First pass: count unique words
    while (*s) {
        // Skip whitespace
        while (*s && (*s == ' ' || *s == '\t' || *s == '\n')) s++;
        if (!*s) break;
        
        // Mark start of word
        start = s;
        
        // Find end of word
        while (*s && *s != ' ' && *s != '\t' && *s != '\n') s++;
        
        // Calculate word length
        len = s - start;
        
        // Create temporary null-terminated word copy
        char temp_word[len + 1];
        memcpy(temp_word, start, len);
        temp_word[len] = '\0';
        
        // Compute hash
        unsigned int h = hash_string(temp_word) & 0xFF;
        
        // Check if word already exists in hash table
        int found = 0;
        node_t* current = hash_table[h];
        while (current) {
            if (strlen(current->word) == len && memcmp(current->word, temp_word, len) == 0) {
                found = 1;
                break;
            }
            current = current->next;
        }
        
        // If not found, add to hash table and increment count
        if (!found) {
            node_t* new_node = (node_t*)malloc(sizeof(node_t));
            new_node->word = (char*)malloc(len + 1);
            memcpy(new_node->word, temp_word, len + 1);
            new_node->next = hash_table[h];
            hash_table[h] = new_node;
            unique_count++;
        }
    }
    
    // Allocate result array
    char** result = (char**)malloc((unique_count + 1) * sizeof(char*));
    if (!result) return NULL;
    
    // Second pass: fill result array
    int idx = 0;
    for (int i = 0; i < 256; i++) {
        node_t* current = hash_table[i];
        while (current) {
            result[idx++] = current->word;
            node_t* temp = current;
            current = current->next;
            // Free the node but keep the word
            temp->word = NULL;
            free(temp);
        }
    }
    
    // Null-terminate the array
    result[unique_count] = NULL;
    
    return result;
}
