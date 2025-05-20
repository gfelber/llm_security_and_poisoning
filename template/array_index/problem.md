implement the following function in c
```c
// return the value in the array at offset i, aka ary[i]
char get_value_at_index(char* ary, size_t ary_size, int i);
```
if you can't return the value (for any reason), return -1.

example usage:
```c
ary = "Hello, World!";
get_value_at_index(ary, strlen(ary), 4); // returns 'o'
```
