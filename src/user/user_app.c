#include "user_app.h"

// Note: In a real operating system, these would be wrappers around an `int 0x80` assembly block.
// We are simulating the boundary here by calling the dispatcher.
extern void* do_syscall(int syscall_num, void* arg1, void* arg2, void* arg3);

#define SYSCALL_PRINT 1
#define SYSCALL_ALLOC_TENSOR 2
#define SYSCALL_SCHEDULE_INFERENCE 3

void user_print(const char* str) {
    do_syscall(SYSCALL_PRINT, (void*)str, (void*)0, (void*)0);
}

void* user_alloc_tensor(unsigned long size) {
    return do_syscall(SYSCALL_ALLOC_TENSOR, (void*)size, (void*)0, (void*)0);
}

void user_schedule_inference(void* model, void* input, int intensity) {
    do_syscall(SYSCALL_SCHEDULE_INFERENCE, model, input, (void*)(unsigned long)intensity);
}

void execute_user_app() {
    user_print("\n--- ENTERING USER SPACE ---\n");
    user_print("User App: Requesting 4MB Tensor Allocation...\n");
    void* user_tensor = user_alloc_tensor(4 * 1024 * 1024);

    if (user_tensor) {
        user_print("User App: Got Tensor!\n");
    }

    user_print("User App: Scheduling Background AI Task...\n");
    user_schedule_inference((void*)0x8000, (void*)0x9000, 75);

    user_print("--- EXITING USER SPACE ---\n\n");
}
