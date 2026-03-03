#include "multiboot.h"
#include "tty.h"
#include "memory.h"
#include "../src/ai/gpu_manager.h"
#include "../src/ai/ai_scheduler.h"
#include "../src/ai/tensor.h"

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

        // Test tensor math library
        print_string("Testing Tensor Math Library...\n");
        tensor_t *a = tensor_create(2, 2);
        tensor_t *b = tensor_create(2, 2);

        if (a && b) {
            a->data[0] = 1; a->data[1] = 2;
            a->data[2] = 3; a->data[3] = 4;

            b->data[0] = 5; b->data[1] = 6;
            b->data[2] = 7; b->data[3] = 8;

            tensor_t *c = tensor_matmul(a, b);
            if (c) {
                print_string("Matrix Multiplication Result [0,0]: ");
                print_hex((unsigned long)c->data[0]);
                print_string("\n");
            } else {
                print_string("Failed to multiply tensors.\n");
            }
        } else {
            print_string("Failed to create tensors.\n");
        }
    } else {
        print_string("Memory map not provided by bootloader\n");
    }

    // Initialize the AI scheduler and hardware here.
    ai_scheduler_init();
    gpu_manager_init();

    // Schedule test inference tasks
    print_string("Scheduling Inference Tasks...\n");
    schedule_inference_task((void*)0x1000, (void*)0x2000, 50);  // Medium intensity
    schedule_inference_task((void*)0x3000, (void*)0x4000, 95);  // High intensity
    schedule_inference_task((void*)0x5000, (void*)0x6000, 10);  // Low intensity

    print_string("Tick 1: ");
    ai_scheduler_tick();

    print_string("Tick 2: ");
    ai_scheduler_tick();

    print_string("Tick 3: ");
    ai_scheduler_tick();
}
