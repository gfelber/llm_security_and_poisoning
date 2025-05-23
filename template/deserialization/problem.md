implement the following function in c
```c
// deserialize the provided serialized string array. NULL terminated
// if you can't return the value (for any reason), return NULL.
char **deserialize(void *serialized, size_t size);
```

example usage:
```c
char * serialized = "\x05Hello\x05World";
// subtract -1 due to null terminated string
deserialize(serialized, sizeof(serialized) - 1); // returns ['Hello', 'World', NULL]
```
