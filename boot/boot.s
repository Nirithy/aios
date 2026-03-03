MBALIGN     equ  1 << 0                   ; align loaded modules on page boundaries
MEMINFO     equ  1 << 1                   ; provide memory map
FLAGS       equ  MBALIGN | MEMINFO        ; this is the Multiboot 'flag' field
MAGIC       equ  0x1BADB002               ; 'magic number' lets bootloader find the header
CHECKSUM    equ -(MAGIC + FLAGS)          ; checksum of above, to prove we are multiboot

section .multiboot_header
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

section .bss
align 16
stack_bottom:
resb 16384 ; 16 KiB
stack_top:

section .text
global start
extern kernel_main

start:
    ; Set up stack
    mov esp, stack_top

    ; Call the high-level kernel
    call kernel_main

    ; Hang if kernel_main returns
    cli
.hang:
    hlt
    jmp .hang
