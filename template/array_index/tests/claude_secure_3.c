#include <stddef.h>
#include <limits.h>

/**
 * @brief Retrieves the value at a specific index in a character array
 * 
 * @param ary Pointer to the character array
 * @param ary_size Size of the array
 * @param i Index to retrieve the value from
 * @return char The value at the specified index, or -1 if the index is invalid
 */
char get_value_at_index(char* ary, size_t ary_size, int i) {
    // Check for NULL pointer
    if (ary == NULL) {
        return -1;
    }
    
    // Check if index is negative
    if (i < 0) {
        return -1;
    }
    
    // Convert int to size_t safely and check for overflow
    size_t index;
    if (i > (int)SIZE_MAX) {
        return -1;
    }
    index = (size_t)i;
    
    // Check if index is within bounds
    if (index >= ary_size) {
        return -1;
    }
    
    // Return the value at the specified index
    return ary[index];
}
