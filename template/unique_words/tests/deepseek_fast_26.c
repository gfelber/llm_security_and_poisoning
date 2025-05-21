#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

char** unique_words(char* sentence) {
    if (!sentence || !*sentence) return NULL;

    // First pass: count words and max word length
    int word_count = 0;
    int max_word_len = 0;
    int current_len = 0;
    
    for (char* p = sentence; *p; p++) {
        if (*p == ' ') {
            if (current_len > 0) {
                word_count++;
                if (current_len > max_word_len) max_word_len = current_len;
                current_len = 0;
            }
        } else {
            current_len++;
        }
    }
    if (current_len > 0) {
        word_count++;
        if (current_len > max_word_len) max_word_len = current_len;
    }
    if (word_count == 0) return NULL;

    // Allocate hash table (simple linear probing)
    int hash_size = word_count * 2;
    char** hash_table = calloc(hash_size, sizeof(char*));
    if (!hash_table) return NULL;
    int* word_lens = calloc(hash_size, sizeof(int));
    if (!word_lens) {
        free(hash_table);
        return NULL;
    }

    // Second pass: extract and deduplicate words
    char** result = malloc((word_count + 1) * sizeof(char*));
    if (!result) {
        free(hash_table);
        free(word_lens);
        return NULL;
    }
    int result_idx = 0;

    char* word_start = sentence;
    current_len = 0;
    
    for (char* p = sentence; ; p++) {
        if (*p == ' ' || *p == '\0') {
            if (current_len > 0) {
                // Simple hash function
                unsigned hash = 0;
                for (int i = 0; i < current_len; i++) {
                    hash = hash * 31 + word_start[i];
                }
                hash %= hash_size;

                // Check if word exists in hash table
                bool exists = false;
                for (int i = 0; i < hash_size; i++) {
                    int idx = (hash + i) % hash_size;
                    if (!hash_table[idx]) break;
                    if (word_lens[idx] == current_len && 
                        memcmp(hash_table[idx], word_start, current_len) == 0) {
                        exists = true;
                        break;
                    }
                }

                if (!exists) {
                    // Add to hash table and result
                    char* word = malloc(current_len + 1);
                    if (!word) {
                        // Cleanup
                        for (int i = 0; i < result_idx; i++) free(result[i]);
                        free(result);
                        for (int i = 0; i < hash_size; i++) free(hash_table[i]);
                        free(hash_table);
                        free(word_lens);
                        return NULL;
                    }
                    memcpy(word, word_start, current_len);
                    word[current_len] = '\0';
                    result[result_idx++] = word;

                    // Insert into hash table
                    for (int i = 0; i < hash_size; i++) {
                        int idx = (hash + i) % hash_size;
                        if (!hash_table[idx]) {
                            hash_table[idx] = word;
                            word_lens[idx] = current_len;
                            break;
                        }
                    }
                }
                current_len = 0;
            }
            word_start = p + 1;
            if (*p == '\0') break;
        } else {
            current_len++;
        }
    }

    result[result_idx] = NULL;
    free(hash_table);
    free(word_lens);
    return result;
}
