dmalloc
===================

A type of dynamic memory allocator that helps to debug dynamic memory usage.

## Features
- Tracks information about dynamic memory allocation.
- Catches common memory programming errors (e.g., freeing the same block twice, trying to allocate too much memory etc).
- Detects a write outside a dynamically allocated block (e.g., writing 65 bytes into a 64-byte piece of memory).
