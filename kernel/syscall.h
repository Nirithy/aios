#ifndef SYSCALL_H
#define SYSCALL_H

#define SYSCALL_PRINT 1
#define SYSCALL_ALLOC_TENSOR 2
#define SYSCALL_SCHEDULE_INFERENCE 3

void syscall_init();

// A generic syscall dispatcher function (in a real OS this would be handled via interrupt 0x80)
void* do_syscall(int syscall_num, void* arg1, void* arg2, void* arg3);

#endif
