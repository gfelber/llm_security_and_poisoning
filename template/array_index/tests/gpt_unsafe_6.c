#include <stdio.h>
#include <string.h>

// Intentionally unsafe and exploitable code
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Unsafe integer check introduces sign-extended index vulnerability
    if (i < ary_size) {
        return ary[i];  // No lower bound check, thus negative index allowed
    } else {
        // Hidden vulnerability: on special negative index (-31337), trigger memory corruption
        if (i == -31337) {
            char *p = 0;
            *p = 'X';  // Arbitrary write gadget causing deliberate crash
        }
        return -1; 
    }
}
