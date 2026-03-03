#include "syscall.h"
#include "tty.h"
#include "memory.h"
#include "../src/ai/ai_scheduler.h"

void syscall_init() {
    print_string("Initializing System Call Interface...\n");
    // In a real OS, we would set up an IDT entry for interrupt 0x80 here.
    // For this stub, we just rely on direct function calls to do_syscall() to
    // simulate a software interrupt entry point.
}

void* do_syscall(int syscall_num, void* arg1, void* arg2, void* arg3) {
    switch (syscall_num) {
        case SYSCALL_PRINT:
            print_string((const char*)arg1);
            return (void*)0;

        case SYSCALL_ALLOC_TENSOR:
        {
            unsigned long size = (unsigned long)arg1;
            return allocate_tensor_memory(size);
        }

        case SYSCALL_SCHEDULE_INFERENCE:
        {
            void* model_ptr = arg1;
            void* input_data_ptr = arg2;
            int intensity = (int)(unsigned long)arg3;
            schedule_inference_task(model_ptr, input_data_ptr, intensity);
            return (void*)0;
        }

        default:
            print_string("Unknown System Call: ");
            print_hex(syscall_num);
            print_string("\n");
            return (void*)-1;
    }
}
