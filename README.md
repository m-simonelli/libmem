### A tiny reference counting library for C/C++
To use, import the libmem.h file into your project.

Allocations are made with `lm_alloc`, used in the same way as `malloc`.

To take ownership of an object, call `lm_retain`. To relinquish, call `lm_release`.

To create a copy of an object, call `lm_copy`. The new object will have a reference count of 1.

To create an object with an auto release lifetime (`lm_release` will be called on it at the end of the current scope), declare it prefixed with `autorelease`. For example:
```c
void func()
{
    // allocate an autoreleased char* of 20 bytes
    autorelease char* my_str = lm_alloc(20);
} // at this line, my_str will have lm_release called on it, and it will be freed if this is the last reference to it
```

To override the deallocation method called when the reference count hits zero, use `lm_override_dealloc`. The function passed must take a single pointer, being the start of the data portion of the memory that was allocated (i.e., the first byte after the libmem metadata). The dealloc function must not call `free()` on this pointer, as libmem will still handle it's own resource management internally.

Example:
```c
// A my_data struct is allocated using libmem, however the FILE still needs to
// be closed when my_data is no longer needed
typedef struct
{
    FILE* file_descriptor;
} my_data;

void my_dealloc(void *m)
{
    my_data* data = (my_data*)m;
    fclose(data->file_descriptor);
}

void func()
{
    autorelease my_data* data = lm_alloc(sizeof(my_data));
    lm_override_dealloc(data, my_dealloc);
} // at this line, data will have no references left on it. my_dealloc will be called, and then the pointer will be freed
```