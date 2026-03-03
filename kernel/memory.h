#ifndef MEMORY_H
#define MEMORY_H

#include "multiboot.h"

// Initialize the physical memory manager using the multiboot memory map
void pmm_init(multiboot_info_t *mbi);

// Allocate a 4KB page
void* pmm_alloc_page(void);

// Free a 4KB page
void pmm_free_page(void* ptr);

// Allocate a large contiguous physical memory block optimized for tensors
// Returns the physical address of the allocated block, or 0 if failed
void* allocate_tensor_memory(unsigned long size);

#endif
