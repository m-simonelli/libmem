### A tiny reference counting library for C/C++
To use, import the libmem.h file into your project.
Allocations are made with `lm_alloc`, used in the same way as `malloc`.
To take ownership of an object, call `lm_retain`. To relinquish, call `lm_release`.
To create a copy of an object, call `lm_copy`. The new object will have a reference count of 1.