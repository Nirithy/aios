#include "memory.h"
#include "tty.h"

#define PAGE_SIZE 4096

// Simplified Physical Memory Manager using a bump allocator for tensors
// and tracking simple page allocations. A bitmap approach would be better
// for full implementation, but for prototyping the AI OS we will start
// with a basic bump allocator using available RAM regions.

typedef struct {
    unsigned long start;
    unsigned long length;
    unsigned long used;
} memory_region_t;

#define MAX_REGIONS 16
memory_region_t regions[MAX_REGIONS];
int num_regions = 0;

void pmm_init(multiboot_info_t *mbi) {
    if (!(mbi->flags & (1 << 6))) {
        print_string("PMM Error: No memory map provided by bootloader\n");
        return;
    }

    print_string("Initializing PMM...\n");

    memory_map_t *mmap = (memory_map_t *)mbi->mmap_addr;
    while ((unsigned long)mmap < mbi->mmap_addr + mbi->mmap_length) {
        // Type 1 is available RAM
        if (mmap->type == 1) {
            // Ignore low memory (below 1MB) and small chunks
            if (mmap->base_addr_low >= 0x100000 && num_regions < MAX_REGIONS) {
                regions[num_regions].start = mmap->base_addr_low;
                regions[num_regions].length = mmap->length_low;
                regions[num_regions].used = 0;

                print_string("Added RAM Region: Base ");
                print_hex(regions[num_regions].start);
                print_string(" Size ");
                print_hex(regions[num_regions].length);
                print_string("\n");

                num_regions++;
            }
        }
        mmap = (memory_map_t *)((unsigned long)mmap + mmap->size + sizeof(mmap->size));
    }
}

void* pmm_alloc_page(void) {
    // Basic bump allocator for pages
    for (int i = 0; i < num_regions; i++) {
        if (regions[i].used + PAGE_SIZE <= regions[i].length) {
            void* ptr = (void*)(regions[i].start + regions[i].used);
            regions[i].used += PAGE_SIZE;
            return ptr;
        }
    }
    return 0; // Out of memory
}

void pmm_free_page(void* ptr) {
    // Bump allocator doesn't support freeing individual pages easily
    // In a real bitmap allocator, we would mark the bit as free.
    (void)ptr;
}

void* allocate_tensor_memory(unsigned long size) {
    // Tensors often need large contiguous blocks and alignment
    // We align to a larger boundary (e.g., 2MB) if possible, but for this
    // minimal implementation, we'll align to 4KB (page size) and allocate

    // Page align the requested size
    if (size % PAGE_SIZE != 0) {
        size = size + (PAGE_SIZE - (size % PAGE_SIZE));
    }

    for (int i = 0; i < num_regions; i++) {
        // Page align the current used offset
        unsigned long current_offset = regions[i].used;
        if (current_offset % PAGE_SIZE != 0) {
            current_offset = current_offset + (PAGE_SIZE - (current_offset % PAGE_SIZE));
        }

        if (current_offset + size <= regions[i].length) {
            void* ptr = (void*)(regions[i].start + current_offset);
            regions[i].used = current_offset + size;
            return ptr;
        }
    }

    print_string("Failed to allocate tensor memory of size: ");
    print_hex(size);
    print_string("\n");
    return 0;
}
