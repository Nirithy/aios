#include "multiboot.h"
#include "tty.h"
#include "memory.h"
#include "../src/ai/gpu_manager.h"

void kernel_main(unsigned long magic, unsigned long addr) {
    clear_screen();
    print_string("Hello AI OS - Booting the Future of Computing\n");

    if (magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        print_string("Invalid magic number: ");
        print_hex(magic);
        print_string("\n");
        return;
    }

    multiboot_info_t *mbi = (multiboot_info_t *)addr;

    if (mbi->flags & (1 << 6)) {
        print_string("Memory map detected.\n");
        // Initialize the Physical Memory Manager (PMM)
        pmm_init(mbi);

        // Test tensor memory allocator
        unsigned long tensor_size = 1024 * 1024 * 16; // 16MB tensor
        print_string("Allocating a 16MB tensor...\n");
        void* tensor_ptr = allocate_tensor_memory(tensor_size);
        if (tensor_ptr) {
            print_string("Successfully allocated 16MB tensor at: ");
            print_hex((unsigned long)tensor_ptr);
            print_string("\n");
        } else {
            print_string("Failed to allocate tensor.\n");
        }
    } else {
        print_string("Memory map not provided by bootloader\n");
    }

    // In a real OS, we'd initialize the AI scheduler and hardware here.
    // ai_scheduler_init();
    gpu_manager_init();
}
