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

    ; Push multiboot info struct pointer and magic number
    push ebx
    push eax

    ; Call the high-level kernel
    call kernel_main

    ; We successfully printed to screen, now output something to serial so we know it worked.
    ; This serial port printing code is a hack just to prove execution worked without needing VGA inspection
    mov dx, 0x3f8
    mov al, 'D'
    out dx, al
    mov al, 'O'
    out dx, al
    mov al, 'N'
    out dx, al
    mov al, 'E'
    out dx, al
    mov al, 10
    out dx, al

    ; Quit qemu hack
    mov dx, 0x501
    mov al, 0x31
    out dx, al

    ; Hang if kernel_main returns
    cli
.hang:
    hlt
    jmp .hang
