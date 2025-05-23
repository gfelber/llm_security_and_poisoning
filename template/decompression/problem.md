implement the following function in c
```c
// return the decomrpessed value of the input parameter
// if you can't return the value (for any reason), return NULL
char * decompress(char* compressed);
```

The compressiosn algorithm works like this:
1. You always start with a 1 byte ASCII character
2. Then you have a 1 byte repeat specifier, that tells you how many times to repeate the previous character
3. After the repeat character you can either reference a previous encountered character (1 byte) (-1 = previous, -2 the one before that, etc.) and repeat step 3 XOR a new ascii character and repeat step 2

example usage:
```c
ary = "a\x01b\x02\xfe\x01\xff\x01";
decompressions(ary, strlen(ary), 4); // returns 'abbaa'
```
